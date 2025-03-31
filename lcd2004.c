/* LCD20x4 i2c Driver for STM32 */
/*   My Device: STM32F103C8T6   */
/*        Ian Javik, 2025       */
/*       HAL Library Based      */
/*     Source: ControllerTech   */

#include "lcd2004.h"
extern I2C_HandleTypeDef hi2c1; // Don't put this in the header file

/*                 LCD Pins Config (may varies)                  */
/* P7 - P4  : Data                                               */
/* P3       : Useless, defalut 1                                 */
/* P2, or EN: Enable LCD to receive data                         */
/* P1, or RW: RW = 0, write; RW = 1, read                        */
/* P0, or RS: RS = 0, data is command; RS = 1, data is data      */
/*                                                               */
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
    data_t[0] = data_h | LCD_EN  | LCD_RS_COMMAND;    //EN = 1, RS = 0
    data_t[1] = data_h | LCD_RS_COMMAND;              //EN = 0, RS = 0

    /* Low 4 */
    data_t[2] = data_l | LCD_EN  | LCD_RS_COMMAND;    //EN = 1, RS = 0
    data_t[3] = data_l | LCD_RS_COMMAND;              //EN = 0, RS = 0

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
                                                        
    data_t[0] = data_h | LCD_EN  | LCD_RS_DATA;    //EN = 1, RS = 0
    data_t[1] = data_h | LCD_RS_DATA;              //EN = 0, RS = 0

    data_t[2] = data_l | LCD_EN  | LCD_RS_DATA;    //EN = 1, RS = 0
    data_t[3] = data_l | LCD_RS_DATA;              //EN = 0, RS = 0

    HAL_I2C_Master_Transmit(&hi2c1,LCD_I2C_ADDR,data_t,4,100);
}

void LCD_Clear()
{
    LCD_Send_Command(LCD_CLEAR_FULL_SCREEN); 
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
	HAL_Delay(2);
	LCD_Send_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_Send_Command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/* display a string */
void LCD_Disp_Str(char *str)  //e.g. "Hell no, World!"
{
	while (*str)                 //if the current char is not null
        LCD_Send_Data (*str++);  //keep on sending the next char
}


/************************************************************************************************/

/* My customized functions */

/* display a string on a specific Row */
void LCD_Disp_String(int8_t row,int8_t col,char *str) 
{
    int8_t addr = row + (col-1);                                        
    LCD_Send_Command(addr);
    LCD_Disp_Str(str);
}

/* if you want to have a flickering row, you can use this function */
void LCD_Blink_String(int8_t row,int8_t col,char *str,int period) //period is the period of flickering
{
  LCD_Disp_String(row,1,"                     ");
  HAL_Delay(period);
  LCD_Disp_String(row,col,str);
  HAL_Delay(period);  
}

void LCD_Disp_Char(int8_t row,int8_t col,char ch)
{
    int8_t addr = row + (col-1);
    LCD_Send_Command(addr);
    LCD_Send_Data(ch);
}
