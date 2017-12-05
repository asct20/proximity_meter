/* 
 * File:   const.h
 * Author: Alex Saucet
 *
 * Created on December 4, 2017, 5:44 PM
 */

#ifndef CONST_H
#define	CONST_H

// Comment to disable UART debugging
#define UART_DEBUG

// CONFIG1
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has PGM function, Low-Voltage Programming enabled)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB0      // CCP1 Pin Selection bit (CCP1 function on RB0)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// CONFIG2
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode disabled)

#define _XTAL_FREQ  8000000     // System clock frequency

/* 
 * Values for the WDT clock source
 * Input frequency is 31.25KHz
 */
#define PS_WDT      0b1001  // WDT prescaler - 1:16394 => 524ms
#define PS_IDLE     0b011   // TMR0 clock source prescaler - 1:8 => 4.2s
#define PS_ACTIVE   0b001   // TMR0 clock source prescaler - 1:2 => 1s

// If distance is the same for this many times, go to idle mode
#define THRESHOLD_COUNT_IDENTICAL   20

// Proximity levels (higher level = object is closer)
#define METER_LEVEL_1   75
#define METER_LEVEL_2   65
#define METER_LEVEL_3   55
#define METER_LEVEL_4   40

#endif	/* CONSTANTS_H */


