#ifndef GLOBALS_H
#define GLOBALS_H

#include "../../sw_version.h"

#define I2C_DEV_ADDR 0x29
#define SOFTWARE_VERSION PINPOINT_SW_VERSION // "0.0.1"

// this number is the lowest address that can be set on the jumpers
// this leaves room for other devices on the addresses below this one.
// (and above CAN_ADDRESS_MAX)
#define CAN_ADDRESS_SHIFT 16
#define CAN_ADDRESS_MIN 0 + CAN_ADDRESS_SHIFT
#define CAN_ADDRESS_MAX 63 + CAN_ADDRESS_SHIFT

// defining the gpio function so we can change the code for the pico w easily.
#define apc_gpio_init    gpio_init
#define apc_gpio_set_dir gpio_set_dir
#define apc_gpio_get     gpio_get

#define PRODUCT_NAME "APC"

#endif // GLOBALS_H
