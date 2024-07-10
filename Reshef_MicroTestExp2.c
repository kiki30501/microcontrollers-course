/**********************************************************************
 *   Tittle:  			Delfino Evaluation Board Lab experiment 2     *
 *   Filename:	    	Reshef_MicroTestExp2.c                        *
 *   Date:          	24-11-2022                                    *
 *   Last Modify:   	24-11-2022                                    *
 *   File Version:  	1.0                                           *
 *                                                                    *
 *   Author:  			Reshef Schachter                              *
 *   Company:			Reshef Schachter                              *
 *                                                                    *
 *                                                                    */

/**********************************************************************/

#include "DSP28x_Project.h"     // Device Header file and Examples Include File



void MyLedTrain_withSW8 (void) // My program for exercise 2- question 3
{
	long i;
	long j = (1<<8)-1<<8;
	i = 0x00000080; 							// initial condition for SW8=1
	if (GpioDataRegs.GPADAT.bit.GPIO12 == 0)
		i = 0x00010000; 						// initial condition for SW8=0
	while(1)
	{
	  GpioDataRegs.GPCSET.all = i;				// set the Led i is currently pointing to
	  GpioDataRegs.GPCCLEAR.all = j&(~i);		// clear other the other Led not pointed to by i
	  DELAY_US(500000L);
	  if (GpioDataRegs.GPADAT.bit.GPIO12 == 1) 	//running check for state if SW8 with appropriate operations
	  { if (i == 0x00008000)
		  	  		  {i = 0x00000080;}
		  	  i = i<<1;}
	  else
	  {
		  if (i == 0x00000100)
		  {i = 0x00010000;}
			  i = i>>1;
	  }
	}
}
/*****************************************************************************/
void MyLedSwitches(void) // My program for exercise 2- question 4
{
	while(1)
	{
		GpioDataRegs.GPCSET.all = (0x0000FF00)&(GpioDataRegs.GPADAT.all);
		DELAY_US(250000);
		GpioDataRegs.GPCCLEAR.all=0x0000FF00;
		DELAY_US(250000);
	}
}
