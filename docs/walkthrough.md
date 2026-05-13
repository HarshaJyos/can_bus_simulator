# Beginner's Walkthrough

## 1. Prerequisites
Ensure you have the following installed on your Windows machine:
- **MinGW-w64**: To provide the GCC compiler.
- **CMake**: To manage the build process.

### Verifying Tools
Open your terminal (MinGW64) and type:
```bash
gcc --version
cmake --version
```

## 2. Setting Up the Project
Clone or extract the project to your desired directory.

## 3. Building the Project
Navigate to the project root and execute these commands:
```bash
# Create a build directory
mkdir build

# Move into it
cd build

# Generate build files
cmake .. -G "MinGW Makefiles"

# Compile the code
cmake --build .
```

## 4. Running the Simulator
Run the generated executable:
```bash
./CANBusSimulator.exe
```

## 5. Testing Scenarios

### Scenario A: Real-time Monitoring
1. Select `1` from the main menu.
2. Observe the RPM and Temperature values changing.
3. Note how the "Brake Status" changes periodically.
4. Press `Ctrl+C` to stop.

### Scenario B: Custom Injection
1. Select `2` from the main menu.
2. ID: `123`, DLC: `2`, Data: `AA BB`.
3. Check the logs in the next step.

### Scenario C: Log Verification
1. Exit the program or minimize it.
2. Open `logs/can_bus.log` in Notepad.
3. You will see every frame sent during the simulation with a timestamp.

### Scenario D: Diagnostics
1. Select `4` from the main menu.
2. The system will send an OBD-II request for RPM and wait for the response.
3. Verify the calculated RPM value displayed on screen.
