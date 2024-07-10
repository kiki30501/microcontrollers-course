//###########################################################################
//
// FILE:    Flax_DelfinoEvbGpioToggle.c
//
// TITLE:   DSP2833x Device GPIO toggle test program. 
// This template was written by Eli Flaxer for DelfinoEvb Evaluation Board
//
//
//###########################################################################
// $TI Release: 2833x/2823x Header Files and Peripheral Examples V133 $
// $Release Date: June 8, 2012 $
//###########################################################################

#include "Reshef_MicroTestExp2.h"
#include "Reshef_MicroTestExp3.h"
#include "Timer_interrupt.h"
#include "Timer_StateMachine.h"
#include "DSP28x_Project.h"    		// Device Header file and Examples Include File
#include "LCD2x16Display.h"
#include "FinalProject_Micro_ReshefV2.h" // My Course final project V2
#include "ECAP_Defines.h"

void DelfinoEvbGpioSelect(void);
void MyMainProg(void);

int32 MyDelayLoop = 200000L;


/*****************************************************************************/
void GpioCSetClear(int k,int x)
{
	if (x)
		GpioDataRegs.GPCSET.all = (1L<<k);
	else
		GpioDataRegs.GPCCLEAR.all = (1L<<k);
}
/*****************************************************************************/
void main(void)
{

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();
   
// Step 2. Initalize GPIO: 
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example
 
// For this example use the following configuration:
   DelfinoEvbGpioSelect();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();
	
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
	
// Step 5. User specific code:
	
// This example uses DATA, SET, CLEAR and TOGGEL registers to toggle I/O's
   MyMainProg();
}
/*****************************************************************************/
void MyMainProg(void) // User code:
{ 
	GpioDataRegs.GPASET.bit.GPIO27 = 1; 	// Buzzer On
	DELAY_US(1000L*20);
	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1; 	// Buzzer Off
	InItLCD();			// Initialize the LCD
	DELAY_US(10000L);
	BackLightLCD(1);	// Turn on LCD back-light
	DELAY_US(10000L);

	ConfigAndInstallKBInt();	// Set all Gpio and Interrupt required for XINT with the Keyboard
	CFG_ECAP_APWM();			// Initialize the ECAP6 module as APWM
	while(1)
		DELAY_US(1000);
}
/*****************************************************************************/
void DelfinoEvbGpioSelect(void)
{
    EALLOW;
	GpioCtrlRegs.GPAMUX1.all = 0x00000008;  	// All GPIO and GPIO1 set to peripheral 2
    GpioCtrlRegs.GPADIR.all = 0x08000002;		// All GPIO on channel A set as input except GPIO1 = output
    GpioCtrlRegs.GPBDIR.all = 0x07FF0F00;   	// Extended Bus Direction GPIO40-GPIO47 KB
    GpioCtrlRegs.GPCDIR.all = 0x0000FFFF;   	// Extended Bus Direction GPIO80-GPIO83 Button
    EDIS;
}
/*****************************************************************************/

