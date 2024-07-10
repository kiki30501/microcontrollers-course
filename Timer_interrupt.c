/********************************************************************/
/*	Title:			 	timer state machine with interrupt			*/
/*	File name: 			Timer_interrupt.c			 				*/
/*	Course: 			Micro-processors and micro-controllers		*/
/*	Created: 			29.12.2022 									*/
/*	Last modified:  	29.12.2022									*/
/* 	Last modified by:	Reshef Schachter							*/
/* 	Author:				Reshef Schachter							*/
/* 	Company:			Reshef Schachter							*/
/********************************************************************/


#include "DSP28x_Project.h"    		// Device Header file and Examples Include File
#include "Timer_StateMachine.h"

interrupt void MyBgProg(void);
/*-----------------------------------------------------------------------------------------------------------*/
void config_timer0_1ms_MyBgProg()
{
//addition to allow pie usage with timer0 at 1ms
   EALLOW;
   PieVectTable.TINT0 = &MyBgProg;
   EDIS;
   InitCpuTimers();
   ConfigCpuTimer(&CpuTimer0, 150, 1000);
   CpuTimer0Regs.TCR.all = 0x4001;
   IER |= M_INT1;
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
   EINT;
   ERTM;
}
/*-----------------------------------------------------------------------------------------------------------*/
int Start, Stop;
interrupt void MyBgProg(void)
{
	CpuTimer0.InterruptCount++;
	Start = IsButton(4);
	Stop = IsButton(1);
	RunStateMachine(Start, Stop);
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
/*-----------------------------------------------------------------------------------------------------------*/
