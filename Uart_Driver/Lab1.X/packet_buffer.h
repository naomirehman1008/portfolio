/* Template ver 1.0 W2022
 * File:   filename.h
 * Author: <Your Name>
 * Brief: 
 * Created on <month> <day>, <year>, <hour> <pm/am>
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

#ifndef PACKET_BUFFER_H // Header guard
#define	PACKET_BUFFER_H //

/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "BOARD.h"
#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


typedef struct Buffer {
    int head;
    int tail;
    int buffer_size;
    int is_full; 
    int is_empty;
    Packet buffer_arr[16];
} Buffer;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>,
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> 
uint8_t someFunction(int foo);*/

/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>,
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
int Init_Buffer(Buffer *buffer);

//int Is_Buffer_Full(char* buffer);

/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>,
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
//int Is_Buffer_Empty(char* buffer);

/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>,
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
int Buffer_Write(Buffer *buffer, char new_data);

/**
 * @Function someFunction(void)
 * @param foo, some value
 * @return TRUE or FALSE
 * @brief 
 * @note 
 * @author <Your Name>,
 * @modified <Your Name>, <year>.<month>.<day> <hour> <pm/am> */
int Buffer_Read(Buffer *buffer);


#endif	/* LIBRARYTEMPLATE_H */ // End of header guard


