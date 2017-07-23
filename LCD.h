/* 
 * File:   LCD.h
 * Author: Alex Saucet
 *
 * Created on July 22, 2017, 5:29 PM
 * 
 * Library for using a LCD display based on a HD44780 controller, with a PIC
 * microcontroller. Change the pin names between the designated comments below
 * to the the pins used in your circuit.
 * 
 */

/***    Send command:   RS=0, EN=1
        Send data:      RS=1, EN=1 */

#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include <stdint.h>

/****** CHANGE VALUES BELOW ******/
// System clock frequency
#define _XTAL_FREQ  8000000

// Define the pins used
#define LCD_EN      PORTAbits.RA0
#define LCD_TRIS_EN TRISAbits.TRISA0
#define LCD_RS      PORTAbits.RA1
#define LCD_TRIS_RS TRISAbits.TRISA1

#define LCD_D0  PORTBbits.RB0
#define LCD_D1  PORTBbits.RB1
#define LCD_D2  PORTBbits.RB2
#define LCD_D3  PORTBbits.RB3
#define LCD_D4  PORTBbits.RB4
#define LCD_D5  PORTBbits.RB5
#define LCD_D6  PORTBbits.RB6
#define LCD_D7  PORTBbits.RB7

#define LCD_TRIS0  TRISBbits.TRISB0
#define LCD_TRIS1  TRISBbits.TRISB1
#define LCD_TRIS2  TRISBbits.TRISB2
#define LCD_TRIS3  TRISBbits.TRISB3
#define LCD_TRIS4  TRISBbits.TRISB4
#define LCD_TRIS5  TRISBbits.TRISB5
#define LCD_TRIS6  TRISBbits.TRISB6
#define LCD_TRIS7  TRISBbits.TRISB7

/****** DO NOT CHANGE VALUES BELOW ******/
#define LCD_MODE_4_BIT  0x28    // 4 bit data bus
#define LCD_MODE_8_BIT  0x38    // 8 bit data bus
#define LCD_CLEAR       0x01
#define LCD_ON          0x0C    // Display ON, cursor and blink OFF
#define LCD_HOME        0x02    // Moves cursor to line 0, column 0

/**
 * Initialize the LCD display and the pins used
 * @param _mode bus mode (either LCD_MODE_4_BIT or LCD_MODE_8_BIT)
 */
void LCDInit(uint8_t mode);

/**
 * Clear the LCD and move cursor to origin
 */
void LCDClear();

/**
 * Send a high pulse on EN pin
 */
void LCDPulseEnable();

/**
 * Set values for pins of the data bus
 * @param value
 */
void LCDSetDataBus(uint8_t value);

/**
 * Send a command to the LCD display
 * @param cmd
 */
void LCDCommand(uint8_t cmd);

/**
 * Write a character on the display
 * @param data character to display
 */
void LCDWriteChar(unsigned char data);

/**
 * Write a string on the display
 * @param str string to write
 */
void LCDWriteString(unsigned char* str);

/**
 * Move the cursor to the selected position
 * @param line starting at 0
 * @param column starting at 0
 */
void LCDMoveCursor(uint8_t line, uint8_t column);

#endif	/* LCD_H */


