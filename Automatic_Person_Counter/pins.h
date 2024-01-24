#ifndef PINS_H
#define PINS_H

#include "globals.h"

// CAN BUS pins
#define CAN_BUS_RX 12
#define CAN_BUS_TX 13

// Distance sensor 0 (Right side)
#define I2C_SDA 4
#define I2C_SCL 5

// Distance sensor 1 (Left side)
#define I2C_SDA_2 26
#define I2C_SCL_2 27

// 6-11 are the pins for the 6 Address jumpers
#define ADDRESS_J0_PIN 6
#define ADDRESS_J1_PIN 7
#define ADDRESS_J2_PIN 8
#define ADDRESS_J3_PIN 9
#define ADDRESS_J4_PIN 10
#define ADDRESS_J5_PIN 11

// Direction Select Jumper
#define DIRECTION_SELECT_PIN 14

// Door Open / Closed detection pin.
#define DOOR_SENSOR_PIN 15

// SD card pins
#define SD_CD_PIN 22  // Card detect
#define SD_CS_PIN 17  // Chip select
#define SD_DI_PIN 19  // Data in
#define SD_DO_PIN 16  // Data out
#define SD_SCK_PIN 18 // Clock

#endif  // PINS_H
