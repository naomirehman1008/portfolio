/*
 * File:   uart_note.c
 * Author: Petersen, ECE121 W2023
 */
 // Be sure this include is added to your uart.c code.
#include <sys/attribs.h> 

/*******************************************************************************
 * PRIVATE FUNCTIONS to add to your Uart.c file                                                         *
 ******************************************************************************/

/**
* Refer to ...\docs\MPLAB C32 Libraries.pdf: 32-Bit Language Tools Library.
* In sec. 2.13.2 helper function _mon_putc() is noted as normally using
* UART2 for STDOUT character data flow. Adding a custom version of your own
* can redirect this to UART1 by calling your putchar() function.   
*/
void _mon_putc(char c){
 //your code goes here
}


/****************************************************************************
 * Function: IntUart1Handler
 * Parameters: None.
 * Returns: None.
 * The PIC32 architecture calls a single interrupt vector for both the 
 * TX and RX state machines. Each IRQ is persistent and can only be cleared
 * after "removing the condition that caused it". This function is declared in
 * sys/attribs.h. 
 ****************************************************************************/
void __ISR(_UART1_VECTOR) IntUart1Handler(void) {
 //your interrupt handler code goes here
}
