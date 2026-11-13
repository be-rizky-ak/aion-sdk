# Integrating Aion SDK with CMake

This guide explains how to integrate the Aion SDK 1.2.0 into an external C++ project using modern CMake for both native desktop (Windows and Linux) and WebAssembly (WebGL 2) targets.

---

## 1. Requirements

### Native Desktop Development (Windows & Linux)
- **CMake:** Version 3.16 or newer.
- **C++ Standard:** Standard C++20 strictly required (`set(CMAKE_CXX_STANDARD 20)`).
- **Compilers:**
  - Windows: MSVC from Visual Studio 2019/2022 (`/std:c++20`).
  - Linux: GCC 10+ or Clang 10+ (`-std=c++20`).
- *Neither Emscripten nor Ninja is required for native desktop development.*

### WebAssembly Target (Web / WebGL 2)
Required only when compiling for the browser:
- **Emscripten SDK (`emsdk`):** Supported versions 3.1.50 through 3.1.64+.
- **Build System:** Ninja (`-G Ninja`) and CMake 3.16+.

---

## 2. Project `CMakeLists.txt` Setup

The Aion SDK exports a standard CMake configuration package named `AionEngine` (also aliased as `AionSDK`).

Here is a complete, multi-platform `CMakeLists.txt` supporting both native desktop and Web targets:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyAionGame CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the installed AionEngine package from the unified SDK
find_package(AionEngine CONFIG REQUIRED)

# Define application executable
add_executable(MyAionGame
    src/main.cpp
)

# Link against the imported AionEngine target
target_link_libraries(MyAionGame PRIVATE Aion::AionEngine)

# WebAssembly (Emscripten) configuration
if(EMSCRIPTEN)
    # Configure Web executable with canvas HTML shell, VFS preloaded assets, and memory limit
    aion_configure_web_executable(MyAionGame
        ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        INITIAL_MEMORY 128MB
    )
else()
    # Native desktop: copy runtime standard shaders to output assets directory
    add_custom_command(
        TARGET MyAionGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:MyAionGame>/assets/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${AION_COMMON_DIR}/shaders/standard.vert"
            "${AION_COMMON_DIR}/shaders/standard.frag"
            "$<TARGET_FILE_DIR:MyAionGame>/assets/shaders/"
        COMMENT "Copying Aion runtime standard shaders to output assets directory..."
    )
endif()
```

---

## 3. Configuring and Building

### 3.1 Native Desktop Build (Windows & Linux)

Point `CMAKE_PREFIX_PATH` to the extracted unified `AionSDK` directory:

#### On Linux:
```bash
# Configure
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/AionSDK/AionSDK -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Run
./build/MyAionGame
```

#### On Windows:
```cmd
:: Configure pointing to the unified SDK
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK/AionSDK

:: Build in Release configuration (automatically links lib/Release/)
cmake --build build --config Release

:: Or build in Debug configuration (automatically links lib/Debug/)
cmake --build build --config Debug

:: Run
.\build\Release\MyAionGame.exe
```

---

### 3.2 WebAssembly Build (Emscripten)

When compiling for WebAssembly using Emscripten, use `emcmake cmake` with the Ninja build generator.

#### Crucial Flag: `-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH`
Emscripten toolchain files default `CMAKE_FIND_ROOT_PATH_MODE_PACKAGE` to `ONLY`, restricting `find_package` lookups exclusively to the Emscripten sysroot. Because the Aion SDK is an external package residing on the host filesystem, passing `-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH` is required so CMake searches host directories in `CMAKE_PREFIX_PATH` alongside the sysroot.

```bash
# 1. Activate Emscripten environment
source ~/emsdk/emsdk_env.sh

# 2. Configure with emcmake pointing to the same unified SDK root
emcmake cmake -S . -B build_web \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH \
    -DCMAKE_PREFIX_PATH=/opt/AionSDK/AionSDK

# 3. Build static web deliverables
cmake --build build_web
```

On Windows PowerShell:
```powershell
# Activate EMSDK
& $env:USERPROFILE\emsdk\emsdk_env.ps1

# Configure
emcmake cmake -S . -B build_web `
    -G Ninja `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH `
    -DCMAKE_PREFIX_PATH=C:/AionSDK/AionSDK

# Build
cmake --build build_web
```

The build outputs `MyAionGame.html`, `MyAionGame.js`, `MyAionGame.wasm`, and `MyAionGame.data` in `build_web/`.

---

## 4. The `aion_configure_web_executable` Macro

When `EMSCRIPTEN` is detected, the SDK automatically loads `cmake/platform/Web.cmake` which provides the helper function `aion_configure_web_executable`:

```cmake
aion_configure_web_executable(TARGET_NAME
    [SHELL_FILE <path-to-shell.html>]
    [ASSETS_DIR <path-to-assets-directory>]
    [INITIAL_MEMORY <size>]
    [EXTRA_PRELOAD_FILES <file1> <file2> ...]
)
```

