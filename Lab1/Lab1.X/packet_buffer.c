/* Template ver 1.0 W2022
 * File:   LibraryTemplate.c
 * Author: <Your Name>
 * Brief: 
 * Created on <month> <day>, <year>, <hour> <pm/am>
 * Modified on <month> <day>, <year>, <hour> <pm/am
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "Circular_Buffer.h" // The header file for this source file. 
#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>
#include "BOARD.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TRUE 1
#define FALSE 0

/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
/*typedef struct Buffer {
    int head;
    int tail;
    int buffer_size;
    char buffer_arr[8];
    int is_full; //??bad??
    int is_empty;
    
} Buffer;*/
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                 *
 ******************************************************************************/
int Init_Buffer(Buffer *buffer);
/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE FUNCTION IMPLEMENTATIONS                                            *
 ******************************************************************************/
int Init_Buffer(Buffer *buffer){
    //buffer = (char*)calloc(sizeof(*buffer) + BUFFER_LENGTH, sizeof(char));
    buffer->head = 0;
    buffer->tail = 0;
    buffer->buffer_size = 8;
    //buffer->buffer_arr = (char *)calloc(BUFFER_LENGTH, size);
    buffer->is_full = FALSE;
    buffer->is_empty = TRUE;
}


int Buffer_Write(Buffer *buffer, char new_data){ 
    
    if(buffer->is_full){
        return 0;
    }
    else{
        buffer->buffer_arr[buffer->head] = new_data;
        buffer->head = (buffer->head + 1) % buffer->buffer_size; //this is definitely WRONG!?!?!!!?
        if(buffer->head == buffer->tail){
            buffer->is_full = TRUE;
        }
        buffer->is_empty = FALSE;
        return 1;
    }
}

int Buffer_Read(Buffer *buffer){
    if(buffer->is_empty){
        return 0;
    }
    else{
        char data = buffer->buffer_arr[buffer->tail];
        buffer->tail = (buffer->tail + 1) % buffer->buffer_size; //this is definitely WRONG!?!?!!!?
        if(buffer->head == buffer->tail){
            buffer->is_empty = TRUE;
        }
        buffer->is_full = FALSE;
        return data;
    }
}


/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> 
uint8_t someFunction(int foo)*/




