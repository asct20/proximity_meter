/* 
 * File:   LCD.c
 * Author: Alex Saucet
 *
 * Created on July 22, 2017, 5:29 PM
 */

#include "LCD.h"

static uint8_t mode;

/**
 * Initialize the LCD display and the pins used
 * @param _mode bus mode (either LCD_MODE_4_BIT or LCD_MODE_8_BIT)
 */
void LCDInit(uint8_t _mode)
{
    mode = _mode;
    
    // Init pins to 0:
    LCD_EN = 0;
    LCD_RS = 0;
    LCD_D7 = 0;
    LCD_D6 = 0;
    LCD_D5 = 0;
    LCD_D4 = 0;
    if (mode == LCD_MODE_8_BIT) {
        LCD_D3 = 0;
        LCD_D2 = 0;
        LCD_D1 = 0;
        LCD_D0 = 0;
    }
    
    // Set pins as output:
    LCD_TRIS_EN = 0;
    LCD_TRIS_RS = 0;
    
    LCD_TRIS7 = 0;
    LCD_TRIS6 = 0;
    LCD_TRIS5 = 0;
    LCD_TRIS4 = 0;
    if (mode == LCD_MODE_8_BIT) {
        LCD_TRIS3 = 0;
        LCD_TRIS2 = 0;
        LCD_TRIS1 = 0;
        LCD_TRIS0 = 0;
    }
    
    // Init LCD:
    if (mode == LCD_MODE_4_BIT) {
        LCDCommand(0x02);
        LCDCommand(LCD_MODE_4_BIT);
    } else {
        LCDCommand(LCD_MODE_8_BIT);
    }
    LCDCommand(LCD_CLEAR);
    LCDCommand(LCD_ON);
    LCDCommand(LCD_HOME);
}

/**
 * Clear the LCD and move cursor to origin
 */
void LCDClear()
{
    // Send clear command:
    LCDCommand(LCD_CLEAR);
    LCDCommand(LCD_HOME);
}

/**
 * Send a high pulse on EN pin
 */
void LCDPulseEnable() {
    LCD_EN  = 1;    
    __delay_ms(5);
    LCD_EN  = 0;
}

/**
 * Set values for pins of the data bus
 * @param value
 */
void LCDSetDataBus(uint8_t value) {
    if (value & 0x80) LCD_D7 = 1; else LCD_D7 = 0;
    if (value & 0x40) LCD_D6 = 1; else LCD_D6 = 0;
    if (value & 0x20) LCD_D5 = 1; else LCD_D5 = 0;
    if (value & 0x10) LCD_D4 = 1; else LCD_D4 = 0;
    if (mode == LCD_MODE_8_BIT) {
        if (value & 0x08) LCD_D3 = 1; else LCD_D3 = 0;
        if (value & 0x04) LCD_D2 = 1; else LCD_D2 = 0;
        if (value & 0x02) LCD_D1 = 1; else LCD_D1 = 0;
        if (value & 0x01) LCD_D0 = 1; else LCD_D0 = 0;
    }
}

/**
 * Send a command to the LCD display
 * @param cmd
 */
void LCDCommand(uint8_t cmd)
{
    LCDSetDataBus(cmd & 0xf0);
    LCD_RS  = 0;    // Cmd mode
    LCDPulseEnable();
    
    LCDSetDataBus((cmd & 0x0f) << 4);
    LCDPulseEnable();
}

/**
 * Write a character on the display
 * @param data character to display
 */
void LCDWriteChar(unsigned char data)
{
    LCDSetDataBus(data & 0xf0);
    LCD_RS  = 1;    // data mode
    LCDPulseEnable();
    
    LCDSetDataBus((data & 0x0f) << 4);
    LCDPulseEnable();
}

/**
 * Write a string on the display
 * @param str string to write
 */
void LCDWriteString(unsigned char* str)
{
    while(*str != '\0') // Print characters until end of line
    {
        LCDWriteChar(*str);
        str++;
    }
}

/**
 * Move the cursor to the selected position
 * @param line starting at 0
 * @param column starting at 0
 */
void LCDMoveCursor(uint8_t line, uint8_t column)
{
    uint8_t AC  = 0x80;
    if(line > 1 || column > 15)
    {
        return;
    }
    AC += line*0x40;
    AC += column;
    LCDCommand(AC);
}
