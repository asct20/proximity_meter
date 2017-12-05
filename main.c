/*
 * File:   main.c
 * Author: Alexandre
 *
 * Created on October 13, 2016, 5:29 PM
 */

#include <xc.h>
#include <stdint.h>

#include "const.h"
#include "dist_measure.h"

#ifdef UART_DEBUG
    #include "UART.h"
#endif

// TODO replace by simple array!
struct proximity_levels {
    uint8_t level_1;    // Closest
    uint8_t level_2;
    uint8_t level_3;
    uint8_t level_4;     // Furthest
};

// Mode used for sleep times
enum wdt_mode {
    mode_active,     // When an object is close
    mode_idle        // When no object has been close for THRESHOLD_COUNT_IDENTICAL
                     // or an object has been at the same distance for THRESHOLD_COUNT_IDENTICAL
};

void init();

/**
 * Update the LEDs depending on a distance
 * @param distance
 */
void update_proximity_meter(uint16_t distance);

/**
 * Put the MCU into sleep mode. Will temporarily set PORTA as an output and
 * revert it to its original state after we exit sleep mode.
 * @param mode indicate the sleep time
 */
void sleep_mode(enum wdt_mode mode);

void main(void) {
    init();
    
    uint8_t count_identical = 0;
    uint16_t distance = 0;
    uint16_t prev_distance = 0;
    enum wdt_mode wdt_mode = mode_active;
    
    PORTAbits.RA0 = 0;
    TRISAbits.TRISA0 = 0;
    
    while(1)
    {    
        if (PORTAbits.RA0 == 0) {
            PORTAbits.RA0 = 1;
        } else {
            PORTAbits.RA0 = 0;
        }
        // Get distance
        distance = HCCalculateDistance();
        if (distance > METER_LEVEL_1) {
            distance = METER_LEVEL_1 + 1;
        }
        
        wdt_mode = mode_active;
        if (distance != prev_distance) {
            update_proximity_meter(distance);
            prev_distance = distance;
            count_identical = 0;
        } else {
            if (count_identical < THRESHOLD_COUNT_IDENTICAL) {
                count_identical++;
            } else {
                update_proximity_meter(METER_LEVEL_1 + 1);  // All LEDs OFF
                wdt_mode = mode_idle;
            }
        }
        
        sleep_mode(wdt_mode);
    }
    return;
}

void init() {
    // Oscillator config
    OSCCONbits.SCS = 0b00;      // Oscillator mode defined by FOSC<2:0>
    OSCCONbits.IRCF = 0b111;    // Internal RC freq to 8 MHz
    OSCCONbits.IOFS = 1;        // Int freq is stable
    
    OPTION_REGbits.PSA = 1;     // Assign Prescaler to WDT
    OPTION_REGbits.PS = PS_ACTIVE;
    WDTCONbits.WDTPS = PS_WDT;
    
    ANSEL = 0x00; // Set all I/O to digital I/O
    
    PORTB = 0x00; // PortB (0:4) is the proximity meter
    TRISB = 0x00;
    
    PORTA = 0x00;
    TRISA = 0x00;

#ifdef UART_DEBUG
    UARTInit(9600, 1);
#endif
    
    // Init the distance measurer with 5 samples per measure
    HCInit(5);
}

//uint8_t get_current_level(uint16_t distance, const struct proximity_levels* levels) {
//    
//}

void update_proximity_meter(uint16_t distance) {
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB0 = 0;
    
    if (distance <= METER_LEVEL_4) {
        PORTBbits.RB3 = 1;
    }
    if (distance <= METER_LEVEL_3) {
        PORTBbits.RB4 = 1;
    }
    if (distance <= METER_LEVEL_2) {
        PORTBbits.RB1 = 1;
    }
    if (distance <= METER_LEVEL_1) {
        PORTBbits.RB0 = 1;
    }
}

void sleep_mode(enum wdt_mode mode) {
    // Set PORTA as an output to reduce current draw
    uint8_t prev_PORTA = PORTA;
    uint8_t prev_TRISA = TRISA;
    PORTA = 0x00;
    TRISA = 0x00;
    
    if (mode == mode_active) {
        OPTION_REGbits.PS = PS_ACTIVE;
    } else {
        OPTION_REGbits.PS = PS_IDLE;
    }
    
    WDTCONbits.SWDTEN = 1; // Enable WDT
    CLRWDT();
    SLEEP();
    WDTCONbits.SWDTEN = 0; // Disable WDT
    
    // Revert PORTA config to previous state
//    PORTA = prev_PORTA;
    TRISA = prev_TRISA;
}
