/*

			My Musical Keyboard
			Reshef Schachter
			Last edit: 12.01.23
			Based on Flaxer's keyboard matrix file


*/
#include "DSP28x_Project.h" 	// Device Header file and Examples Include File
#include "ECAP_Defines.h"
#include "LCD2x16Display.h"

void StopNote(void);
void PlayNote(int note);

char pressed = 'Y';
int kbCounter = 0;
int volume = 5;

void MaryHadaLittleLamb(void);
interrupt void Xint3456_isr(void);

/*-----------------------------------------------------------------------------------------------------------*/
inline static void KeboardWriteCode(char data)
 {
	GpioDataRegs.GPBCLEAR.all = (0x0FL << 8); 		// Clear 4 data bits GPIO40-GPIO43
	GpioDataRegs.GPBSET.all = ((long)data << 8); 	// Set the relevant data bits GPIO40-GPIO43
	DELAY_US(1000);
 }
/*-----------------------------------------------------------------------------------------------------------*/
inline static char KeboardReadCode()
 {
	return((GpioDataRegs.GPBDAT.all >> 12) & 0x0FL);// Read 4 data bits GPIO44-GPIO47
 }
/*-----------------------------------------------------------------------------------------------------------*/
void Beep(int MiliSec)
 {
	GpioDataRegs.GPASET.bit.GPIO27 = 1; 	// Buzzer On
	DELAY_US(1000L*MiliSec);
	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1; 	// Buzzer Off
 }
/*-----------------------------------------------------------------------------------------------------------*/
static char scan2ascii(char sc)
 {
	switch (sc)
	  {
		case 0xE0: return('1');
		case 0xD0: return('2');
		case 0xB0: return('3');
		case 0x70: return('A');
		case 0xE1: return('4');
		case 0xD1: return('5');
		case 0xB1: return('6');
		case 0x71: return('B');
		case 0xE2: return('7');
		case 0xD2: return('8');
		case 0xB2: return('9');
		case 0x72: return('C');
		case 0xE3: return('*');
		case 0xD3: return('0');
		case 0xB3: return('#');
		case 0x73: return('D');
	  }
	return(0);
 }
/*-----------------------------------------------------------------------------------------------------------*/
char ReadKB(char wait)		/*** IMPORTANT! this function assumes only 1 key is being pressed at a time ***/
 {
	static char code[] = {0xE, 0xD, 0xB, 0x7}; 	// Will be used to rotate through the rows
	char data;
	char i;
	KeboardWriteCode(0x0);				// Clear all rows
	DELAY_US(1000);
	while(KeboardReadCode() == 0x0F)	// While the 4 data bits GPIO44-GPIO47 == 1 (no pressed buttons)
		if (!wait) return(0);			// If wait=0, the function will not wait until input is given
										// If wait!=0, the function will wait until input is given

	for(i=0; i<4; i++)					// Keyboard matrix scanning algorithm
		{
		KeboardWriteCode(code[i]);
		DELAY_US(1000);					// Waiting for voltages to stabilize
		data = KeboardReadCode();
		if (data != 0x0F)				// Check if a button is pressed
			break;
		}
	pressed = scan2ascii((data << 4) | i);
	PlayNote(pressed);					// Play the note and print the frequency on the LCD
	while(KeboardReadCode() != 0x0F);	// Wait for button release
	StopNote();							// Stop note and clear the LCD
	KeboardWriteCode(0x0);				// Clear columns
	DELAY_US(3000);
	return(0);
 }
/*-----------------------------------------------------------------------------------------------------------*/
char PushButtonRead(int Inx) //Takes input should be between 0-3
// This function  is used to check if one of the 4 "dumb" buttons is pressed
  {
	switch (Inx)
	{
		case 0:
			return (GpioDataRegs.GPCDAT.bit.GPIO80);
		case 1:
			return (GpioDataRegs.GPCDAT.bit.GPIO81);
		case 2:
			return (GpioDataRegs.GPCDAT.bit.GPIO82);
		case 3:
			return (GpioDataRegs.GPCDAT.bit.GPIO83);
	}
	return(0);
  }
