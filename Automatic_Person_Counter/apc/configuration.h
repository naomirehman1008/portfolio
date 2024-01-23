#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdbool.h>
#include <stdint.h>

extern bool sensor_a_initialized;
extern bool sensor_b_initialized;

extern unsigned int total_boardings;
extern unsigned int total_departures;

void setup_configuration();

void print_status();
void run_calibrate_a(uint16_t targetDistInMm, int16_t* offset);
void run_calibrate_b(uint16_t targetDistInMm, int16_t* offset);
void run_calibrate_xtalk_a(uint16_t targetDistInMm, uint16_t* xtalk);
void run_calibrate_xtalk_b(uint16_t targetDistInMm, uint16_t* xtalk);

#endif // CONFIGURATION_H
