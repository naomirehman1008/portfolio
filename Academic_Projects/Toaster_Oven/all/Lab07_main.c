// **** Include libraries here ****
// Standard libraries
#include <stdio.h>

//CSE13E Support Library
#include "BOARD.h"
#include "Ascii.h"
#include "Oled.h"
#include "OledDriver.h"
#include "Adc.h"
#include "Buttons.h"
#include "Leds.h"

// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>



// **** Set any macros or preprocessor directives here ****
// Set a macro for resetting the timer, makes the code a little clearer.
#define TIMER_2HZ_RESET() (TMR1 = 0)
#define COOKING_TIME(time) ((time >> 2) + 1)
#define MINUTES(time) ((time / 60))
#define SECONDS(time) ((time % 60))
#define OVEN_TEMP(tempurature) ((tempurature >> 2) + MIN_TEMP)
#define LED_INTERVAL (oven.init_time / 8)

#define MAX_TEMP 555
#define MIN_TEMP 300

#define BAKE "Bake "
#define TOAST "Toast"
#define BROIL "Broil"

#define SELECTOR_CHAR ">"
#define BLANK " "

#define SETUP_MODE BAKE
#define SETUP_INIT_TIME 1
#define SETUP_TEMP 350
#define SETUP_BROIL_TEMP 500

#define LONG_PRESS 5
#define TICK 5



// **** Set any local typedefs here ****

typedef enum {
    SETUP, SELECTOR_CHANGE_PENDING, COOKING, RESET_PENDING
} OvenState;

typedef enum {
    LED8 = 0xFF,
    LED7 = 0xFE,
    LED6 = 0xFC,
    LED5 = 0xF8,
    LED4 = 0xF0,
    LED3 = 0xE0,
    LED2 = 0xC0,
    LED1 = 0x80,
    LED0 = 0x00
} LedChars;

typedef struct {
    OvenState state;
    int time_left;
    int init_time;
    uint16_t button_event_time;
    uint16_t button_total_time;
    char* mode;
    uint16_t temp;
    char* time_selector;
    char* temp_selector;
    uint8_t adc_event;



    //add more members to this struct
} OvenData;

typedef struct {
    uint8_t event;
    uint8_t button1up;
    uint8_t button1down;
    uint8_t button2up;
    uint8_t button2down;
    uint8_t button3up;
    uint8_t button3down;
    uint8_t button4up;
    uint8_t button4down;

} Button;

typedef struct {
    uint8_t TIMER_TICK; //event every one second
    int16_t start_time; //
    int16_t current_time; //running time in interrupt ticks
    int16_t time_ellapsed; //current - start;

} Timer;

// **** Declare any datatypes here ****

// **** Define any module-level, global, or external variables here ****
static char dest_str0[80];
//static char dest_str1[30];
//static char dest_str2[30];
//static char dest_str3[30];
static OvenData oven;
static Button button;
static Timer timer;


// **** Put any helper functions here ****

/*This function will update your OLED to reflect the state .*/
void updateOvenOLED(OvenData ovenData) {
    //off
    switch (ovenData.state) {
        case SETUP:
            if (oven.mode == BAKE) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------|%sTime: %d:%02d\n|------|%sTemp: %u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        oven.temp_selector,
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);
            } else if (oven.mode == TOAST) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|      |%sTime: %d:%02d\n|------|          \n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        //oven.temp_selector,
                        //ovenData.temp,
                        //DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);
            } else if (oven.mode == BROIL) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------|%sTime: %d:%02d\n|      |%sTemp: %u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        oven.temp_selector,
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);

            }

            break;



            //sprintf(dest_str1, "\n|      |   Time:");
            //sprintf(dest_str2, "\n|      |   Temp:");
            //sprintf(dest_str3, "\n|%s%s%s%s%s%s|", OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF, OVEN_BOTTOM_OFF);


            //on
        case SELECTOR_CHANGE_PENDING://don't know what these do??
            if (oven.mode == BAKE) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------|%sTime: %d:%02d\n|------|%sTemp: %u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        oven.temp_selector,
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);
            } else if (oven.mode == TOAST) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|      |%sTime: %d:%02d\n|------|           \n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        //oven.temp_selector,
                        //ovenData.temp,
                        //DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);
            } else if (oven.mode == BROIL) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------|%sTime: %d:%02d\n|      |%sTemp: %u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        OVEN_TOP_OFF,
                        ovenData.mode,
                        oven.time_selector,
                        MINUTES(ovenData.init_time),
                        SECONDS(ovenData.init_time),
                        oven.temp_selector,
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF,
                        OVEN_BOTTOM_OFF);

            }

            break;
        case RESET_PENDING://don't know what these do??
            break;

        case COOKING:
            if (oven.mode == BAKE) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------| Time: %d:%02d\n|------| Temp: %3u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        ovenData.mode,
                        MINUTES(ovenData.time_left),
                        SECONDS(ovenData.time_left),
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON);
            } else if (oven.mode == TOAST) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|      | Time: %d:%02d \n|------|           \n|%s%s%s%s%s%s|",
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        ovenData.mode,
                        MINUTES(ovenData.time_left),
                        SECONDS(ovenData.time_left),
                        //ovenData.temp,
                        //DEGREE_SYMBOL,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON);
            } else if (oven.mode == BROIL) {
                sprintf(dest_str0, "|%s%s%s%s%s%s| Mode: %s\n|------| Time: %d:%02d\n|      | Temp: %3u%s\n|%s%s%s%s%s%s|",
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        OVEN_TOP_ON,
                        ovenData.mode,
                        MINUTES(ovenData.time_left),
                        SECONDS(ovenData.time_left),
                        ovenData.temp,
                        DEGREE_SYMBOL,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON,
                        OVEN_BOTTOM_ON);

            }
            break;
    }

    OledDrawString(dest_str0);
    OledUpdate();
    //sprintf(dest_str0, "|%s%s%s%s%s%s|" ,  OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON, OVEN_TOP_ON);

    //update OLED here
}

