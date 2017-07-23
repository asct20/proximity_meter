#include <xc.h>
#include <stdint.h>

#include "dist_measure.h"

/*Time = (TMR1H:TMR1L) / 1 000 000

d = s / t
s = 34 000 cm/s

1 sec -- 34000cm
t sec -- d

d = (34000*t)/2
t is Time above;
 */

#define _XTAL_FREQ  8000000     // System clock frequency

/**
 * Initialize timer1 and pins of HC-SR04
 */
void HCInit() {
    // Timer1 config
    T1CONbits.TMR1CS = 0;   // Clock source is internal clock (Fosc/4)
    T1CONbits.TMR1ON = 0;   // Timer off
    T1CONbits.T1CKPS = 0b01;// Prescaler 2:1
    T1CONbits.T1OSCEN = 0;  // Disable timer1 oscillator
    
    // HC-SR04 pins
    HCSR04_TRIG = 0;
    HCSR04_TRIG_TRIS = 0;
    HCSR04_ECHO_TRIS = 1;
}

/**
 * Calculate the time taken for signal to get to obstacle and come back
 * @return time, equal 2 times the distance to object
 */
uint16_t HCCalculateTime() {
    uint16_t time = 0;
    
    // Reset timer
    TMR1L = 0;
    TMR1H = 0;

    // Trigger high/low
    HCSR04_TRIG = 1;
    __delay_us(20);
    HCSR04_TRIG = 0;

    // Wait for ECHO to be high
    while (!HCSR04_ECHO);
    T1CONbits.TMR1ON = 1;
    // Wait for ECHO to be low again
    while (HCSR04_ECHO);
    T1CONbits.TMR1ON = 0;

    // Read timer value
    time = (TMR1H << 8) | TMR1L;
    
    return time;
}

/**
 * Calculate the distance to object
 * @return distance in cm
 */
uint16_t HCCalculateDistance() {
    uint16_t time = HCCalculateTime();
    uint16_t distance = time / 58.77;
    return distance;
}



