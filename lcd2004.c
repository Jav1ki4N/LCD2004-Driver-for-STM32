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

    // display initialisation
    LCD_Send_Command (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	HAL_Delay(1);
	LCD_Send_Command (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	HAL_Delay(1);
	LCD_Send_Command (0x01); // clear display
	HAL_Delay(2);
	LCD_Send_Command (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	HAL_Delay(1);
	LCD_Send_Command (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

/* disaplay a string */
void LCD_Disp_Str(char *str)     //e.g. "Hell no, World!"
{
	while (*str)                 //if the current char is not null
        LCD_Send_Data (*str++);  //keep on sending the next char
}


/************************************************************************************************/
/*                               My customized functions                                        */
/************************************************************************************************/

/*
 *@brief: displaying a string at (Rowx,Colx)  
*/
void LCD_Disp_String(int8_t row,int8_t col,char *str) 
{
    int8_t addr = row + (col-1);                                        
    LCD_Send_Command(addr);
    LCD_Disp_Str(str);
}

/*
 *@brief: displaying a blinking string at (Rowx,Colx)  
*/
void LCD_Blink_String(int8_t row,int8_t col,char *str,int period) //period is the period of flickering
{
  LCD_Disp_String(row,1,"                     ");
  HAL_Delay(period);
  LCD_Disp_String(row,col,str);
  HAL_Delay(period);  
}

/*
 *@brief: displaying a char at (Rowx,Colx)  
*/
void LCD_Disp_Char(int8_t row,int8_t col,char ch)
{
    int8_t addr = row + (col-1);
    LCD_Send_Command(addr);
    LCD_Send_Data(ch);
}

/*
 *@brief: displaying a blinking char at (Rowx,Colx)  
*/
void LCD_Blink_Char(int8_t row,int8_t col,char ch,int period)
{
    LCD_Disp_Char(row,col,' ');
    HAL_Delay(period);
    LCD_Disp_Char(row,col,ch);
    HAL_Delay(period);
}

/* Every custom char his 8 bits       */
/* and the CGRAM has 64 bits in total */
/* the start address of CGRAM is 0x40 */
/* the address of the first character will be 0x40 + 0x00 */
/* while the second character will be 0x40 + 0x08 */
/* which is 0x40 + (index-1 * 8), consider the second character's index is 2 */
/* so for the <index> Character, the address of the first character will be 0x40 + (index-1 * 8) */
/* where index could be 1-8 */

/* Not a fan of counting from 0 */
/* if you wish to start from 0 */
/* replace "index - 1" with "index" and use index ranged from[0,7] */

// E.g. LCD_Add_Custom_Char(1,heart_shape) creates a ‘💖’ character as the 1st character of CGRAM
void LCD_Add_Custom_Char(uint8_t index, const uint8_t* char_buf) // Write a char customized in char_buf 
                                                                 // as the <index> char in CGRAM
 {
    if(index > 8 )index = 8; //we dont have rooms for >8 chars
    if(index <= 1)index = 1; //and 0 chars means nothing

    LCD_Send_Command(LCD_CGRAM_ADDR + ((index-1) * 8));          //locate the address of the character

    for(int cnt = 0; cnt < 7; cnt++) // write char data in the 7*5 space, 7 lines in total
    {
        LCD_Send_Data(char_buf[cnt]);
    }
}

//E.g. LCD_Disp_Custom_Char(ROW1,COL1,1) displays a ‘💖’ character in (1,1)
void LCD_Disp_Custom_Char(int8_t row,int8_t col,uint8_t index) // Display the <index> char in CGRAM
{
    if(index > 8 )index = 8;
    if(index <= 1)index = 1;

    int8_t addr = row + (col-1);
    LCD_Send_Command(addr);
    LCD_Send_Data(index-1); 
}

void LCD_Blink_Custom_Char(int8_t row,int8_t col,uint8_t index,int period)
{
    LCD_Disp_Char(row,col,' ');
    HAL_Delay(period);
    LCD_Disp_Custom_Char(row,col,index);
    HAL_Delay(period);
}

uint8_t heart_shape[] =         //the char '💖'
{
    0x00,  // 0b00000  -> 0x00
    0x0A,  // 0b01010  -> 0x0A
    0x1F,  // 0b11111  -> 0x1F
    0x1F,  // 0b11111  -> 0x1F

    0x0E,  // 0b01110  -> 0x0E
    0x04,  // 0b00100  -> 0x04
    0x00   // 0b00000  -> 0x00
};
