#include "configuration.h"

#include "SerialConsole/serial_console.h"
#include "globals.h"
#include "door_sensor.h"
#include "hw_settings.h"

#include "VL53L1X_types.h"
#include "VL53L1X_calibration.h"
#include "VL53L1X_calibration2.h"
#include "VL53L1X_api.h"
#include "VL53L1X_api2.h"

#include <stdio.h>

#define VL53L1_ERROR_NONE 0

unsigned int total_boardings = 0;
unsigned int total_departures = 0;

bool sensor_a_initialized = false;
bool sensor_b_initialized = false;

void setup_configuration()
{
    print_status_ptr = &print_status;
    run_calibrate_a_ptr = &run_calibrate_a;
    run_calibrate_b_ptr = &run_calibrate_b;
    run_calibrate_xtalk_a_ptr = &run_calibrate_xtalk_a;
    run_calibrate_xtalk_b_ptr = &run_calibrate_xtalk_b;
}

void print_status() {
    printf("Status:\n");
    printf("Total boardings: %d\n", total_boardings);
    printf("Total departures: %d\n", total_departures);

    printf("door_sensor_is_open: %s\n", door_sensor_is_open() ? "true" : "false");

    // print CAN bus address range
    printf("CAN bus address range: %d - %d\n", CAN_ADDRESS_MIN, CAN_ADDRESS_MAX);

    // show direction setting
    printf("Direction: %s\n", get_direction() ? "INVERTED" : "NOT INVERTED");

    // show address setting
    printf("Address: %d\n", get_address());

    // show sensor initialization status
    printf("Sensor A initialized: %s\n", sensor_a_initialized ? "true" : "false");
    printf("Sensor B initialized: %s\n", sensor_b_initialized ? "true" : "false");

    if (sensor_a_initialized) {
        VL53L1X_Version_t a_version;
        VL53L1X_GetSWVersion(&a_version);
        printf("Sensor A SW version: %d.%d.%d\n", a_version.major, a_version.minor, a_version.build);
    }
    if (sensor_b_initialized) {
        VL53L1X_Version_t b_version;
        VL53L1X_GetSWVersion2(&b_version);
        printf("Sensor B SW version: %d.%d.%d\n", b_version.major, b_version.minor, b_version.build);
    }

    // display data about VL53L1X_GetTimingBudgetInMs for each sensor
    if (sensor_a_initialized) {
        uint16_t budget_a = 0;
        VL53L1X_GetTimingBudgetInMs(I2C_DEV_ADDR, &budget_a);
        printf("Timing budget A: %d\n", budget_a);
    }
    if (sensor_b_initialized) {
        uint16_t budget_b = 0;
        VL53L1X_GetTimingBudgetInMs2(I2C_DEV_ADDR, &budget_b);
        printf("Timing budget B: %d\n", budget_b);
    }

    // print VL53L1X_GetDistanceMode for each sensor
    if (sensor_a_initialized) {
        uint16_t mode_a = 0;
        VL53L1X_GetDistanceMode(I2C_DEV_ADDR, &mode_a);
        printf("Distance mode A: %d\n", mode_a);
    }
    if (sensor_b_initialized) {
        uint16_t mode_b = 0;
        VL53L1X_GetDistanceMode2(I2C_DEV_ADDR, &mode_b);
        printf("Distance mode B: %d\n", mode_b);
    }

    // print VL53L1X_GetInterMeasurementInMs for each sensor
    if (sensor_a_initialized) {
        uint16_t measurement_a = 0;
        VL53L1X_GetInterMeasurementInMs(I2C_DEV_ADDR, &measurement_a);
        printf("Measurement delay A: %d\n", measurement_a);
    }
    if (sensor_b_initialized) {
        uint16_t measurement_b = 0;
        VL53L1X_GetInterMeasurementInMs2(I2C_DEV_ADDR, &measurement_b);
        printf("Measurement delay B: %d\n", measurement_b);
    }

    // print VL53L1X_GetRangeStatus for each sensor
    if (sensor_a_initialized) {
        uint8_t status_a = 0;
        VL53L1X_GetRangeStatus(I2C_DEV_ADDR, &status_a);
        printf("Range status A: %d\n", status_a);
    }
    if (sensor_b_initialized) {
        uint8_t status_b = 0;
        VL53L1X_GetRangeStatus2(I2C_DEV_ADDR, &status_b);
        printf("Range status B: %d\n", status_b);
    }

    // print VL53L1X_GetOffset for each sensor
    if (sensor_a_initialized) {
        int16_t offset_a = 0;
        VL53L1X_GetOffset(I2C_DEV_ADDR, &offset_a);
        printf("Offset A: %d\n", offset_a);
    }
    if (sensor_b_initialized) {
        int16_t offset_b = 0;
        VL53L1X_GetOffset2(I2C_DEV_ADDR, &offset_b);
        printf("Offset B: %d\n", offset_b);
    }

    // print VL53L1X_GetDistanceThresholdLow for each sensor
    if (sensor_a_initialized) {
        uint16_t threshold_low_a = 0;
        VL53L1X_GetDistanceThresholdLow(I2C_DEV_ADDR, &threshold_low_a);
        printf("Distance threshold low A: %d\n", threshold_low_a);
    }
    if (sensor_b_initialized) {
        uint16_t threshold_low_b = 0;
        VL53L1X_GetDistanceThresholdLow2(I2C_DEV_ADDR, &threshold_low_b);
        printf("Distance threshold low B: %d\n", threshold_low_b);
    }

    // print VL53L1X_GetDistanceThresholdHigh for each sensor
    if (sensor_a_initialized) {
        uint16_t threshold_high_a = 0;
        VL53L1X_GetDistanceThresholdHigh(I2C_DEV_ADDR, &threshold_high_a);
        printf("Distance threshold high A: %d\n", threshold_high_a);
    }
    if (sensor_b_initialized) {
        uint16_t threshold_high_b = 0;
        VL53L1X_GetDistanceThresholdHigh2(I2C_DEV_ADDR, &threshold_high_b);
        printf("Distance threshold high B: %d\n", threshold_high_b);
    }


    // show max distance setting
    // printf("Max distance: %d\n", get_max_dist());

    // show measurement delay setting
    // printf("Measurement delay: %d\n", get_measurement_delay());
}

