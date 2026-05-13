cmake_minimum_required(VERSION 3.10)
project(CANBusSimulator C)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
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
)

# Output directory for logs
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Add executable
add_executable(CANBusSimulator ${SOURCES})

# Windows specific libraries (if any, e.g. for threading or timing)
if(WIN32)
    target_link_libraries(CANBusSimulator winmm)
endif()

# Installation rules (optional)
install(TARGETS CANBusSimulator DESTINATION bin)
