#ifndef _LCD2004_H_
#define _LCD2004_H_

/* LCD2004 Line address */
#define LINE1 0x80|0x00
#define LINE3 0x80|0x14
#define LINE2 0x80|0x40
#define LINE4 0x80|0x54

#include "linker.h"

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Disp_Line(int8_t line,char *str);

#endif
