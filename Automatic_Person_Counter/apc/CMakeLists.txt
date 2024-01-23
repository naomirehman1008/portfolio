cmake_minimum_required(VERSION 3.16)

# Include pico sdk
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in the API
include(library/import.cmake)

# Create project and set basic info
project(APC C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)
pico_sdk_init()

add_executable(${PROJECT_NAME}
    main.c
    can_bus_send.c
    hw_settings.c
    door_sensor.c
    configuration.c
    custom_watchdog.c
)

pico_add_extra_outputs(${PROJECT_NAME})

#include libraries folder
include_directories(../../libraries/)

add_subdirectory(SerialConsole)
add_subdirectory(filesystem)
add_subdirectory(../../libraries/CAN ${CMAKE_CURRENT_BINARY_DIR}/CAN/)

target_link_libraries(${PROJECT_NAME} PUBLIC
    pico_stdlib
    pico_platform
    pico_multicore
    pico_time
    hardware_i2c
    hardware_irq
    VL53L1X_pico_api
    CANBUS
    serial_console
    filesystem
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
