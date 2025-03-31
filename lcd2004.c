/* LCD20x4 i2c Driver for STM32 */
/*   My Device: STM32F103C8T6   */
/*        Ian Javik, 2025       */
/*       HAL Library Based      */
/*     Source: ControllerTech   */

#include "lcd2004.h"

#define LCD_I2C_ADDR 0x4e // I2C device slave address
                          // My adapter address is 0x4e

extern I2C_HandleTypeDef hi2c1;
 
/*                   I2C Adapter Board                           */
/* P7 - P4  : Data                                               */
/* P3       : Useless, defalut 1                                 */
/* P2, or EN: Enable LCD to receive data                         */
/* P1, or RS: RS = 0, data is command; RS = 1, data is data      */
/* P0, or RW: RW = 0, write; RW = 1, read                        */
/*                                                               */
/* For write-only, the format should be xxxx 1100 or xxxx 1110   */
/* A data or command is xxxx xxxx, you must send its high 4 Bits */
/* and low 4 Bits seperately.                                    */
                                          

/******************************************************************/
/* To send command to LCD, EN = 1,RS = 0, RW = 0                  */
/* So P3 - P0 is 1100, and 1100(b) = C(hex)                       */
/* the format should be <data> | 0xC0                             */
/* and when it is over, EN = 0                                    */
/* the format should be <data> | 0x80                             */

void LCD_Send_Command(char cmd) // e.g 0x3F
{
    char data_h, data_l;        // the high / low 4 Bits of cmd
    uint8_t data_t[4];          // What will be sent via I2C

    /* Step.1 figure out the High and Low 4 Bits */
    data_h =  cmd       & 0xf0; // result: 0x30(H)
    data_l = (cmd << 4) & 0xf0; // result: 0xF0(L)

    /* Step.2 processing data */
    /* High 4 */
    data_t[0] = data_h | 0x0C;    //EN = 1, RS = 0, RW = 0
    data_t[1] = data_h | 0x08;    //EN = 0, RS = 0, RW = 0

    /* Low 4 */
    data_t[2] = data_l | 0x0C;    //EN = 1, RS = 0, RW = 0
    data_t[3] = data_l | 0x08;    //EN = 1, RS = 0, RW = 0

    /* step.3 Send data via I2C */
    HAL_I2C_Master_Transmit(&hi2c1,LCD_I2C_ADDR,data_t,4,100);
}

/*****************************************************************/
/* pretty much the same but RS = 1                               */
void LCD_Send_Data(char data) 
{
    char data_h, data_l;       
    uint8_t data_t[4];            
                          
    data_h = data        & 0xf0; 
    data_l = (data << 4) & 0xf0; 
                                                        
    data_t[0] = data_h | 0x0D;    //EN = 1, RS = 0, RW = 0
    data_t[1] = data_h | 0x09;    //EN = 0, RS = 0, RW = 0

    data_t[2] = data_l | 0x0D;    //EN = 1, RS = 0, RW = 0
    data_t[3] = data_l | 0x09;    //EN = 1, RS = 0, RW = 0

    HAL_I2C_Master_Transmit(&hi2c1,LCD_I2C_ADDR,data_t,4,100);
}

void LCD_Clear()
{
    LCD_Send_Command(0x01); 
}

/* Initialization Time Sequence */
void LCD_Init (void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	LCD_Send_Command (0x30);
	HAL_Delay(5);  // wait for >4.1ms
	LCD_Send_Command (0x30);
	HAL_Delay(1);  // wait for >100us
	LCD_Send_Command (0x30);
	HAL_Delay(10);
	LCD_Send_Command (0x20);  // 4bit mode
	HAL_Delay(10);

    // dislay initialisation
    LCD_Send_Command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD_Send_Command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	LCD_Send_Command (0x01);  // clear display
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_Send_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_Send_Command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/* disaplay a string */
void LCD_Disp_String(char *str)  //e.g. "Hell no, World!"
{
	while (*str)                 //if the current char is not null
        LCD_Send_Data (*str++);  //keep on sending the next char
}


/************************************************************************************************/


/* My customized functions */

/* display a string on a specific line */
void LCD_Disp_Line(int8_t line,char *str) // line is LINEx, where x could be 1-4
{                                         // defined in the header file
    LCD_Send_Command(line);
    LCD_Disp_String(str);
}
