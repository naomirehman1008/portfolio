
#include "Protocol2.h"
#include "uart.h"
#include <xc.h>
#include <sys/attribs.h>
#include "BOARD.h"
#include <stdint.h>
#include "MessageIDs.h"

//THE BUGS ARE: ISSUE GETTING THE CORRECT SIZE WHEN PASSING THE MESSAGE INTO SEND PACKAGE
//DATA IN MESSAGE POINTER IS UNINITIALIZED (ISSUE IN getInPacket)(probably) CAUSING CRASHES WHEN
//I TRY TO ACCESS THE DATA

/**
 * @Function Protocol_Init(baudrate)
 * @param Legal Uart baudrate
 * @return SUCCESS (true) or ERROR (false)
 * @brief Initializes Uart1 for stream I/O to the lab PC via a USB 
 *        virtual comm port. Baudrate must be a legal value. 
 * @author instructor W2022 */
//POSSIBLE ISSUES:
//len of payload includes the message ID!!
//MAKE SURE THAT PAYLOADS DO NOT INCLUDE ID MESSAGE;


Packet_Buffer packets;

int Protocol_Init(unsigned long baudrate){
    Uart_Init(baudrate);
    packets.head = 0;
    packets.tail = 0;
    packets.buffer_size = PACKETBUFFERSIZE;
    packets.is_empty = TRUE;
    packets.is_full = FALSE;
    return 1;
}

/**
 * @Function unsigned char Protocol_QueuePacket()
 * @param none
 * @return the buffer full flag: 1 if full
 * @brief Place in the main event loop (or in a timer) to continually check 
 *        for completed incoming packets and then queue them into 
 *        the RX circular buffer. The buffer's size is set by constant
 *        PACKETBUFFERSIZE.
 * @author instructor W2023 */
uint8_t Protocol_QueuePacket (){
    unsigned char reset = 0; //maybe add a loop
    if(packets.is_full){
        return 1;            //fix pointer stuff
    }
    else{
        packets.packet_arr[packets.head] = (rxpADT)malloc(sizeof(struct rxpT));
        BuildRxPacket(packets.packet_arr[packets.head], reset); //this is problematic bc if 
                                                                //it gets a character that isnt head it just finishes
                                                                //trying to access uninitialized memory -> segfault -> crash
        packets.head = (packets.head + 1) % packets.buffer_size; 
        if(packets.head == packets.tail){
            packets.is_full = TRUE;
        }
        packets.is_empty = FALSE;
        return 0;
    }
}

/**
 * @Function int Protocol_GetInPacket(uint8_t *type, uint8_t *len, uchar *msg)
 * @param *type, *len, *msg
 * @return SUCCESS (true) or WAITING (false)
 * @brief Reads the next packet from the packet Buffer 
 * @author instructor W2022 */
int Protocol_GetInPacket(uint8_t *type, uint8_t *len, unsigned char *msg){ //write to these pointers
    if(packets.is_empty){
        return 0;
    }
    else{
        rxpADT temp_packet = packets.packet_arr[packets.tail]; //gets the address of the packet
        int i;
        int range;
        *type = temp_packet->ID;
        *len = temp_packet->len;
        
        range = (temp_packet->len) / sizeof(temp_packet->payLoad[0]);
        //msg = temp_packet->payLoad; //changing the address of msg has no meaning outside of the function
                                    //this may require a loop
        //this chunk of code right here is the problem
        for(i=0;i<range;i++){
            *(msg + i) = temp_packet->payLoad[i]; //this looks illegal
                                                  //crashing right here
        }
        packets.tail = (packets.tail + 1) % packets.buffer_size; 
        if(packets.head == packets.tail){
            packets.is_empty = TRUE;
        }
        packets.is_full = FALSE;
        return 1;
    }
}

/**
 * @Function int Protocol_SendDebugMessage(char *Message)
 * @param Message, Proper C string to send out
 * @return SUCCESS (true) or ERROR (false)
 * @brief Takes in a proper C-formatted string and sends it out using ID_DEBUG
 * @warning this takes an array, do <b>NOT</b> call sprintf as an argument.
 * @author mdunne */
int Protocol_SendDebugMessage(char *Message){ //address is four bytes char is 1 byte
    int len;
    len = 0;
    while(Message[len] != '\0'){
        len++;
    }
    Protocol_SendPacket(len, ID_DEBUG, Message); //THE WAY THE len IS CALCULATED IS NOT WORKING!!!!
    //PutChar('a');
    return 1;
}

/**
 * @Function int Protocol_SendPacket(unsigned char len, void *Payload)
 * @param len, length of full <b>Payload</b> variable
 * @param Payload, pointer to data
 * @return SUCCESS (true) or ERROR (false)
 * @brief composes and sends a full packet
 * @author instructor W2022 */
