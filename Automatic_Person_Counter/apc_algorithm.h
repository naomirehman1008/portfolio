// Naomi Rehman 2024

#ifndef _APC_ALGORITHM_H_
#define _APC_ALGORITHM_H_

typedef enum BoardingEvents {
	boarding,
	deboarding,
	invalid,
	no_event
} BoardingEvents;

int APC_UpdateState(uint16_t distance, uint16_t dist_threshold, uint8_t sensor_num);

BoardingEvents APC_EvalState();

#endif //_APC_ALGORITHM_H_