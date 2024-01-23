// Naomi Rehman 2024
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "apc_algorithm.h"


#define TESTING

uint32_t timesCovered[2] = {0,0};
uint32_t timesCleared[2] = {0,0};

uint8_t coverEvent = 0;
uint8_t coverEvent2 = 0;
uint8_t clearEvent = 0;
uint8_t clearEvent2 = 0;
uint8_t clear = 0;
uint8_t clear2 = 0;

unsigned int counter = 0;

//sensor 1 -> sensor_num = 1
//sensor 2 -> sensor_num = 2
int APC_UpdateState(uint16_t distance, uint16_t dist_threshold, uint8_t sensor_num){
	if (sensor_num == 1){
		if(distance >= dist_threshold){
            clear = 1;
            if(coverEvent && !clearEvent){
                timesCleared[0] = counter + 1;
                clearEvent = 1;
            }
        }
        else{
            clear = 0;
        }
        //something in front of sensor
        if((distance < dist_threshold)){
            if(!coverEvent){
                timesCovered[0] = counter + 1;
                coverEvent = 1;
            }
            clearEvent = 0;
        }

	}
	else if (sensor_num == 2){
		if(distance >= dist_threshold){
            clear2 = 1;
            if(coverEvent2 && !clearEvent2){
                timesCleared[1] = counter;
                clearEvent2 = 1;
            }
        }
        else{
            clear2 = 0;
        }
        //something blocking sensor
        if((distance < dist_threshold)){
            if(!coverEvent2){
                timesCovered[1] = counter;
                coverEvent2 = 1;
            }
            clearEvent2 = 0;
        }

	}
	else{
		printf ("Error in APC_UpdateState: invalid value for sensor_num\n");
		return 0;
	}
	return 1;
}

BoardingEvents APC_EvalState(){
	BoardingEvents cur_event = no_event;
	if(clear && clear2){
        //if both sensors have been covered and cleared
        if(clearEvent && clearEvent2){
            //if sensor 0 cleared after sensor 1
            if((timesCovered[0] > timesCovered[1] && timesCleared[0] > timesCleared[1]) ||
                (timesCovered[0] == timesCovered[1] && timesCleared[0] > timesCleared[1]) ||
                (timesCovered[0] > timesCovered[1] && timesCleared[0] == timesCleared[1])){
                //boarding
                #ifdef TESTING
                printf("Boarding!\n");
                printf("++++++++++++\n");
                #endif
                cur_event = boarding;
            }
            //if sensor 1 cleared after sensor 0
            else if((timesCovered[1] > timesCovered[0] && timesCleared[1] > timesCleared[0]) ||
                (timesCovered[1] == timesCovered[0] && timesCleared[1] > timesCleared[0]) ||
                (timesCovered[1] > timesCovered[0] && timesCleared[1] == timesCleared[0])){
                //deboarding
                #ifdef TESTING
                printf("Deboarding!\n");
                printf("///////////////\n");
                #endif
                cur_event = deboarding;
            }
            //invalid crossing
            else{
            	#ifdef TESTING
                printf("Oh no! Invalid crossing\n");
                #endif
                cur_event = invalid;
            }
            #ifdef TESTING
            printf("covered: %hhu, %hhu\n", coverEvent, coverEvent2);
            printf("cleared: %hhu, %hhu\n", clear, clear2);
            printf("times covered: %lu, %lu\n", timesCovered[0], timesCovered[1]);
            printf("times cleared: %lu, %lu\n", timesCleared[0], timesCleared[1]); 
            #endif
        }
        //clear events
        coverEvent = 0;
        coverEvent2 = 0;
        clearEvent = 0;
        clearEvent2 = 0;
    }
    counter++;
    return cur_event;
}