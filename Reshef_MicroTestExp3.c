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
#include "LCD2x16Display.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void MicroTest3_Q2(char *str)
{
	InItLCD();
	ClearLCD();
	PrintLCD(str); //str is my name, entered in the main program
}
/**********************************************************************/

void AfekaScreen() //untested
{
	InItLCD();
	ClearLCD();
	BackLightLCD(1);
	while(1)
	{
		GoToLCD(0, 5);
		PrintLCD("Afeka");
		GoToLCD(1, 2);
		PrintLCD("Micro-Lab");
		DELAY_US(1000000L);
		ClearLCD();
		DELAY_US(1000000L);
	}
}
/**********************************************************************/
void revString(char str[], int length)
{
	int start = 0;
	int end = length -1;
	while (start < end)
	    {
			str[start] = str[start] + str[end];
			str[end] = str[start] - str[end];
			str[start] = str[start] - str[end];
	        start++;
	        end--;
	    }
}
/**********************************************************************/
char * Int2Ascii20(int num, char* str)
{
	int i = 0;
	int Neg = 0;
	int rem;
	if (num == 0)
	    {
	        str[i++] = '0';
	        str[i] = '\0';
	        return str;
	    }
	if (num < 0)
	    {
	        Neg = 1;
	        num = -1 * num;
	    }
	while (num != 0)
	    {
	        rem = num % 10;
	        str[i++] = rem + '0';
	        num = num/10;
	    }
	if (Neg)
	        str[i++] = '-';

	str[i] = '\0';
	revString(str, i);
	return str;
}
/**********************************************************************/
void SwitchToSum() //need to fix
{
	InItLCD();
	DELAY_US(10);
	ClearLCD();
	DELAY_US(10);
	BackLightLCD(1);
	long int SwNow = 0x10000;
	int a, b, sum;
	int e = 1;
	char * A;
	char * B;
	char * SUM;
	char * str;
	PrintLCD("Ready :)");
	DELAY_US(1);
	ClearLCD();
	DELAY_US(1);
	while(1)
	{
		GoToLCD(0, 0);
		DELAY_US(1);
		if((SwNow != (0x0000FF00 & (GpioDataRegs.GPADAT.all))) || e)
		{
			e = 0;
			ClearLCD();
			SwNow = 0x0000FF00 & (GpioDataRegs.GPADAT.all);
			a = ((0xF00 & SwNow) >> 8);
			b = ((0xF000 & SwNow) >> 12);
			sum = a + b;
			sprintf(A, "%d", a);
			sprintf(B, "%d", b);
			sprintf(SUM, "%d", sum);
			PrintLCD(A);
			PrintLCD("+");
			PrintLCD(B);
			PrintLCD("=");
			PrintLCD(SUM);
			DELAY_US(1000);
			free(str);
		}
	}
}
/**********************************************************************/
