# Aion SDK

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Standard: C++20](https://img.shields.io/badge/Standard-C%2B%2B20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Graphics: OpenGL 3.3 Core](https://img.shields.io/badge/Graphics-OpenGL%203.3%20Core-green.svg)](https://www.khronos.org/opengl/)

**Aion** is a modular, general-purpose 3D engine and framework written in standard C++20 using OpenGL 3.3 Core Profile for rendering. It provides an efficient foundation for 3D simulations, real-time graphics applications, and game development with a clean application lifecycle, hierarchical scene graph, component architecture, glTF/GLB asset importing, and modern CMake integration.

This repository distributes the public Aion Software Development Kit (SDK), including public C++ headers, prebuilt static libraries, CMake package configurations, runtime shaders, comprehensive documentation, and external consumer examples.

---

## Supported Features

The Aion SDK provides only verified, fully implemented functionality:

- **Application Lifecycle:** Clean lifecycle architecture (`Aion::Application`) supporting `OnCreate()`, `OnUpdate()`, `OnRender()`, `OnEvent()`, and `OnDestroy()`, with configurable window dimensions, titles, and VSync.
- **Hierarchical Scene Graph:** Parent-child transform hierarchies (`Aion::Object3D`, `Aion::Transform`) with automatic local and world matrix propagation, quaternion rotations, and bounding volume computation.
- **Component Model:** Extensible component architecture including `CameraComponent`, `CameraControllerComponent`, `MeshRenderer`, `ModelComponent`, `AnimationComponent`, and `RotatorComponent`.
- **Renderer Subsystem:** Forward rendering using OpenGL 3.3 Core Profile with depth testing, viewport adaptation, render state guards, and frustum culling.
- **Camera System:** Perspective and orthographic cameras (`PerspectiveCamera`, `OrthographicCamera`) with configurable field of view, near/far planes, and interactive first-person fly camera controls.
- **Procedural Geometry:** Built-in primitive mesh generation via `MeshFactory` (Cube, Plane, Sphere, Capsule).
- **Materials & Shaders:** PBR-inspired material parameters (base color, metallic, roughness, emissive) and shader management with built-in GLSL 330 standard shaders and embedded C++ shader fallbacks.
- **Image-Based Lighting (IBL):** Procedural sky environment generation and diffuse/specular ambient irradiance convolution.
- **Asset Pipeline:** glTF 2.0 and binary GLB model loading via TinyGLTF, skeletal animation playback and clip blending, 2D texture loading via stb_image, and Virtual File System (VFS) catalog support.
- **Windowing & Input:** Cross-platform window creation, keyboard state, mouse positioning, mouse buttons, and scroll delta tracking via GLFW.
- **Modern CMake Export:** Native CMake package configuration with imported targets (`Aion::AionEngine`), automatic dependency linking, and Semantic Versioning compatibility.

---

## Supported Platform & Compiler

- **Windows (x86_64):** Supported with Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 using `/std:c++20`.
- **Linux:** Linux SDK distribution is out of scope and not supported for the initial public release.

---

## Graphics API & Technical Limitations

- **Graphics API:** OpenGL 3.3 Core Profile.
- **Rendering Architecture:** Single-pass forward rendering.
- **Skeletal Skinning Limit:** Skeletal animation shaders support up to 128 joints per model. Models exceeding 128 joints will log a diagnostic warning and fallback to unskinned rendering.

---

## SDK Installation Overview

1. Download the unified release archive for Windows x64 from GitHub Releases (e.g. `AionSDK-1.0.0-windows-x86_64.zip`), which contains both Debug and Release libraries in a single distribution.
2. Verify the archive against the official checksums in [`checksums/SHA256SUMS.txt`](checksums/SHA256SUMS.txt) using PowerShell:
   ```powershell
   Get-FileHash -Algorithm SHA256 .\AionSDK-1.0.0-windows-x86_64.zip
   ```
3. Extract the archive into your preferred installation location (e.g., `C:\AionSDK`).
4. In your project's `CMakeLists.txt`, find and link Aion:
   ```cmake
   cmake_minimum_required(VERSION 3.16)
   project(MyGame CXX)

   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   find_package(AionEngine CONFIG REQUIRED)

   add_executable(MyGame main.cpp)
   target_link_libraries(MyGame PRIVATE Aion::AionEngine)
   ```
5. Configure your build by setting `CMAKE_PREFIX_PATH`:
   ```cmd
   cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK
   cmake --build build --config Release
   ```
   *(Or `--config Debug` — CMake automatically links the corresponding Debug or Release libraries).*

---

## Documentation

Detailed guides are available in the [`docs/`](docs/) directory:

- [Getting Started Guide](docs/getting-started.md) — Write and run your first Aion application.
- [Installation Guide](docs/installation.md) — Extract, verify, and understand the SDK package layout.
- [CMake Integration Guide](docs/cmake-integration.md) — CMake project configuration, exported targets, and asset copying.
- [Troubleshooting Guide](docs/troubleshooting.md) — Diagnose and resolve common build and runtime errors.
- [Basic Application Example](examples/basic-application/) — Minimal, standalone reference project consuming the SDK.

---

## Example Project

A minimal, independently buildable application demonstrating SDK consumption is provided in [`examples/basic-application/`](examples/basic-application/).

To build and run the example against an installed SDK:

```cmd
cd examples\basic-application
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK
cmake --build build --config Release
.\build\Release\basic-application.exe
```

---

## License

The Aion SDK is open-source software licensed under the **[MIT License](LICENSE)**.

Third-party libraries distributed with or utilized by the SDK (GLFW, GLAD, GLM, Dear ImGui, nlohmann/json, stb_image, TinyGLTF) retain their respective original licenses and copyrights. Detailed attribution and license texts are provided in **[THIRD-PARTY-NOTICES.txt](THIRD-PARTY-NOTICES.txt)**.
