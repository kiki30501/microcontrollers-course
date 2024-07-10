
//###########################################################################
//
// FILE:   LCD2x16Display.h
//
// TITLE:  LCD text 2x16 Display
//
//###########################################################################

#ifndef LCD2x16DISPLAY_H
#define LCD2x16DISPLAY_H

extern void PrintLCD(const char *str);
extern void InItLCD(void);
extern void ClearLCD(void);
extern void BackLightLCD(int x);
extern void PrintLineLCD(char line, const char *str);
void GoToLCD(char y, char x);

#endif  // end of LCD2x16DISPLAY_H definition