int Protocol_SendPacket(unsigned char len, unsigned char ID, void *Payload){
    unsigned char checksum = 0;
    PutChar(PACKET_HEAD); //check the endianness on this guy
    //checksum = Protocol_CalcIterativeChecksum(PACKET_HEAD, checksum);
    PutChar(len + 1);
    //checksum = Protocol_CalcIterativeChecksum(len + 1, checksum);
    PutChar(ID);
    checksum = Protocol_CalcIterativeChecksum(ID, checksum);
    char * message = (char *)Payload; //type cast void to char
    int i;
    for(i=0;i<len;i++){
        PutChar((message[i]));
        checksum = Protocol_CalcIterativeChecksum(message[i], checksum);
    }
    PutChar(PACKET_TAIL);
    //checksum = Protocol_CalcIterativeChecksum(PACKET_TAIL, checksum);
    PutChar(checksum);
    PutChar(PACKET_END_0); //this needs to be sent as two characters
    PutChar(PACKET_END_1);
}

/**
 @Function unsigned char Protocol_ReadNextID(void)
 * @param None
 * @return Reads the ID of the next available Packet
 * @brief Returns ID or 0 if no packets are available
 * @author instructor W2022 */
unsigned char Protocol_ReadNextPacketID(void){
    if(packets.is_empty){
        return 0;
    }
    return packets.packet_arr[packets.tail]->ID;
}

/**
 * @Function flushPacketBuffer()
 * @param none
 * @return none
 * @brief flushes the rx packet circular buffer  
 * @author instructor W2022 */
void flushPacketBuffer (){
    uint8_t type;
    uint8_t len;
    unsigned char * payload;
    while(packets.is_empty != 1){
        Protocol_GetInPacket(&type, &len, payload);
        Protocol_SendPacket(type, len, payload);
    }
}

unsigned int convertEndian(unsigned int * number){
    //sizeof(int) better be 4
        unsigned int byte_0 = *number & 0xff000000; //this line crashes the program (probably improperly initialized memory)
        unsigned int byte_1 = *number & 0x00ff0000;
        unsigned int byte_2 = *number & 0x0000ff00;
        unsigned int byte_3 = *number & 0x000000ff;
        
        unsigned int new_byte_0 = byte_3 << 24;
        unsigned int new_byte_1 = byte_2 << 8;
        unsigned int new_byte_2 = byte_1 >> 8;
        unsigned int new_byte_3 = byte_0 >> 24;
        
        unsigned int new_number = new_byte_0 | new_byte_1 | new_byte_2 | new_byte_3;
        
        return new_number;
}
/*******************************************************************************
 * PRIVATE FUNCTIONS
 * Generally these functions would not be exposed but due to the learning nature 
 * of the class some are are noted to help you organize the code internal 
 * to the module. 
 ******************************************************************************/

/* BuildRxPacket() should implement a state machine to build an incoming
 * packet incrementally and return it completed in the called argument packet
 * structure (rxPacket is a pointer to a packet struct). The state machine should
 * progress through discrete states as each incoming byte is processed.
 * 
 * To help you get started, the following ADT is an example of a structure 
 * intended to contain a single rx packet. 
 * typedef struct rxpT {
 *    uint8_t ID;      
 *    uint8_t len;
 *    uint8_t checkSum; 
 *    unsigned char payLoad[MAXPAYLOADLENGTH];
 * }*rxpADT; 
 *   rxpADT rxPacket ...
 * Now consider how to create another structure for use as a circular buffer
 * containing a PACKETBUFFERSIZE number of these rxpT packet structures.
 ******************************************************************************/
uint8_t BuildRxPacket (rxpADT rxPacket, unsigned char reset){ //wtf is reset for???
    unsigned char checksum;
    unsigned char id;
    unsigned char ch;
    uint8_t len;
    int i;
    int boolean = 1;
    while(boolean){
        checksum = 0;
        ch = GetChar();
        //PutChar(ch);
        if(ch == PACKET_HEAD){  //thinks ch != 0xcc but prints it???
                                //sw = PARSE_PACKET; //it would be faster to use a GOTO!!!
                                //Protocol_CalcIterativeChecksum(PACKET_HEAD, checksum);
            len = GetChar();
            id = GetChar();
            for(i=0;i<len-1;i++){
                rxPacket->payLoad[i] = GetChar();
            }
            rxPacket->len = len - 1;
            //Protocol_CalcIterativeChecksum(len, checksum);
            rxPacket->ID = id;

            ch = GetChar(); //tail
            ch = GetChar(); //checksum
            ch = GetChar(); //end
            ch = GetChar(); //end
            boolean = 0;
        }
    }
}

/**
 * @Function char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
 * @param charIn, new char to add to the checksum
 * @param curChecksum, current checksum, most likely the last return of this function, can use 0 to reset
 * @return the new checksum value
 * @brief Returns the BSD checksum of the char stream given the curChecksum and the new char
 * @author mdunne */
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum){
    unsigned char checksum = (curChecksum >> 1) + (curChecksum << 7);
    checksum += charIn;
    return checksum;
}