/*-----------------------------------------------------------------------------------------------------------*/
void ConfigAndInstallKBInt()
{
	EALLOW; // This is needed to write to EALLOW protected registers

	// Set input qualification period for GPIO44-GPIO47
	GpioCtrlRegs.GPBCTRL.bit.QUALPRD3=1;// Qualifier period = SYSCLKOUT/2
	GpioCtrlRegs.GPBQSEL1.bit.GPIO44=2; // 6 samples
	GpioCtrlRegs.GPBQSEL1.bit.GPIO45=2; // 6 samples
	GpioCtrlRegs.GPBQSEL1.bit.GPIO46=2; // 6 samples
	GpioCtrlRegs.GPBQSEL1.bit.GPIO47=2; // 6 samples

	GpioCtrlRegs.GPBPUD.bit.GPIO40 = 0;	// Pull-up enabled for GPIO40
	GpioCtrlRegs.GPBPUD.bit.GPIO41 = 0;	// Pull-up enabled for GPIO41
	GpioCtrlRegs.GPBPUD.bit.GPIO42 = 0;	// Pull-up enabled for GPIO42
	GpioCtrlRegs.GPBPUD.bit.GPIO43 = 0;	// Pull-up enabled for GPIO43

	GpioIntRegs.GPIOXINT3SEL.all = 12; 	// Xint3 connected to GPIO44 32+12
	GpioIntRegs.GPIOXINT4SEL.all = 13; 	// Xint4 connected to GPIO45 32+13
	GpioIntRegs.GPIOXINT5SEL.all = 14; 	// Xint5 connected to GPIO46 32+14
	GpioIntRegs.GPIOXINT6SEL.all = 15; 	// Xint6 connected to GPIO47 32+15

	PieVectTable.XINT3 = &Xint3456_isr;
	PieVectTable.XINT4 = &Xint3456_isr;
	PieVectTable.XINT5 = &Xint3456_isr;
	PieVectTable.XINT6 = &Xint3456_isr;
	EDIS; // This is needed to disable write to EALLOW protected registers

	// Enable XINT 3,4,5,6 in the PIE: Group 12 interrupt 1-4
	// Enable int1 which is connected to WAKEINT:
	PieCtrlRegs.PIECTRL.bit.ENPIE = 1; 	// Enable the PIE block
	PieCtrlRegs.PIEIER12.bit.INTx1 = 1; // Enable PIE Group 12 INT1
	PieCtrlRegs.PIEIER12.bit.INTx2 = 1; // Enable PIE Group 12 INT2
	PieCtrlRegs.PIEIER12.bit.INTx3 = 1; // Enable PIE Group 12 INT3
	PieCtrlRegs.PIEIER12.bit.INTx4 = 1; // Enable PIE Group 12 INT4

	// Enable CPU int12
	IER |= M_INT12;

	// Configure XINT3-6
	XIntruptRegs.XINT3CR.bit.POLARITY = 0; // Falling edge interrupt
	XIntruptRegs.XINT4CR.bit.POLARITY = 0; // Falling edge interrupt
	XIntruptRegs.XINT5CR.bit.POLARITY = 0; // Falling edge interrupt
	XIntruptRegs.XINT6CR.bit.POLARITY = 0; // Falling edge interrupt

	// Enable XINT3-6
	XIntruptRegs.XINT3CR.bit.ENABLE = 1; // Enable XINT3
	XIntruptRegs.XINT4CR.bit.ENABLE = 1; // Enable XINT4
	XIntruptRegs.XINT5CR.bit.ENABLE = 1; // Enable XINT5
	XIntruptRegs.XINT6CR.bit.ENABLE = 1; // Enable XINT6

	EINT;
	ERTM;

}

/*-----------------------------------------------------------------------------------------------------------*/
void CFG_ECAP_APWM()		// Configure all necessary properties for using ECAP for PWM output
{
	EALLOW;

	//GpioCtrlRegs.GPAMUX1.all = 0x00000008;			// Set gpio1 for peripheral 2
	ECap6Regs.ECCTL2.bit.CAP_APWM 	= EC_APWM_MODE;	// Set to APWM mode
	ECap6Regs.ECCTL2.bit.APWMPOL 	= EC_ACTV_HI;	// Active-High mode
	ECap6Regs.ECCTL2.bit.SYNCI_EN 	= EC_DISABLE;	// Disable Sync-in
	ECap6Regs.ECCTL2.bit.SYNCO_SEL 	= EC_SYNCO_DIS;	// Disable Sync-out
	ECap6Regs.CAP1 		= 0x1000;					// Set Period Value
	ECap6Regs.CAP2 		= 0x0;						// Set the duty-cycle on-time
	ECap6Regs.CTRPHS 	= 0x0;						// Zero phase delay
	// I need to check if I should add a line for PCLKCR1 (page 41 in the manual)
	ECap6Regs.ECCTL2.bit.TSCTRSTOP 	= EC_RUN;		// Freeze the time-stamp counter

	EDIS;
}

