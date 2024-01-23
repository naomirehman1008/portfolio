/* 
 * File:   LEDs.h
 * Author: Naomi Rehman
 *
 * Created on January 17, 2023, 8:07 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#ifdef	__cplusplus
extern "C" {
#endif
    

//sets LED port to output
//sets LEDs to 0x00 (off)    
void LEDS_Init(void);

//returns current LED value
unsigned char LEDS_Read(void);

//writes LED_PATTERN to the LEDs
void LEDS_Write(unsigned char LED_PATTERN);




#ifdef	__cplusplus
}
#endif

#endif	/* LEDS_H */