### Parameters
- **`TARGET_NAME`** (Required): The CMake executable target to configure. Sets target suffix to `.html`.
- **`SHELL_FILE`** (Optional): Path to a custom HTML shell template. If omitted, the helper checks `${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html` or falls back to the bundled SDK shell template (`Common/web/shell.html`).
- **`ASSETS_DIR`** (Optional): Path to the application assets directory to preload into Emscripten's Virtual File System (VFS) at `/assets` via `--preload-file`. Defaults to `${CMAKE_CURRENT_SOURCE_DIR}/assets` if it exists.
- **`INITIAL_MEMORY`** (Optional): Initial WebAssembly memory allocation (e.g. `128MB`, `256MB`). Defaults to `128MB`. `-sALLOW_MEMORY_GROWTH=1` is enabled automatically.
- **`EXTRA_PRELOAD_FILES`** (Optional): List of additional file or directory paths to preload into the virtual filesystem.

### Link Flags Configured by Macro
- `-sUSE_GLFW=3`: Enables Emscripten's GLFW 3 implementation.
- `-sMAX_WEBGL_VERSION=2` & `-sMIN_WEBGL_VERSION=2`: Enforces WebGL 2 (OpenGL ES 3.0) context creation.
- `-sFULL_ES3=1`: Enables full OpenGL ES 3.0 emulation.
- `-sALLOW_MEMORY_GROWTH=1`: Allows dynamic WebAssembly heap resizing as required by asset loading.

---

## 5. Exported CMake Targets & Platform Routing

The SDK root configuration (`AionEngineConfig.cmake`) identifies the active toolchain and automatically resolves the correct platform directory:

```text
AionSDK/
├── Windows/  <-- Resolved for MSVC / Windows builds
├── Linux/    <-- Resolved for GCC/Clang Linux builds
└── Web/      <-- Resolved when EMSCRIPTEN is defined
```

Exported CMake targets under the `Aion::` namespace:

| Target | Target Type | Supported Platforms | Description |
|---|---|---|---|
| `Aion::AionEngine` | `STATIC IMPORTED` | Windows, Linux, Web | Primary engine static library. Automatically resolves platform dependencies. |
| `Aion::glad` | `STATIC IMPORTED` | Windows, Linux | Vendored GLAD OpenGL loader. (Emulated internally under Web). |
| `Aion::glfw` | `STATIC IMPORTED` | Windows, Linux | GLFW 3 windowing and input library. |

In typical application development, you only need to link `Aion::AionEngine`. All required transitive dependencies (`Threads`, `OpenGL`, `glfw`, `glad`) are linked automatically.

### Automatic Configuration Selection on Windows (MSVC)
The unified Windows SDK distributes prebuilt static libraries for both `Debug` and `Release` configurations:
- **Debug Configuration:** Links against `lib/Debug/` (`AionEngine.lib`, `glad.lib`, `glfw3.lib`) compiled with `/MDd`.
- **Release Configuration:** Links against `lib/Release/` compiled with `/MD`.
- **`RelWithDebInfo` and `MinSizeRel`:** Safely map to `Release` static libraries via `MAP_IMPORTED_CONFIG`, avoiding runtime incompatibility (`/MDd` vs `/MD`).

---

## 6. Header Include Conventions

All engine headers are accessible under the `Aion/` prefix within the SDK `include/` directory:

```cpp
// Core Subsystems
#include "Aion/Core/Application.h"
#include "Aion/Core/Window.h"
#include "Aion/Core/Input.h"
#include "Aion/Core/Event.h"

// Scene & Hierarchy
#include "Aion/Scene/Scene.h"
#include "Aion/Scene/Object3D.h"
#include "Aion/Scene/Transform.h"
#include "Aion/Scene/MeshRenderer.h"
#include "Aion/Scene/PerspectiveCamera.h"
#include "Aion/Scene/CameraControllerComponent.h"

// Audio & Concurrency
#include "Aion/Audio/AudioSystem.h"
#include "Aion/Audio/AudioClip.h"
#include "Aion/Audio/AudioSource.h"
#include "Aion/Core/TaskSystem.h"

// Renderer & Materials
#include "Aion/Renderer/Renderer.h"
#include "Aion/Renderer/Material.h"
#include "Aion/Assets/MeshFactory.h"

// Math
#include "Aion/Math/Vector3.h"
#include "Aion/Math/Matrix4.h"
#include "Aion/Math/Quaternion.h"
```

Third-party public headers (`glad/glad.h`, `GLFW/glfw3.h`, `glm/glm.hpp`, `nlohmann/json.hpp`, `stb/stb_image.h`, `tiny_gltf.h`) can be included directly.

---

## 7. Runtime Asset and Shader Management

- **Built-in Shaders:** Standard GLSL shaders reside at `Common/shaders/` (`standard.vert` and `standard.frag`).
- **WebGL 2 Automatic Adaptation:** Shaders are automatically converted at runtime to GLSL ES 3.00 (`#version 300 es`) with appropriate precision qualifiers.
- **Embedded Fallback:** If external shaders are not found on disk or in the VFS at runtime, `ShaderLibrary` falls back automatically to embedded builtin GLSL shaders compiled into the engine binary.
- **Custom Assets:** Place custom shaders or 3D models under an `assets/` directory. On Web, specify `ASSETS_DIR` in `aion_configure_web_executable` to preload them into Emscripten's virtual filesystem.
