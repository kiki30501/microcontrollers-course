/********************************************************************/
/*	Title:			 	Reshef's Musical Keyboard					*/
/*	File name: 			FinalProject_Micro_Reshef.c 				*/
/*	Course: 			Micro-processors and micro-controllers		*/
/*	Created: 			29.12.2022 									*/
/*	Last modified:  	29.12.2022									*/
/* 	Last modified by:	Reshef Schachter							*/
/* 	Author:				Reshef Schachter							*/
/* 	Company:			Reshef Schachter							*/
/********************************************************************/

/*-----------------------------------------------------------------------------------------------------------*/
#include "DSP28x_Project.h"    		// Device Header file and Examples Include File
#include "KeyboardMat4x4.h"
#include "DSP2833x_Device.h"
#include "eCAP_Defines.h"

#define speaker 		// Defined to be the GPIO that connects to the BNC connector from port DOA on the board
#define ecap_us = (((((long double) 1 * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L) // 1 micro-second
/*-----------------------------------------------------------------------------------------------------------*/
void CFG_ECAP_PWM()		// Configure all necessary properties for using ECAP for PWM output
{
	ECap1Regs.CAP1 = 0x1000;						// Set Period Value
	ECap1Regs.CTRPHS = 0x0;							// Zero phase delay
	ECap1Regs.ECCTL2.bit.CAP_APWM = EC_APWM_MODE;	// Set to APWM mode
	ECap1Regs.ECCTL2.bit.APWMPOL = EC_ACTV_HI;		// Active-High mode
	ECap1Regs.ECCTL2.bit.SYNCI_EN = EC_DISABLE;		// Disable Sync-in
	ECap1Regs.ECCTL2.bit.SYNCO_SEL = EC_SYNCO_DIS;	// Disable Sync-out
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;		// Allow the time-stamp counter to run

	ECap1Regs.CAP2 = 0x300;							// Set the duty-cycle (in time units)
}
/*-----------------------------------------------------------------------------------------------------------*/
int kb2Note(char key)	// Convert ASCII code to corresponding frequency - conversion table
{
return 0;
}
/*-----------------------------------------------------------------------------------------------------------*/
void Note2ECAP(int note)// For a given note, change the values of the ECAP registers so that
// a signal is being output from the ECAP PWM to the speaker using the GPIO to the BNC
{

}
/*-----------------------------------------------------------------------------------------------------------*/
void StopNote()			// Stops any output through ECAP-PWM through the GPIO to the BNC
{						// Unknown if necessary, need to check

}
/*-----------------------------------------------------------------------------------------------------------*/
void PlayNote(int note)
{
	//if(note)
		//output the note to ECAP with: Note2ECAP(note);
	//else {that means that note=0}
		//stop output to ECAP using StopNote();
}
/*-----------------------------------------------------------------------------------------------------------*/
void MusicalKB(char wait) 	//I need to change this code so that it does my musical keyboard routine
							// I need to create: CFG_ECAP_PWM, kb2note, PlayNote
 {
	static char code[] = {0xE, 0xD, 0xB, 0x7}; 	// Will be used to rotate through the rows
	char data;
	char i;
	KeboardWriteCode(0x0);						// Clear all rows
	DELAY_US(1000);
	while(KeboardReadCode() == 0x0F)			// Check 4 data bits GPIO44-GPIO47
		if (!wait) return(0);					// If wait=0, the function will not wait until input is given
												// If wait!=0, the function will wait until input is given
	Beep(20);
	for(i=0; i<4; i++)							// Keyboard matrix scanning algorithm
		{
		KeboardWriteCode(code[i]);
		DELAY_US(1000);							// Waiting for voltages to stabilize
		data = KeboardReadCode();
		if (data != 0x0F)						// Check if a button is pressed
			break;
		}
	while(KeboardReadCode() != 0x0F);			// Wait for button release
	DELAY_US(1000);
	KeboardWriteCode(0x0);						// Clear columns
	return 0;									// Using scan2ascii return the ASCII value of the key pressed
												// The argument for scan2ascii is basically: (column, row)
 }
/*-----------------------------------------------------------------------------------------------------------*/
/*void config_timer0_1ms_MusicalKB()
{
// Addition to allow pie usage with timer0 at 1ms for MusicalKB
   EALLOW;
   PieVectTable.TINT0 = &MusicalKB;
   EDIS;
   InitCpuTimers();
   ConfigCpuTimer(&CpuTimer0, 150, 1000);
   CpuTimer0Regs.TCR.all = 0x4001;
   IER |= M_INT1;
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   EINT;
   ERTM;
}
*/
