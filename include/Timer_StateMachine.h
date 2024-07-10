
//###########################################################################
//
// FILE:   LCD2x16Display.h
//
// TITLE:  LCD text 2x16 Display
//
//###########################################################################

#ifndef TIMER_STATEMACHINE_H
#define TIMER_STATEMACHINE_H

void SetExtKeyboard(void);
extern int IsButton(int button);
extern void RunStateMachine(int Start, int Stop);

void BuzzerOff(void);

#endif  // end of LCD2x16DISPLAY_H definition
