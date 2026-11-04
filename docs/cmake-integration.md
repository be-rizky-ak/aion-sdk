# Integrating Aion SDK with CMake

This guide explains how to integrate the Aion SDK into an external C++ project using CMake.

---

## 1. Requirements

- **CMake:** Version 3.16 or newer.
- **C++ Standard:** Standard C++20 is strictly required (`set(CMAKE_CXX_STANDARD 20)`).
- **Target Platform:** Windows x64 (`windows-x86_64`) with Microsoft Visual C++ (MSVC from Visual Studio 2019 or Visual Studio 2022). Linux SDK distribution is out of scope and not supported for the initial public release.

---

## 2. CMakeLists.txt Configuration

Aion exports a standard CMake configuration package named `AionEngine`.

In your project's `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyAionGame CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the installed AionEngine package
find_package(AionEngine CONFIG REQUIRED)

# Define your executable
add_executable(MyAionGame
    src/main.cpp
)

# Link against the imported AionEngine static library
target_link_libraries(MyAionGame PRIVATE Aion::AionEngine)

# Optional: Copy runtime shaders from the SDK to your executable directory
add_custom_command(
    TARGET MyAionGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:MyAionGame>/assets/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${AionEngine_DIR}/../../../share/Aion/shaders/standard.vert"
        "${AionEngine_DIR}/../../../share/Aion/shaders/standard.frag"
        "$<TARGET_FILE_DIR:MyAionGame>/assets/shaders/"
    COMMENT "Copying Aion runtime standard shaders to output assets directory..."
)
```

---

## 3. Configuring and Building with `CMAKE_PREFIX_PATH`

Point CMake to the extracted unified `AionSDK` directory using `CMAKE_PREFIX_PATH`:

```cmd
:: Configure the project pointing to the unified SDK
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK

:: Build the project in Release configuration (automatically links lib/Release/)
cmake --build build --config Release

:: Or build in Debug configuration (automatically links lib/Debug/)
cmake --build build --config Debug
```

For example, if the SDK was extracted to `C:\AionSDK\AionSDK`:
```cmd
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK/AionSDK
cmake --build build --config Release
```

---

## 4. Exported CMake Targets & Configuration Mapping

The `AionEngine` package exports the following CMake targets under the `Aion::` namespace:

| Target | Type | Description |
|---|---|---|
| `Aion::AionEngine` | `STATIC IMPORTED` | The primary engine target. Links `Aion::glad`, `Aion::glfw`, and `OpenGL::GL`. |
| `Aion::glad` | `STATIC IMPORTED` | Vendored OpenGL 3.3 Core GLAD loader. |
| `Aion::glfw` | `STATIC IMPORTED` | Windowing and input library. |

In typical application development, you only need to link `Aion::AionEngine`. Its interface dependencies (`glad`, `glfw`, system `Threads`, and `OpenGL`) are linked transitively.

### Automatic Configuration Selection & Runtime Compatibility

The unified Windows SDK distributes prebuilt static libraries for both `Debug` and `Release` configurations:
- **Debug Configuration:** When building with `Debug` (e.g. `--config Debug` or Visual Studio Debug configuration), CMake automatically links against `lib/Debug/` (`AionEngine.lib`, `glad.lib`, `glfw3.lib`), which are compiled with the MSVC Debug runtime (`/MDd`).
- **Release Configuration:** When building with `Release` (e.g. `--config Release` or Visual Studio Release configuration), CMake automatically links against `lib/Release/` (`AionEngine.lib`, `glad.lib`, `glfw3.lib`), which are compiled with the MSVC Release runtime (`/MD`).
- **`RelWithDebInfo` and `MinSizeRel` Mapping:** The exported targets configure `MAP_IMPORTED_CONFIG_RELWITHDEBINFO` and `MAP_IMPORTED_CONFIG_MINSIZEREL` to `Release`. Because `RelWithDebInfo` and `MinSizeRel` share the standard non-debug MSVC runtime (`/MD`), they safely resolve to `Release` static libraries without silent fallback to incompatible Debug libraries (`/MDd`).
- External consumers do not need to manually configure library paths or select individual `.lib` files.

---

## 5. Header Include Conventions

All engine headers are installed under the `Aion/` prefix within the SDK `include/` directory:

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

// Renderer & Materials
#include "Aion/Renderer/Renderer.h"
#include "Aion/Renderer/Material.h"
#include "Aion/Assets/MeshFactory.h"

// Math
#include "Aion/Math/Vector3.h"
#include "Aion/Math/Matrix4.h"
#include "Aion/Math/Quaternion.h"
```

Third-party public headers included in the SDK (`glad/glad.h`, `GLFW/glfw3.h`, `glm/glm.hpp`, `nlohmann/json.hpp`, `stb/stb_image.h`, `tiny_gltf.h`) can be included directly.

---

## 6. Runtime Asset and Shader Management

- **Built-in Shaders:** The engine bundles runtime shader files at `share/Aion/shaders/` (`standard.vert` and `standard.frag`).
- **Automatic Fallback:** If `assets/shaders/` is not found at runtime, `ShaderLibrary` falls back automatically to embedded builtin GLSL shaders compiled into `AionEngine.lib`.
- **Custom Shaders:** To provide custom shaders, place them in an `assets/` folder relative to your application's current working directory or executable directory.
