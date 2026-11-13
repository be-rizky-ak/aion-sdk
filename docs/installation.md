# Aion SDK Installation Guide

This guide details how to install, verify, and set up the Aion SDK 1.2.0 on your development workstation.

---

## 1. Prerequisites

Aion adheres to an **Optional Platform-Toolchain Architecture**: you only need the tools required by the platform you intend to target.

### Windows (x86_64) Native Desktop
- **Operating System:** Windows 10 or Windows 11 (64-bit).
- **Compiler:** Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 with C++20 standard support (`/std:c++20`).
- **Build System:** CMake 3.16 or newer.
- *Neither Emscripten nor Ninja is required for native Windows development.*

### Linux (x86_64) Native Desktop
- **Operating System:** Linux x86_64 (Ubuntu 20.04+, Debian 11+, Fedora 36+, Arch Linux).
- **Compiler:** GCC 10+ or Clang 10+ with standard C++20 support (`-std=c++20`).
- **Build System:** CMake 3.16 or newer.
- **System Libraries:** OpenGL / Mesa development libraries and X11 headers.
  ```bash
  sudo apt-get install -y build-essential cmake libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev
  ```
- *Neither Emscripten nor Ninja is required for native Linux development.*

### WebAssembly (wasm32 / WebGL 2) Target
Required only when compiling downstream applications for the Web:
- **Emscripten SDK (`emsdk`):** Supported versions 3.1.50 through 3.1.64+.
- **Build System:** Ninja (`ninja-build`) and CMake 3.16+.

---

## 2. Obtaining Release Archives

Official SDK release archives are distributed via GitHub Releases. Version 1.2.0 provides unified multi-platform archives containing native desktop, WebAssembly, and shared resources in a single canonical package:

| Archive Filename | Platform / Target | Configuration | Description |
|---|---|---|---|
| `AionSDK-1.2.0-linux-x86_64-Release.tar.gz` | Linux x86_64 | Release | Unified Linux + Web SDK Tarball |
| `AionSDK-1.2.0-linux-x86_64-Release.zip` | Linux x86_64 | Release | Unified Linux + Web SDK Zip Archive |
| `AionSDK-1.2.0-linux-x86_64-Debug.tar.gz` | Linux x86_64 | Debug | Unified Debug SDK Tarball |
| `AionSDK-1.2.0-linux-x86_64-Debug.zip` | Linux x86_64 | Debug | Unified Debug SDK Zip Archive |
| `AionSDK-1.2.0-windows-x86_64.zip` | Windows x86_64 | Debug & Release | Unified Windows + Web SDK Zip Archive |
| `AionSandbox-1.2.0-linux-x86_64-Release.tar.gz` | Linux x86_64 | Release | Standalone prebuilt native Sandbox runtime |

### Verifying Checksums

Verify the integrity of downloaded archives using the SHA-256 checksums in [`checksums/SHA256SUMS.txt`](../checksums/SHA256SUMS.txt).

On Linux:
```bash
sha256sum -c checksums/SHA256SUMS.txt
```

On Windows (PowerShell):
```powershell
Get-FileHash -Algorithm SHA256 .\AionSDK-1.2.0-windows-x86_64.zip
```

Or using Command Prompt (`certutil`):
```cmd
certutil -hashfile AionSDK-1.2.0-windows-x86_64.zip SHA256
```

---

## 3. Extracting the SDK

Extract the archive into your preferred installation directory, for example `/opt/AionSDK` (Linux) or `C:\AionSDK` (Windows):

### On Linux:
```bash
mkdir -p /opt/AionSDK
tar -xzf AionSDK-1.2.0-linux-x86_64-Release.tar.gz -C /opt/AionSDK
```

### On Windows (PowerShell):
```powershell
Expand-Archive -Path AionSDK-1.2.0-windows-x86_64.zip -DestinationPath C:\AionSDK
```

After extraction, the canonical SDK base directory is `/opt/AionSDK/AionSDK` or `C:\AionSDK\AionSDK`.

---

## 4. Unified SDK Package Directory Layout

Aion 1.2.0 establishes a unified, platform-structured distribution architecture (`AionSDK/`):

```text
AionSDK/
├── Windows/                          # Native Windows distribution (when built on Windows)
│   ├── include/                      # Public C++ headers
│   │   ├── Aion/                     # Engine public headers
│   │   ├── GLFW/, glad/, glm/        # Interface headers
│   │   └── tiny_gltf.h, stb/, nlohmann/
│   ├── lib/                          # Static libraries (Debug/Release)
│   └── CMake/                        # CMake target files
├── Linux/                            # Native Linux distribution (when built on Linux)
│   ├── include/                      # Public C++ headers
│   ├── lib/                          # Static libraries (libAionEngine.a, libglad.a, libglfw3.a)
│   └── CMake/                        # CMake target files
├── Web/                              # WebAssembly / WebGL 2 distribution
│   ├── include/                      # C++ headers with GLES3 / WebGL 2 mapping
│   ├── lib/                          # Static WebAssembly library (libAionEngine.a)
│   └── lib/cmake/AionEngine/         # Web target CMake exports and Web.cmake
├── Common/                           # Shared cross-platform assets
│   ├── shaders/                      # Runtime GLSL 330 standard shaders
│   ├── web/shell.html                # Emscripten HTML5 shell template
│   └── Examples/                     # Standalone developer demonstrations
├── Examples/                         # Convenience symlink/directory to Examples
│   └── Sandbox/
│       ├── Source/                   # C++ demo source code
│       ├── Assets/                   # Demonstration textures and shaders
│       ├── CMakeLists.txt            # Standalone build configuration
│       └── Web/                      # Prebuilt WebGL 2 demonstration (ready to host statically)
├── README.md                         # SDK documentation
├── LICENSE                           # MIT License for Aion Engine SDK
├── THIRD-PARTY-NOTICES.txt           # Third-party notices and attribution
├── AionEngineConfig.cmake            # Canonical platform-routing CMake entrypoint
├── AionEngineConfigVersion.cmake     # Target-aware version check
├── AionSDKConfig.cmake               # Alias entrypoint for find_package(AionSDK)
└── AionSDKConfigVersion.cmake
```

### Key Architectural Benefits
- **Single Canonical Root:** External consumers specify `CMAKE_PREFIX_PATH` pointing to `AionSDK/`. The root `AionEngineConfig.cmake` automatically detects whether the target is native Windows, native Linux, or Emscripten WebAssembly, and loads the corresponding platform targets.
- **Bundled Prebuilt WebGL 2 Demo:** Every official SDK distribution includes a prebuilt, ready-to-host WebGL 2 Sandbox export in `Examples/Sandbox/Web/` (`index.html`, `sandbox.js`, `sandbox.wasm`, `sandbox.data`). Developers can test or host the demonstration on any static HTTP server immediately without any build tools.

---

## 5. Next Steps

Proceed to the [CMake Integration Guide](cmake-integration.md) to integrate the extracted SDK into your native or Web CMake project.
