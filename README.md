# Virtual Automotive CAN Bus Simulator in C

## Project Overview
This project is a high-fidelity, software-simulated CAN (Controller Area Network) Bus environment designed for automotive software development training and testing. It simulates multiple Electronic Control Units (ECUs) communicating over a virtual bus, supporting standard CAN frames, OBD-II diagnostics, and system logging.

## Key Features
- **Virtual CAN Bus Layer**: Broadcast-style message handling with ID and DLC support.
- **Engine ECU Simulation**: Real-time simulation of RPM, Coolant Temperature, and Throttle Position.
- **Brake ECU Simulation**: Simulates braking events, ABS activation, and hydraulic pressure.
- **Interactive Dashboard**: ASCII-based terminal UI showing live vehicle telemetry.
- **OBD-II Diagnostics**: Support for PID requests (0x01) and responses.
- **System Logger**: All bus traffic is recorded with timestamps in `logs/can_bus.log`.
- **Error Detection**: Basic detection of invalid DLC and bus overflows.

## Project Architecture
The simulator uses a modular architecture:
- `can.c`: Core bus logic and synchronization.
- `ecu.c` files: Domain-specific simulation logic.
- `dashboard.c`: Visualization layer.
- `menu.c`: User interaction and system control.

## Build Requirements
- **OS**: Windows (MinGW64 recommended)
- **Compiler**: GCC (C11 support)
- **Build System**: CMake 3.10+

## Compilation Instructions
Open your MinGW64 terminal and run:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Simulator
After building, run the executable:
```bash
./CANBusSimulator.exe
```

## Usage
1. **Start Simulation**: Watch live data flowing between ECUs on the dashboard.
2. **Send Custom Frame**: Manually inject CAN frames into the bus.
3. **OBD-II Test**: Run a diagnostic routine to query specific PIDs.
4. **View Logs**: Check the generated log files for historical analysis.

## Project Structure
```text
CAN-Bus-Simulator/
├── include/       # Header files
├── src/           # Implementation files
├── docs/          # Detailed documentation
├── logs/          # Generated CAN logs
└── CMakeLists.txt # Build configuration
```

## Future Improvements
- J1939 Protocol support.
- Graphical UI using raylib or SDL2.
- Network socket support for multi-process simulation.
