///////////////////////////////////
//new apc with CAN
//11/8/2023
//Naomi Rehman
///////////////////////////////////


//standard includes
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
//#include <stdatomic.h>

//VL53L1X api (each sensor has its own)
// #include "VL53L1X_api2.h"
// #include "VL53L1X_api.h"
// #include "VL53L1X_types.h"
// #include "VL53L1X_calibration2.h"
// #include "VL53L1X_calibration.h"

//VL53L1X c++ api

#include "VL53L1X_api3.h"
extern "C" {
    #include "VL53L1X_types.h"
}


//pico includes
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/watchdog.h"
#include "pico/sync.h"

//add extern c things in .h files
extern "C" {
//bus includes
#include "apc_algorithm.h"
#include "pins.h"
#include "SerialConsole/serial_console.h"
#include "door_sensor.h"
#include "hw_settings.h"
#include "can_bus_send.h"
#include "filesystem/filesystem.h"
#include "configuration.h"
#include "globals.h"
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define TESTING

//Global variables

//changed from atomic_int to int
int loop0ready;
int loop1ready;

mutex_t myMutex; //<3

uint16_t dev = I2C_DEV_ADDR;

uint8_t sensorState2=0;
uint16_t Distance2, Signal2;
uint8_t RangeStatus2 = 0;
uint8_t dataReady2;
VL53L1X_Status_t status2;
uint32_t curTime2;

uint8_t reinit_sensor_2 = 1;

uint16_t prevDist = 0;
uint16_t times_same = 0;
int times_reset = 0;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++

// function definitions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
void core1_entry0(){
    //initalize and set up sensor 1
    //prinfs removed to avoid collisions
    //gotos >>>>>
    VL53L1X_Sensor sensor2;

    while(1){
        if(reinit_sensor_2){
            reinit_sensor_2 = 0;
            sensor2.VL53L1X_SensorSetUp(I2C_DEV_ADDR, i2c1, I2C_SDA, I2C_SCL, DISTANCE_MODE, TIMING_BUDGET, TIMING_BUDGET, 2);
        }
        
        //wait for other core to push
        multicore_fifo_pop_blocking();

        sensor2.VL53L1X_DistanceMeasurement(I2C_DEV_ADDR, RangeStatus2, Distance2);

        sensor2.VL53L1X_DistanceErrorCorrection(Distance2, MAX_DISTANCE, MIN_DISTANCE, RangeStatus2);

        APC_UpdateState(Distance2, DIST_THRESHOLD, 2);
    
        //signal that this core is done
        multicore_fifo_push_blocking(Distance2);

    }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
int run_detections();
void prepare_loop(int delay_in_seconds);

void prepare_loop(int delay_in_seconds) {
    for (int i = 0; i < delay_in_seconds; i++) {
        printf("Prepairing %d", i);
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf("\n");
    }
}


void on_boarding_detected() {
    printf("Boarding detected!\n");
    printf("Person boarding");
    total_boardings++;
    send_boarding();
    printf("Total boardings: %d\n", total_boardings);
}

void on_deboarding_detected() {
    printf("Deboarding detected!\n");
    printf("Person deboarding");
    total_departures++;
    send_deboarding();
    printf("Total departures: %d\n", total_departures);
}

void on_a_then_b() {
    printf("A then B\n");
    if (get_direction()) {
        on_boarding_detected();
    } else {
        on_deboarding_detected();
    }
}

void on_b_then_a() {
    printf("B then A\n");
    if (get_direction()) {
        on_deboarding_detected();
    } else {
        on_boarding_detected();
    }
}



int main() {
    //Configure x-shut pin
    gpio_init(KILL_PIN);

    gpio_set_dir(KILL_PIN, GPIO_OUT);

    gpio_put(KILL_PIN, 1); //drive pin high, open transistor

    prepare_loop(3);

    // init console
    setup_configuration();

    // init can bus
    canbus_setup();   

    // init door sensor
    door_sensor_init();

    // init hw jumpers
    hw_settings_init();

    // print CAN bus address range
    printf("CAN bus address range: %d - %d\n", CAN_ADDRESS_MIN, CAN_ADDRESS_MAX);

    // show address setting
    printf("Address: %d\n", get_address());

    // show direction setting
    printf("Direction: %s\n", get_direction() ? "INVERTED" : "NOT INVERTED");

    // init filesystem
    init_filesystem();

    multicore_launch_core1(core1_entry0); 
    //why?
    sleep_ms(1000);

    VL53L1X_Sensor sensor1;

    uint8_t sensorState=0;
    uint16_t Distance, Signal;
    uint8_t RangeStatus = 0;
    uint8_t dataReady;
    uint32_t curTime;

    VL53L1X_Status_t status;
    //blink LED on boot
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (int i = 0; i < 10; i++) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(250);
    } 

    printf("Starting APC!\n");
    //start core 1
      
    //enable watchdog, restart after 3s
    watchdog_enable(10000, false);    

    sensor1.VL53L1X_SensorSetUp(I2C_DEV_ADDR, i2c0, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, DISTANCE_MODE, TIMING_BUDGET, TIMING_BUDGET, 1);

    watchdog_update();
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    print_hello_message();

    watchdog_update();
    
    while (1) {
        //prevents core 1 from measuring again
        //starting measurement, tell other core to start
        multicore_fifo_push_blocking(1);
        //check for datas
        
        sensor1.VL53L1X_DistanceMeasurement(I2C_DEV_ADDR, RangeStatus, Distance);

        sensor1.VL53L1X_DistanceErrorCorrection(Distance, MAX_DISTANCE, MIN_DISTANCE, RangeStatus);
        
        APC_UpdateState(Distance, DIST_THRESHOLD, 1);
        
        //wait for other core to finish
        multicore_fifo_pop_blocking();
        //process data
        //if both sensors clear
        BoardingEvents crossing = APC_EvalState();
        switch (crossing)
        {
            case boarding:
                on_boarding_detected();
                break;

            case deboarding:
                on_deboarding_detected();
                break;

            case invalid:
                printf("invalid crossing\n");
                break;
            case no_event:
                break;

            default:
                printf("APC_EvalState() in a fucked state");

        }
        watchdog_update();

        //this is ugly but ball
        if(Distance2 == prevDist){
                times_same += 1;
           }
           else{
                times_same = 0;
           }
            prevDist = Distance2;
           if(times_same >= 10){
                times_same = 0;
                printf("reinitializing i2c for sensor 2");
                sleep_ms(10);
                gpio_put(KILL_PIN, 0); //drive pin high, open transistor
                printf("gpio set low\n");
                sleep_ms(1000);
                gpio_put(KILL_PIN, 1); //drive pin high, open transistor
                reinit_sensor_2 = 1;
                times_reset += 1;
            }

        serial_poll();
        loop0ready = 1;
        // printf("Execution time: %ld\n", to_ms_since_boot(get_absolute_time()) - last_loop_time);
        // last_loop_time = to_ms_since_boot(get_absolute_time());
    }
    return 1;
}


int run_detections() {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++

    multicore_launch_core1(core1_entry0); 
    //why?
    sleep_ms(1000);

    VL53L1X_Sensor sensor1;

    uint8_t sensorState=0;
    uint16_t Distance, Signal;
    uint8_t RangeStatus = 0;
    uint8_t dataReady;
    uint32_t curTime;
    int boardings = 0;
    int deboardings = 0;

    VL53L1X_Status_t status;
    //blink LED on boot
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (int i = 0; i < 10; i++) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(250);
    } 

    printf("Starting APC!\n");
    //start core 1
      
    //enable watchdog, restart after 3s
    watchdog_enable(10000, false);    

    sensor1.VL53L1X_SensorSetUp(I2C_DEV_ADDR, i2c0, PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, DISTANCE_MODE, TIMING_BUDGET, TIMING_BUDGET, 1);

    watchdog_update();
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
    print_hello_message();
    watchdog_update();
    
    while (1) {
        //prevents core 1 from measuring again
        loop0ready = 0;
        //starting measurement, tell other core to start
        multicore_fifo_push_blocking(1);
        //check for datas
        
        sensor1.VL53L1X_DistanceMeasurement(I2C_DEV_ADDR, RangeStatus, Distance);

        sensor1.VL53L1X_DistanceErrorCorrection(Distance, MAX_DISTANCE, MIN_DISTANCE, RangeStatus);
        
        APC_UpdateState(Distance, DIST_THRESHOLD, 1);
        
        //wait for other core to finish
        multicore_fifo_pop_blocking();
        //process data
        //if both sensors clear
        BoardingEvents crossing = APC_EvalState();
        switch (crossing)
        {
            case boarding:
                on_boarding_detected();
                break;

            case deboarding:
                on_deboarding_detected();
                break;

            case invalid:
                printf("invalid crossing\n");
                break;
            case no_event:
                break;

            default:
                printf("APC_EvalState() in a fucked state");

        }
        watchdog_update();

        //this is ugly but ball
        if(Distance2 == prevDist){
                times_same += 1;
           }
           else{
                times_same = 0;
           }
            prevDist = Distance2;
           if(times_same >= 10){
                times_same = 0;
                printf("reinitializing i2c for sensor 2");
                sleep_ms(10);
                gpio_put(KILL_PIN, 0); //drive pin high, open transistor
                printf("gpio set low\n");
                sleep_ms(1000);
                gpio_put(KILL_PIN, 1); //drive pin high, open transistor
                reinit_sensor_2 = 1;
                times_reset += 1;
            }

        serial_poll();
        loop0ready = 1;
        // printf("Execution time: %ld\n", to_ms_since_boot(get_absolute_time()) - last_loop_time);
        // last_loop_time = to_ms_since_boot(get_absolute_time());
    }
    return 1;
}

void prepare_loop(int delay_in_seconds) {
    for (int i = 0; i < delay_in_seconds; i++) {
        printf("Prepairing %d", i);
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf("\n");
    }
}