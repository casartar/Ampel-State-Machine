cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(test_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(traffic_light
        src/traffic_light.c
)
pico_enable_stdio_usb(traffic_light 1)
pico_enable_stdio_uart(traffic_light 1)

pico_add_extra_outputs(traffic_light)

target_link_libraries(traffic_light pico_stdlib)