/*This function will execute your state machine.  
 * It should ONLY run if an event flag has been set.*/
void runOvenSM(void) {
    //SETUP
    switch (oven.state) {
        case SETUP:
            updateOvenOLED(oven);
            if (oven.mode == BAKE) {
                if (oven.time_selector == SELECTOR_CHAR) {
                    if (oven.adc_event) {
                        oven.adc_event = FALSE;
                        oven.init_time = COOKING_TIME(AdcRead());
                        updateOvenOLED(oven);
                    }
                } else if (oven.temp_selector == SELECTOR_CHAR) {
                    if (oven.adc_event) {
                        oven.adc_event = FALSE;
                        oven.temp = OVEN_TEMP(AdcRead());
                        updateOvenOLED(oven);
                    }
                }
            } else if (oven.mode == TOAST) {
                if (oven.adc_event) {
                    oven.adc_event = FALSE;
                    oven.init_time = COOKING_TIME(AdcRead());
                    updateOvenOLED(oven);
                }

            } else if (oven.mode == BROIL) {
                oven.temp = SETUP_BROIL_TEMP;
                updateOvenOLED(oven);
                if (oven.adc_event) {
                    oven.adc_event = FALSE;
                    oven.init_time = COOKING_TIME(AdcRead());
                    updateOvenOLED(oven);
                }

            }
            if (button.event) {
                button.event = FALSE;
                if (button.button3down) {
                    oven.button_event_time = timer.current_time;
                    oven.state = SELECTOR_CHANGE_PENDING;
                }
                if (button.button4down) {
                    oven.time_left = oven.init_time;
                    oven.state = COOKING;
                }
            }
            break;

        case SELECTOR_CHANGE_PENDING:
            if (button.event) {
                if (button.button3up) {
                    button.event = FALSE;
                    oven.button_total_time = timer.current_time - oven.button_event_time;
                    if (oven.button_total_time < LONG_PRESS) {
                        if (oven.mode == BAKE) {
                            oven.time_selector = BLANK;
                            oven.temp_selector = BLANK;
                            oven.mode = TOAST;
                        } else if (oven.mode == TOAST) {
                            oven.time_selector = BLANK;
                            oven.temp_selector = BLANK;
                            oven.mode = BROIL;
                        } else if (oven.mode == BROIL) {
                            oven.time_selector = SELECTOR_CHAR;
                            oven.temp_selector = BLANK;
                            oven.mode = BAKE;
                        }
                        updateOvenOLED(oven);
                        oven.state = SETUP;
                    } else {
                        if (oven.mode == BAKE) {
                            if (oven.time_selector == SELECTOR_CHAR) {
                                oven.time_selector = BLANK;
                                oven.temp_selector = SELECTOR_CHAR;
                                updateOvenOLED(oven);
                            } else if (oven.time_selector == BLANK) {
                                oven.time_selector = SELECTOR_CHAR;
                                oven.temp_selector = BLANK;
                                updateOvenOLED(oven);
                            } else {

                            }
                        }
                        oven.state = SETUP;
                    }

                }
            }
            break;

        case COOKING:
            //LEDS_SET(LED8);

            updateOvenOLED(oven);

            if (button.event) {
                button.event = FALSE;
                if (button.button4down) {
                    oven.button_event_time = timer.current_time;
                    oven.state = RESET_PENDING;
                }
            }
            if (timer.TIMER_TICK) {
                timer.TIMER_TICK = FALSE;
                oven.time_left -= 1;
            }
            if (oven.time_left > 0) {
                //LEDS
                //OLED
                if (oven.time_left > LED_INTERVAL * 7) {
                    LEDS_SET(LED8);
                } else if (oven.time_left > LED_INTERVAL * 6) {
                    LEDS_SET(LED7);
                } else if (oven.time_left > LED_INTERVAL * 5) {
                    LEDS_SET(LED6);
                } else if (oven.time_left > LED_INTERVAL * 4) {
                    LEDS_SET(LED5);
                } else if (oven.time_left > LED_INTERVAL * 3) {
                    LEDS_SET(LED4);
                } else if (oven.time_left > LED_INTERVAL * 2) {
                    LEDS_SET(LED3);
                } else if (oven.time_left > LED_INTERVAL * 1) {
                    LEDS_SET(LED2);
                } else {
                    LEDS_SET(LED1);
                }

            } else {
                LEDS_SET(LED0);
                oven.time_left = 0;
                oven.state = SETUP;
            }


            break;

        case RESET_PENDING:
            oven.button_total_time = timer.current_time - oven.button_event_time;
            if (oven.button_total_time >= LONG_PRESS) {
                LEDS_SET(LED0);
                oven.init_time = SETUP_INIT_TIME;
                oven.time_left = 0;
                oven.mode = SETUP_MODE;
                oven.temp = SETUP_TEMP;
                oven.time_selector = SELECTOR_CHAR;
                oven.temp_selector = BLANK;
                oven.state = SETUP;
            } else {
                if (button.event) {
                    button.event = FALSE;
                    if (button.button4up) {
                        oven.state = COOKING;
                    }
                }
            }
            break;

        default:
            break;






    }
    //write your SM logic here.
}

