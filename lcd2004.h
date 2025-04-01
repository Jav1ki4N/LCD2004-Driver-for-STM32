#ifndef _LCD2004_H_
#define _LCD2004_H_

#include "linker.h"

#define LCD_I2C_ADDR 0x4E // I2C device slave address
                          // My adapter address is 0x4e

#define ROW1 0x80|0x00    // LCD row address
#define ROW3 0x80|0x14
#define ROW2 0x80|0x40
#define ROW4 0x80|0x54
#define COL1 1
#define COL2 2
#define COL3 3
#define COL4 4
#define COL5 5
#define COL6 6
#define COL7 7
#define COL8 8
#define COL9 9
#define COL10 10
#define COL11 11
#define COL12 12
#define COL13 13
#define COL14 14
#define COL15 15
#define COL16 16
#define COL17 17
#define COL18 18
#define COL19 19
#define COL20 20

/* LCD Commands */                          
#define LCD_CLEAR_FULL_SCREEN 0X01

/* Data Mode */
#define LCD_EN         0x0C //1100

#define LCD_RW_WRITE   0X08 //1000
#define LCD_RW_READ    0X0A //1010

#define LCD_RS_COMMAND 0x08 //1000
#define LCD_RS_DATA    0X09 //1001

/* Customized Char */
#define LCD_CGRAM_ADDR 0x40
extern uint8_t heart_shape[];
void LCD_Init(void);

void LCD_Send_Command(char cmd);
void LCD_Send_Data(char data);

void LCD_Clear(void);

void LCD_Disp_String(int8_t row,int8_t col,char *str); 
void LCD_Blink_String(int8_t row,int8_t col,char *str,int period);

void LCD_Disp_Char(int8_t row,int8_t col,char ch);
void LCD_Blink_Char(int8_t row,int8_t col,char ch,int period);

void LCD_Add_Custom_Char(uint8_t index, const uint8_t* char_buf);
void LCD_Disp_Custom_Char(int8_t row,int8_t col,uint8_t index);
void LCD_Blink_Custom_Char(int8_t row,int8_t col,uint8_t index,int period);

#endif
