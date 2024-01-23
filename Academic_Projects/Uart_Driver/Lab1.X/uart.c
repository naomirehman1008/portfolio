
#include <xc.h>
#include "uart.h"
#include <sys/attribs.h>
#include "BOARD.h"
#include "Circular_Buffer.h"
#include "LEDs.h"
//#include "uart.h"
/**
 * @Function Uart_Init(unsigned long baudrate)
 * @param baudrate
 * @return none
 * @brief  Initializes UART1 to baudrate N81 and creates circ buffers
 * @author instructor ece121 W2022 */
#define FPB 4000000

#define PART1 1
#define PART2 2

#define BUFFER_LENGTH 8
#define DATA_SIZE 1
//#define TEST

Buffer buffer_rx;
Buffer buffer_tx;

int i;
int rx_event_flag;

char test_string[] = "I love UART so much";

int partnum = PART2;

void Uart_Init(unsigned long baudRate);

void Uart_Init(unsigned long baudRate){
    BOARD_Init();
    Init_Buffer(&buffer_rx);
    Init_Buffer(&buffer_tx);
    
    U1MODE = 0; //clear control registers
    U1STA = 0;
    U1BRG = 21;

    
    //U1MODEbits.BRGH = 0; 
    U1MODEbits.PDSEL = 0; //parity select
    U1MODEbits.STSEL = 0; //stop bit select
    
    IPC6CLR = 0x00000003;
    
    IEC0bits.U1RXIE = 1; //enable interrupts
    IEC0bits.U1TXIE = 1;
    
    IPC6bits.U1IP = 4;//set priority
    //IPC6bits.U1IS = 0b11;
    
    U1STAbits.URXISEL = 0b00; //select interrupt mode
    U1STAbits.UTXISEL = 0b00;

//    U1MODEbits.UEN = 0b00;
    U1STAbits.URXEN = 1;
    U1STAbits.UTXEN = 1; //enable TX and RX THIS WILL SET THE INTERRUPT FLAG!!!

    U1MODEbits.ON = 1; //enable UART
    
    U1TXREG = 0;
    U1RXREG = 0;

}
#ifdef TEST
int main(){
    /*BOARD_Init();
    Init_Buffer(&buffer_rx);
    Init_Buffer(&buffer_tx);*/
     //Initiate read and write buffers
    
    Uart_Init(115200);
    //LEDS_Init();
    rx_event_flag = 0;
    if(partnum == PART1){       //part 1 loop back
        while(1){
            if(U1STAbits.URXDA){ //if the receiver has data set read rx data to tx register
                U1TXREG = U1RXREG; 
            }
        }
    }
    
    if(partnum == PART2){
        for(i=0;i<20;i++){ 
            PutChar(test_string[i]); //print a string to the console
        }
        while(1){
            PutChar(GetChar());     //loop back with getchar and putchar
        }
    }
}
#endif /*test*/
/**
 * @Function int PutChar(char ch)
 * @param ch - the character to be sent out the serial port
 * @return True if successful, else False if the buffer is full or busy.
 * @brief  adds char to the end of the TX circular buffer
 * @author instrutor ECE121 W2022 */
//int PutChar(char ch);

/**
 * @Function unsigned char GetChar(void)
 * @param None.
 * @return NULL for error or a Char in the argument.
 * @brief  dequeues a character from the RX buffer,
 * @author instructor, ECE121 W2022 */
//unsigned char GetChar(unsigned char *);

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/****************************************************************************
 * Function: IntUart1Handler
 * Parameters: None.
 * Returns: None.
 * The PIC32 architecture calls a single interrupt vector for both the 
 * TX and RX state machines. Each IRQ is persistent and can only be cleared
 * after "removing the condition that caused it".  
 ****************************************************************************/
void __ISR(_UART1_VECTOR) IntUart1Handler(void){
    char data;
    if(IFS0bits.U1RXIF){                //RX interrupt
        //rx_event_flag = 1;              //THERE IS DATA TO READ!
        IFS0CLR = _IFS0_U1RXIF_MASK;
        data = U1RXREG;
        Buffer_Write(&buffer_rx, data); //write the data to the read buffer
        
    }
    if(IFS0bits.U1TXIF){                //TX interrupt
        if(buffer_tx.is_empty){
            IFS0CLR = _IFS0_U1TXIF_MASK;
        }
        else{
            IFS0CLR = _IFS0_U1TXIF_MASK;
            U1TXREG = Buffer_Read(&buffer_tx);  //read data from tx buffer into the tx register
            
        }
    }
}

/**
 * @Function int PutChar(char ch)
 * @param ch - the character to be sent out the serial port
 * @return True if successful, else False if the buffer is full or busy.
 * @brief  adds char to the end of the TX circular buffer
 * @author instrutor ECE121 W2022 */
int PutChar(char ch){
    //U1STAbits.UTXEN = 1;
    if(buffer_tx.is_full){
        return 0;
    }
    Buffer_Write(&buffer_tx, ch);
    /*if(U1STAbits.TRMT){
        IFS0bits.U1TXIF = 1;
    }*/
    while(1){
        if(U1STAbits.TRMT){
            IFS0bits.U1TXIF = 1;
            return 1;
        }
    }
}

/**
 * @Function unsigned char GetChar(void)
 * @param None.
 * @return NULL for error or a Char in the argument.
 * @brief  dequeues a character from the RX buffer,
 * @author instructor, ECE121 W2022 */
unsigned char GetChar(void){
    /*if(buffer_rx.is_empty){
        return NULL;
    }*/
    while(1){ //can't call getchar whenever!!!
        if(!(buffer_rx.is_empty)){  
            //rx_event_flag = 0;              //clear flag
            return Buffer_Read(&buffer_rx); //read from buffer
        }
    }
}

