/**********************************************************************
* Tittle: Delfino Evaluation Board Keyboard *
* Filename: KeyboardMat4x4.h *
* Date: 20-11-2014 *
* Last Modify: 20-11-2014 *
* File Version: 1.0 *
* *
* Author: Flaxer Eli *
* Company: Flaxer.net *
* *
**********************************************************************/
#ifndef KEYBOARDMAT4X4_H
#define KEYBOARDMAT4X4_H

interrupt void Xint3456_isr(void);
void Beep(int MiliSec);
static char scan2ascii(char sc);
char ReadKB(char wait);
char PushButtonRead(int Inx);
void ConfigAndInstallKBInt(void);

#endif
