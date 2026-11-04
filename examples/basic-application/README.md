# Aion SDK Basic Application Example

This directory contains a minimal standalone application demonstrating how to consume the public Aion SDK from an external CMake project on Windows x64 with MSVC.

---

## 1. Prerequisites

- Extracted Aion SDK distribution directory (containing `include/`, `lib/`, `share/`).
- **Operating System:** Windows 10 or Windows 11 (64-bit).
- **Compiler:** Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 with C++20 support (`/std:c++20`).
- **Build System:** CMake 3.16 or newer.

> **Note:** Linux SDK distribution is out of scope and not supported for the initial public release.

---

## 2. Building the Example

From this directory:

```cmd
:: 1. Configure CMake pointing to the installed unified SDK prefix path
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/path/to/AionSDK

:: 2. Build the executable in Release configuration (links lib/Release/ libraries)
cmake --build build --config Release

:: Or build the executable in Debug configuration (links lib/Debug/ libraries)
cmake --build build --config Debug
```

For example, if you extracted `AionSDK-1.0.0-windows-x86_64.zip` to `C:\AionSDK\AionSDK`:

```cmd
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK/AionSDK
cmake --build build --config Release
```

---

## 3. Running the Example

### Interactive Mode:
```cmd
:: Run Release build
.\build\Release\basic-application.exe

:: Or run Debug build
.\build\Debug\basic-application.exe
```

### Automated Test Mode:
To run non-interactively for verification:
```cmd
:: Verify Release build
.\build\Release\basic-application.exe --test

:: Verify Debug build
.\build\Debug\basic-application.exe --test
```
In test mode (`--test`), the application initializes the OpenGL context, sets up the camera and primitive mesh in the scene, executes two frames, and exits cleanly with return code 0.
