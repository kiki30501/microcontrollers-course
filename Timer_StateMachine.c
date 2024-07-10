
#include "DSP28x_Project.h"    		// Device Header file and Examples Include File
#include "LCD2x16Display.h"
#include "Reshef_MicroTestExp3.h"

/* S3=0xB=1011		S1=0xE=1110		S4=0x7=0111		S2=0xD=1101 */

typedef enum {IDLE, RUN, WAIT} StateMachineType;

int MyCounter = 0;
/**********************************************************************/
void SetExtKeyboard() 	// Set GpioDIR for 4 push buttons
{
		GpioCtrlRegs.GPCDIR.all = 0x0000FFFF;
}

/**********************************************************************/
int IsButton(int button) //checks state of button S1-S4, input is an int of 1-4
{
	int a = 1 << (button-1);
	int b = (~(GpioDataRegs.GPCDAT.all & 0x000F0000)) >> 16;
	if (a & b)
		return 1;
	else
		return 0;
}
/**********************************************************************/
void RunStateMachine (int Start, int Stop)
{
	static StateMachineType MyStateMachine = IDLE; // only puts the value ONCE and
	//not every time the function is called
	static int j = 0;
	static int a = 1;
	static char string[20];

	if(a)
	{
		InItLCD();
		DELAY_US(10);
		BackLightLCD(1);
		DELAY_US(10);
		ClearLCD();
		DELAY_US(10);
		PrintLCD("IDLE");
		DELAY_US(10);
		GoToLCD(0, 0);
		a--;
	}

	switch (MyStateMachine)
	{
		case IDLE:
			if (Start == 1)
				{
				MyStateMachine = RUN;
				ClearLCD();
				GoToLCD(0,0);
				PrintLCD("RUN");
				}
			MyCounter = 0;
			GoToLCD(1,0);
			PrintLCD("0");
			GpioDataRegs.GPCTOGGLE.bit.GPIO64 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO65 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1;


			break;

		case RUN:
			if (Stop == 1)
			{
				MyStateMachine = WAIT;
				ClearLCD();
				GoToLCD(0,0);
				PrintLCD("WAITING");
			}
			MyCounter++;
			GoToLCD(1,0);
			PrintLCD(Int2Ascii20(MyCounter, string));
			GpioDataRegs.GPCTOGGLE.bit.GPIO65 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO66 = 1;
			break;

		case WAIT:
			if (Start == 1)
			{
				MyStateMachine = RUN;
				ClearLCD();
				GoToLCD(0,0);
				PrintLCD("RUN");
			}
			else if ((Stop == 1) && (j>=1000))
				{
				MyStateMachine = IDLE;
				ClearLCD();
				GoToLCD(0,0);
				PrintLCD("IDLE");
				j = 0;
				}
			j++;
			GpioDataRegs.GPCTOGGLE.bit.GPIO66 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO65 = 1;
			GpioDataRegs.GPCCLEAR.bit.GPIO64 = 1;

			break;
	}
}
/**********************************************************************/
void BuzzerOff()
{

	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;	// Turn off the buzzer
	DELAY_US(10);
}



