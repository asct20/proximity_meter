#include <xc.h>
#include <stdint.h>
#include <stdlib.h>

#include "const.h"
#include "dist_measure.h"

#ifdef UART_DEBUG
    #include "UART.h"
#endif

#define _XTAL_FREQ  8000000     // System clock frequency
#define MAX_LENGTH_UART 16      // Max lenght of a string send or received with UART

static uint8_t n_samples;

/**
 * Initialize timer1 and pins of HC-SR04
 * @param number of samples for the filter
 */
void HCInit(uint8_t _n_samples) {
    // Timer1 config
    T1CONbits.TMR1CS = 0;   // Clock source is internal clock (Fosc/4)
    T1CONbits.TMR1ON = 0;   // Timer off
    T1CONbits.T1CKPS = 0b01;// Prescaler 2:1
    T1CONbits.T1OSCEN = 0;  // Disable timer1 oscillator
    
    // HC-SR04 pins
    HCSR04_TRIG = 0;
    HCSR04_TRIG_TRIS = 0;
    HCSR04_ECHO_TRIS = 1;
    
    // Save n_samples
    if (_n_samples > 5) {
        n_samples = 5;
    } else {
        n_samples = _n_samples;
    }
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
    
#ifdef UART_DEBUG
    char* tmpBuf[16];
    itoa(tmpBuf, (uint8_t)time, 10);
    UARTSendString("\n\rTime = \0", MAX_LENGTH_UART);
    UARTSendString(tmpBuf, MAX_LENGTH_UART);
    UARTSendString("\n\rOver\0", MAX_LENGTH_UART);
#endif
    
    return time;
}

/**
 * Calculate the distance to object
 * @return distance in cm
 */
uint16_t HCCalculateDistance() {
    uint16_t time = 0;
    uint16_t distance = 0;
    
    uint8_t i = 0;
    for (i=0 ; i<n_samples ; i++) {
        time = HCCalculateTime();
        distance += time / 58.77;
        __delay_ms(50);
    }
    
    distance /= n_samples;
    
#ifdef UART_DEBUG
    char* tmpBuf[16];
    itoa(tmpBuf, (uint8_t)distance, 10);
    UARTSendString("\n\rDistance (cm) = \0", MAX_LENGTH_UART);
    UARTSendString(tmpBuf, MAX_LENGTH_UART);
    UARTSendString("\n\rOver\0", MAX_LENGTH_UART);
#endif
    
    return distance;
}



