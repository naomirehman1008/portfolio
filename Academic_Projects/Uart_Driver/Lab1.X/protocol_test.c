/* 
 * File:   protocol_test.c
 * Author: naomi
 *
 * Created on February 27, 2023, 8:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LEDs.h"
#include "Protocol2.h"
#include "uart.h"
#include "LEDs.h"
#include "MessageIDs.h"

/*
 * 
 */
#define BAUDRATE 115200
#define READ_NEXT 0xFF
#define PROTOCOL_TEST_MACRO

#ifdef PROTOCOL_TEST_MACRO
//fix this horrible prepocessor bs
int Packet_IDStateMachine(void);
char message[] = "uart is working";

char leds_get[] = "state machine in ID_LEDS_GET";
char leds_set[] = "state machine in ID_LEDS_SET";
char ping[] = "state machine in ID_PING";

int main(int argc, char** argv) {

    
    time_t curr_time;
    char * time_string;
    
    time(&curr_time);
    time_string = ctime(&curr_time); //thinks its 1970??
    
    Protocol_Init(BAUDRATE);
    LEDS_Init();
    Protocol_SendDebugMessage(time_string); //print the run time to the console
    while(1){
        //Protocol_SendDebugMessage(message);
        Protocol_QueuePacket();
        //PutChar(Protocol_ReadNextPacketID());
        Packet_IDStateMachine();
    }
    return (EXIT_SUCCESS);
}
#endif /*protocol*/

int Packet_IDStateMachine(void){
    unsigned char ID = READ_NEXT;
    
    unsigned char type;
    unsigned char len;
    unsigned char * msg = malloc(MAXPAYLOADLENGTH); //message pointer never initialized ->crash
                            //need to malloc, but how much??
                            //PLS REMEMBER TO FREE!!!
//    char unused_string[5];
    char * payload;
    unsigned int garbage;
    unsigned char leds_state;
    //REMEMBER TO CONVERT ENDIANNESS
    int boolean = 1;
    while(boolean){
        switch(ID){
            case READ_NEXT:
                ID = Protocol_ReadNextPacketID();
                Protocol_GetInPacket(&type, &len, msg);
                //Protocol_SendPacket(len, type, msg);
            break;
            case ID_LEDS_SET:
                //Protocol_SendDebugMessage(leds_set);
                leds_state = msg[0];
                LEDS_Write(leds_state); //everything has pointer problems
                free(msg);
                boolean = 0;
            break;
            case ID_LEDS_GET:
                *payload = LEDS_Read(); //this line crashes the program??
                //ADD STUFF FOR A BINARY REPRESENTATION HERE (probably something to do w powers of two)
                Protocol_SendPacket(sizeof(payload), ID_LEDS_STATE, payload);
                //Protocol_SendDebugMessage(leds_get);
                free(msg);
                boolean = 0;

            break;
            case ID_PING:
                garbage = convertEndian((unsigned int*)msg); //program crashes here!!! memory issue
                garbage = garbage >> 1;
                garbage = convertEndian(&garbage); //needs to be an unsigned int!!!!!
                Protocol_SendPacket(sizeof(garbage), ID_PONG, &garbage);
                //Protocol_SendDebugMessage(ping);
                free(msg);
                boolean = 0;
            break;
            default:
                ID = READ_NEXT;
            break;
        }
    }
}


