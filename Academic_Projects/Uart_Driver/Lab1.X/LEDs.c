//Naomi Rehman
//ECE121 Spring 2023
//Lab0

#include <xc.h>

void LEDS_Init(void){
    TRISE = 0x00; //set to output
    LATE = 0x00; //ADD REGISTER MACROS
}

unsigned char LEDS_Read(void){
    return LATE;
}

void LEDS_Write(unsigned char LED_PATTERN){
    LATE = LED_PATTERN;
}