void run_calibrate_a(uint16_t targetDistInMm, int16_t* offset) {

    // calibrate sensors
    printf("Calibrating sensors(%i)...\n", targetDistInMm);
    VL53L1X_Status_t return_status;
    return_status = VL53L1X_CalibrateOffset(I2C_DEV_ADDR, targetDistInMm, offset);
    // print return_status
    if (return_status == VL53L1_ERROR_NONE) {
        printf("Calibration sucessful!\n");
        printf("Offset: %d\n", *offset);
    }
    else {
        printf("Calibration failed: %i\n", return_status);
    }
    printf("Calibration complete.\n");
}
void run_calibrate_b(uint16_t targetDistInMm, int16_t* offset) {

    // calibrate sensors
    printf("Calibrating sensor 2 (%i)...\n", targetDistInMm);
    VL53L1X_Status_t return_status;
    return_status = VL53L1X_CalibrateOffset2(I2C_DEV_ADDR, targetDistInMm, offset);
    // print return_status
    if (return_status == VL53L1_ERROR_NONE) {
        printf("Calibration sucessful 2!\n");
        printf("Offset: %d\n", *offset);
    }
    else {
        printf("Calibration failed 2: %i\n", return_status);
    }
    printf("Calibration complete 2.\n");
}

void run_calibrate_xtalk_a(uint16_t targetDistInMm, uint16_t* xtalk)
{
    // calibrate sensors
    printf("Calibrating xtalk sensors 1(%i)...\n", targetDistInMm);
    VL53L1X_Status_t return_status;
    return_status = VL53L1X_CalibrateXtalk(I2C_DEV_ADDR, targetDistInMm, xtalk);
    // print return_status
    if (return_status == VL53L1_ERROR_NONE) {
        printf("XTalk Calibration sucessful 1!\n");
        printf("Xtalk: %d\n", *xtalk);
    }
    else {
        printf("Calibration failed 1: %i\n", return_status);
    }
    printf("Calibration complete 1.\n");
}
void run_calibrate_xtalk_b(uint16_t targetDistInMm, uint16_t* xtalk)
{
    // calibrate sensors
    printf("Calibrating xtalk sensor 2(%i)...\n", targetDistInMm);
    VL53L1X_Status_t return_status;
    return_status = VL53L1X_CalibrateXtalk2(I2C_DEV_ADDR, targetDistInMm, xtalk);
    // print return_status
    if (return_status == VL53L1_ERROR_NONE) {
        printf("XTalk Calibration sucessful 2!\n");
        printf("Xtalk: %d\n", *xtalk);
    }
    else {
        printf("Calibration failed 2: %i\n", return_status);
    }
    printf("Calibration complete 2.\n");
}