int main() {
    BOARD_Init();
    OledInit();
    ButtonsInit();
    AdcInit();
    LEDS_INIT();
    //initalize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.

    T2CON = 0; // everything should be off
    T2CONbits.TCKPS = 0b100; // 1:16 prescaler
    PR2 = BOARD_GetPBClock() / 16 / 100; // interrupt at .5s intervals
    T2CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T2IF = 0; //clear the interrupt flag before configuring
    IPC2bits.T2IP = 4; // priority of  4
    IPC2bits.T2IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T2IE = 1; // turn the interrupt on

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescaler, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.

    T3CON = 0; // everything should be off
    T3CONbits.TCKPS = 0b111; // 1:256 prescaler
    PR3 = BOARD_GetPBClock() / 256 / 5; // interrupt at .5s intervals
    T3CONbits.ON = 1; // turn the timer on

    // Set up the timer interrupt with a priority of 4.
    IFS0bits.T3IF = 0; //clear the interrupt flag before configuring
    IPC3bits.T3IP = 4; // priority of  4
    IPC3bits.T3IS = 0; // subpriority of 0 arbitrarily 
    IEC0bits.T3IE = 1; // turn the interrupt on;

    // </editor-fold>

    printf("Welcome to CRUZID's Lab07 (Toaster Oven).  Compiled on %s %s.", __TIME__, __DATE__);

    //initialize state machine (and anything else you need to init) here


    oven.state = SETUP;
    oven.init_time = SETUP_INIT_TIME;
    oven.time_left = 0;
    oven.mode = SETUP_MODE;
    oven.temp = SETUP_TEMP;
    oven.time_selector = SELECTOR_CHAR;
    oven.temp_selector = BLANK;

    timer.TIMER_TICK = FALSE;
    timer.current_time = 0;
    timer.time_ellapsed = 0;

    updateOvenOLED(oven);
    while (1) {
        // Add main loop code here:
        // check for events
        // on event, run runOvenSM()
        // clear event flags

        if (button.event || oven.adc_event) {
            runOvenSM();
        }
        //oven.temp = AdcRead();

    };
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void) {

    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    //LEDS_SET(0x01);
    timer.current_time += 1;

    timer.time_ellapsed = timer.current_time - timer.start_time;

    if (timer.time_ellapsed % TICK == 0) {
        timer.TIMER_TICK = TRUE;
    }

    //add event-checking code here
}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void) {
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;

    //add event-checking code here
    uint8_t ButtonEvent = ButtonsCheckEvents();
    if (ButtonEvent) { //move somewhere else??
        button.event = TRUE;
        button.button1up = ButtonEvent & BUTTON_EVENT_1UP;
        button.button1down = ButtonEvent & BUTTON_EVENT_1DOWN;
        button.button2up = ButtonEvent & BUTTON_EVENT_2UP;
        button.button2down = ButtonEvent & BUTTON_EVENT_2DOWN;
        button.button3up = ButtonEvent & BUTTON_EVENT_3UP;
        button.button3down = ButtonEvent & BUTTON_EVENT_3DOWN;
        button.button4up = ButtonEvent & BUTTON_EVENT_4UP;
        button.button4down = ButtonEvent & BUTTON_EVENT_4DOWN;
    }
    if (AdcRead()) {
        oven.adc_event = TRUE;
    }

}