# Testing and Validation

## 1. Unit Testing Strategy
While the project is a monolithic simulation, each module is designed for independent testing.

### CAN Bus Logic Test
- **Action**: Send 100 frames in a loop.
- **Expected**: Statistics should show exactly 100 frames sent and 100 received.
- **Failure**: DLC > 8 should return `false`.

### OBD-II Response Test
- **Action**: Inject ID `0x7DF` with data `02 01 0C`.
- **Expected**: A frame with ID `0x7E8` should appear on the bus containing RPM data.

## 2. Integration Testing
Run the full simulation for 5 minutes.
- **Check**: Memory usage should remain constant (no leaks).
- **Check**: Log file size should grow linearly.
- **Check**: Dashboard should remain responsive.

## 3. Debugging Tips
- If the program crashes, check if the `logs/` directory exists (the program tries to create it, but permissions might vary).
- Use `printf` inside `can_send_frame` to see traffic in real-time if the dashboard is too fast.
- Compile with debug symbols: `cmake .. -DCMAKE_BUILD_TYPE=Debug`.
