# Architecture Documentation - CAN Bus Simulator

## 1. System Overview
The simulator follows a **Producer-Consumer** architecture over a shared virtual medium (the CAN Bus).

### ASCII Architecture Diagram
```text
 +-------------------------------------------------------+
 |                  USER INTERFACE (menu.c)              |
 +---------------------------+---------------------------+
                             |
         +-------------------+-------------------+
         |                                       |
 +-------v-------+                       +-------v-------+
 |  ENGINE ECU   |                       |   BRAKE ECU   |
 | (Producer)    |                       |  (Producer)   |
 +-------+-------+                       +-------+-------+
         |                                       |
         |           VIRTUAL CAN BUS             |
         +-------------------+-------------------+
                             |
         +-------------------+-------------------+
         |                                       |
 +-------v-------+       +-----------+       +---v---+---+
 |   DASHBOARD   |       |  OBD-II   |       |  LOGGER   |
 |  (Consumer)   |       |  MODULE   |       | (Listener)|
 +---------------+       +-----------+       +-----------+
```

## 2. Component Breakdown

### CAN Bus Layer (`can.c`)
- **Shared Memory**: A circular buffer acts as the electrical bus.
- **Synchronization**: Uses Windows Critical Sections to prevent race conditions during multi-threaded access.
- **Broadcast**: Every message sent is stored and available for any module to read.

### ECUs (`engine_ecu.c`, `brake_ecu.c`)
- These modules simulate physical hardware logic.
- They generate periodic messages containing telemetry data.

### Dashboard (`dashboard.c`)
- Acts as a "Gateway" or "Cluster".
- Filters incoming CAN IDs to update internal vehicle state.
- Renders the state to the console using ASCII bars.

### OBD-II Module (`obd2.c`)
- Responds to specific diagnostic IDs (0x7DF).
- Implements the request-response pattern typical in automotive diagnostics.

## 3. Threading Model
The current implementation uses a single-threaded high-frequency loop in `menu.c` to simulate parallel ECU execution. This ensures maximum compatibility and simplicity while maintaining realistic timing intervals.

## 4. Memory Management
- **Static Allocation**: Most state is statically allocated to mimic embedded system constraints.
- **No Dynamic Heap**: Avoids `malloc` to prevent memory leaks and ensure predictable performance.
