# Aion SDK Basic Application Example

This directory contains a minimal standalone reference application demonstrating how to consume the public Aion SDK 1.2.0 from an external CMake project.

---

## 1. Prerequisites

- Extracted unified Aion SDK 1.2.0 distribution directory (`AionSDK/`).
- **Operating Systems:**
  - **Linux:** 64-bit Linux (Ubuntu 20.04+, Debian 11+, Fedora 36+, Arch Linux) with GCC 10+ or Clang 10+ (`-std=c++20`).
  - **Windows:** 64-bit Windows 10/11 with MSVC from Visual Studio 2019/2022 (`/std:c++20`).
- **Build System:** CMake 3.16 or newer.

---

## 2. Building the Example

From this directory:

### On Linux:
```bash
# 1. Configure CMake pointing to the installed unified SDK prefix path
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/AionSDK -DCMAKE_BUILD_TYPE=Release

# 2. Build the executable
cmake --build build

# 3. Run the executable
./build/basic-application
```

### On Windows:
```cmd
:: 1. Configure CMake pointing to the installed unified SDK prefix path
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/path/to/AionSDK

:: 2. Build the executable in Release configuration (links lib/Release/ libraries)
cmake --build build --config Release

:: Or build in Debug configuration (links lib/Debug/ libraries)
cmake --build build --config Debug

:: 3. Run the executable
.\build\Release\basic-application.exe
```

For example, if you extracted `AionSDK-1.2.0-windows-x86_64.zip` to `C:\AionSDK\AionSDK`:
```cmd
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK/AionSDK
cmake --build build --config Release
```

---

## 3. Running the Example

### Interactive Mode:
- **Linux:** `./build/basic-application`
- **Windows:** `.\build\Release\basic-application.exe`

### Automated Test Mode:
To run non-interactively for headless verification:
- **Linux:**
  ```bash
  xvfb-run -a ./build/basic-application --test
  ```
- **Windows:**
  ```cmd
  .\build\Release\basic-application.exe --test
  ```

In test mode (`--test`), the application initializes the graphics context, sets up the camera and primitive mesh in the scene, executes two simulation and render frames, and exits cleanly with return code 0.
