/* 
 * File:   dist_measure.h
 * Author: Alex Saucet
 *
 * Created on July 23, 2017, 2:21 PM
 */

#ifndef DIST_MEASURE_H
#define	DIST_MEASURE_H

/*** CHANGE PINS BELOW ***/
#define HCSR04_TRIG         PORTAbits.RA6
#define HCSR04_TRIG_TRIS    TRISAbits.TRISA6
#define HCSR04_ECHO         PORTAbits.RA7
#define HCSR04_ECHO_TRIS    TRISAbits.TRISA7
/*** STOP CHANGES ***/


/**
 * Initialize timer1 and pins of HC-SR04
 */
void HCInit(uint8_t _n_samples);

/**
 * Calculate the time taken for signal to get to obstacle and come back
 * @return time, equal 2 times the distance to object
 */
uint16_t HCCalculateTime();

/**
 * Calculate the distance to object
 * @return distance in cm
 */
uint16_t HCCalculateDistance();

#endif	/* DIST_MEASURE_H */

