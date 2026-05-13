# Advanced Walkthrough (v2.0)

## 1. Environment Setup
- **Linux**: Install `can-utils` and `libpthread-stubs0-dev`.
- **Windows**: Use MinGW64 and ensure `pthread` is in your compiler path.

## 2. Compiling the Advanced Project
```bash
cd /c/dev/can_Simulator
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

## 3. Running Unit Tests
Before running the simulator, verify the core logic:
```bash
./can_tests.exe
```
This will test frame creation and the **DBC Decoding Engine**.

## 4. Running the Simulator
```bash
./CANBusSimulator.exe
```

## 5. What to look for
- **Concurrency**: Notice that the dashboard updates even while you are in the middle of typing a custom frame ID. This is because ECUs are running in background threads.
- **DBC Decoding**: The Dashboard now uses `dbc_decode()` to pull signals from raw bits, mimicking how real automotive displays work.
- **Logging**: Check `logs/can_bus.log` to see the TX/RX history of all threads.
