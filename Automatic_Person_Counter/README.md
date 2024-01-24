# APC

This is the code for the APC.

### Instructions to Run

Create a folder called 'build' in this directory. Within this directory run 'cmake ..'. You only need to do this once. Then run 'make'. Once make is done running, open the build folder. Copy APC.uf2. Hold down the bootsel button on the apc pico (the only button), then plug it in to your computer. Paste the .uf2 into the pico.

For trouble shooting APC outputs boardings and deboardings to serial. Connect to the appopriate port in serial monitor of your choice (coolterm).

### how apc stuff works:

The user manuals and datasheets for the VL53L1X sensor are in the documentation folder.
Init:
First the sensors must be configured. The I2C is first configured for each sensor, then the sensor is initialized. The library (https://github.com/alex-mous/VL53L1X-C-API-for-Raspberry-Pi-Pico) only supports 1 sensor at this moment, so we have a copy of the library. All names and functions in the duplicate library have a 2 in them. The pins for i2c0 bus are the default i2C pins (gpio pins 4 and 5). The i2c1 bus doesn't have default pins, so GPIO pins 27 and 28 are used. Currently, the original library MUST be initialized on the i2c1 bus, otherwise the system crashes a lot (reason unkown). After initalizing i2c, the sensor is booted. The boot state is 0 if not booted, 1 on initial boot, and 3 while operating (the 3 might be a bug, but if you see it its fine). Once the sensor is booted (after do while), the parameters are configured. The current parameter setup is:

Distance Mode = 2 (long)
Timing Budget = 33 ms
Inter Measurement Period = 33 ms

Distance mode sets the range of distance measurements. The intermeasurement period is the time between ranges. The timing budget is the amount of time collecting a range measurement. These are currently both set to 33ms, the smallest time recommended to get accurate measurements (20ms is the lowest possible). The intermeasurement period must be at least as long as the timing budget. VL53L1X_StartRanging only needs to be called once.

Ranging:
To take a measurement:
- check for dataReady
- GetRangeStatus
- getDistance
- ClearInterrupt

The range status is strictly required, but is good for error handling. The rest of these steps are mandatory.

Multicore:
This version of APC uses both RP2040 cores. Core 1 is used to range the second sensor. Since core 1 only ever uses I2C0 and vice versa, there are no collisions on the i2c. The cores are synchronized so they range at the same time, then core1 waits for core 0 to evaluate the data.

Algorithm:
The algorithm determines the boarding direction based on when sensors are covered and cleared. Instead of using global time, the program increments the 'time' every time core 0 finishes a loop. The order of the sensors being cleared must be the same order, or it is an invalid crossing. In any boarding, the cover event can only happen once per sensor, but the clear event is updated every time a sensor is no longer covered. Once both sensors are cleared the times are evaluated to determine the direction of the crossing. 

Bus Setup:

APC works best when wall mounted. The bus doorways are too wide to accurately track passengers. It is fine if APC points out a window or at a shiny surface, because error handling overwrites these measurements with the maximum distance. The DIST_THRESHOLD should be calibrated on bus. Mounting at about waist hight is recommended.

### main.c
This contains the code for apc. canbus_setup() must be called in init for CAN to work. On a boarding/deboarding a can message is sent to the main unit, and also printed to serial out.
### can_bus_send.h/c
This contains the functions to send boardings and deboardings over CAN. These functions call the can2040 library, which is located in Libraries/CAN.
### configuration.h/c
Used to calibrate sensors if needed (not used in main)
### door_sensor.h/c
unimplemented
### globals
a bunch of defines
### apc_algorithm.h/c
The algorithm to detect boardings and deboardings
