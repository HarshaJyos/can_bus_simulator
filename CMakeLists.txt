cmake_minimum_required(VERSION 3.10)
project(CANBusSimulator C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/main.c
    src/can.c
    src/engine_ecu.c
    src/brake_ecu.c
    src/dashboard.c
    src/logger.c
    src/obd2.c
    src/menu.c
    src/utils.c
    src/dbc.c
)

add_executable(CANBusSimulator ${SOURCES})

if(WIN32)
    target_link_libraries(CANBusSimulator winmm pthread)
else()
    target_link_libraries(CANBusSimulator pthread)
endif()

# Unit Tests
add_executable(can_tests 
    tests/test_can.c
    src/can.c
    src/dbc.c
    src/logger.c
    src/utils.c
)
target_link_libraries(can_tests winmm pthread)