/*-----------------------------------------------------------------------------------------------------------*/
static int long kb2Note(char key)	// Convert ASCII code to corresponding frequency - conversion table
// And print the frequency being played on the LCD
{
	switch (key)
		  {
			case '1':
				PrintLCD("523Hz");
				return(286800);		// About 523Hz

			case '2':
				PrintLCD("554Hz");
				return(270760);		// About 554Hz

			case '3':
				PrintLCD("587Hz");
				return(255530);		// About 587Hz

			case 'A':
				PrintLCD("622Hz");
				return(241160);		// About 622Hz

			case '4':
				PrintLCD("660Hz");
				return(227270);		// About 660Hz

			case '5':
				PrintLCD("700Hz");
				return(214280);		// About 700Hz

			case '6':
				PrintLCD("740Hz");
				return(202700);		// About 740Hz

			case 'B':
				PrintLCD("784Hz");
				return(191300); 	// About 784Hz

			case '7':
				PrintLCD("830Hz");
				return(180700);		// About 830Hz

			case '8':
				PrintLCD("880Hz");
				return(170450);		// About 880Hz

			case '9':
				PrintLCD("932Hz");
				return(160950);		// About 932Hz

			case 'C':
				PrintLCD("988Hz");
				return(151800);		// About 988Hz

			case '*':
				PrintLCD("1046Hz");
				return(143400);		// About 1046Hz
		  }
	PrintLCD("1.5 kHz");
	return(100000);		// About 1.5kHz
}

/*-----------------------------------------------------------------------------------------------------------*/
void PlayNote(int note)// For a given note, change the values of the ECAP registers to note
{
	static int long period;
	switch (note)
		{
			case '0':	// Play a demo song
			{
				MaryHadaLittleLamb();
				break;
			}

			case '#':	// Decrease volume by lowering the Duty-Cycle
			{
				if(volume > 0) volume--;
				break;
			}

			case 'D':	// Increase volume by lowering the Duty-Cycle
			{
				if(volume < 5) volume++;
				break;
			}
			default:	// Set register values based on volume and note
			{
				period = kb2Note(note);
				ECap6Regs.CAP3 = period;
				ECap6Regs.CAP4 = (period * volume / 10);	// period * Duty-Cycle
				break;
			}
		}
	}

/*-----------------------------------------------------------------------------------------------------------*/
void StopNote()	// Stops any output through ECAP-PWM through the GPIO to the BNC
{
	ECap6Regs.CAP4 = 0;
	ClearLCD();
}

/*-----------------------------------------------------------------------------------------------------------*/
void MaryHadaLittleLamb() // Demo song: "Mary had a Little Lamb"
{
	const char song[26] = {'4','3','1','3','4','4','4','3','3','3','4','B','B','4','3','1','3','4','4','4','4','3','3','4','3','1'};
	const char duration[26] = {1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,4};
	int long SecBPM = 600000L; // For 100 BPM song
	int i = 0;
	for(i = 0 ; i <= 25 ; i++)
	{
		if((GpioDataRegs.GPCDAT.all & 0xF0000) != 0xF0000) break; // Stop the song using any of the push buttons
		PlayNote(song[i]);
		DELAY_US(SecBPM * duration[i]);		// How long should the note be played
		StopNote();
		DELAY_US(100);		// Added small delay to make the song feel more natural
	}
}

/*-----------------------------------------------------------------------------------------------------------*/
interrupt void Xint3456_isr(void)
{
	ReadKB(0);	// Use the ReadKB function without waiting for input
	// Acknowledge this interrupt to receive more interrupts from group 12
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

/*-----------------------------------------------------------------------------------------------------------*/

