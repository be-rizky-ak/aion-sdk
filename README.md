# Aion SDK

[![Version: 1.2.0](https://img.shields.io/badge/Version-1.2.0-blue.svg)](CHANGELOG.md)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Standard: C++20](https://img.shields.io/badge/Standard-C%2B%2B20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Graphics: OpenGL 3.3 Core](https://img.shields.io/badge/Graphics-OpenGL%203.3%20Core-green.svg)](https://www.khronos.org/opengl/)
[![Graphics: WebGL 2](https://img.shields.io/badge/Graphics-WebGL%202-orange.svg)](https://www.khronos.org/webgl/)
[![Platforms: Windows | Linux | Web](https://img.shields.io/badge/Platforms-Windows%20%7C%20Linux%20%7C%20Web-lightgrey.svg)](#supported-platforms--compilers)

**Aion** is a modular, general-purpose 3D engine and framework written in standard C++20 supporting OpenGL 3.3 Core Profile for desktop rendering and WebGL 2 (OpenGL ES 3.0) for WebAssembly. It provides an efficient foundation for 3D simulations, real-time graphics applications, and game development with a clean application lifecycle, hierarchical scene graph, component architecture, glTF/GLB asset importing, multi-threaded task system, reusable audio subsystem, standalone web export automation, and modern CMake integration.

This repository distributes the public Aion Software Development Kit (SDK), including public C++ headers, prebuilt static libraries, CMake package configurations, runtime shaders, comprehensive documentation, and external consumer examples.

---

## Supported Features

The Aion SDK provides only verified, fully implemented functionality:

- **Application Lifecycle:** Clean lifecycle architecture (`Aion::Application`) supporting `OnCreate()`, `OnUpdate()`, `OnRender()`, `OnEvent()`, and `OnDestroy()`, with configurable window dimensions, titles, VSync, and console logging control.
- **Hierarchical Scene Graph:** Parent-child transform hierarchies (`Aion::Object3D`, `Aion::Transform`) with automatic local and world matrix propagation, quaternion rotations, and bounding volume computation.
- **Component Model:** Extensible component architecture including `CameraComponent`, `CameraControllerComponent`, `MeshRenderer`, `ModelComponent`, `AnimationComponent`, `AudioSource`, `AudioListener`, and `RotatorComponent`.
- **Audio Subsystem:** High-performance, low-latency audio engine (`Aion::AudioSystem`, `AudioClip`, `AudioSource`, `AudioListener`, `AudioCategory`) supporting WAV, MP3, and OGG Vorbis playback, 3D spatial positioning, independent volume categories (`Master`, `Music`, `SFX`), and one-shot playback.
- **Task System:** Built-in multi-threaded worker pool (`Aion::TaskSystem`) with task synchronization and `std::future` tracking (native desktop platforms).
- **Renderer Subsystem:** Forward rendering using OpenGL 3.3 Core Profile (desktop) and WebGL 2 / OpenGL ES 3.0 (Web) with depth testing, viewport adaptation, render state guards, frustum culling, and equirectangular skybox rendering.
- **Camera System:** Perspective and orthographic cameras (`PerspectiveCamera`, `OrthographicCamera`) with configurable field of view, near/far planes, and interactive first-person fly camera controls.
- **Procedural Geometry:** Built-in primitive mesh generation via `MeshFactory` (Cube, Plane, Sphere, Capsule).
- **Materials & Shaders:** PBR-inspired material parameters (base color, metallic, roughness, emissive) and shader management with built-in standard shaders, runtime WebGL 2 shader adaptation (`AdaptShaderSourceForWebGL2`), and embedded C++ shader fallbacks.
- **Image-Based Lighting (IBL):** Procedural sky environment generation and diffuse/specular ambient irradiance convolution.
- **Asset Pipeline:** glTF 2.0 and binary GLB model loading via TinyGLTF, skeletal animation playback and clip blending, 2D texture loading via stb_image, SVG vector rasterization, and Virtual File System (VFS) catalog support with Emscripten asset preloading (`--preload-file`).
- **WebAssembly & WebGL 2 Support:** Non-blocking browser main loop via `emscripten_set_main_loop_arg`, GLSL 330 core to GLSL ES 3.00 shader translation, depth texture clamp adaptation to `GL_CLAMP_TO_EDGE`, shadow map filtering, and main-thread synchronous asset parsing fallback avoiding `SharedArrayBuffer` / COOP / COEP requirements.
- **Web Export Automation:** Standalone export scripts (`build_web.sh` for Linux/macOS and `build_web.ps1` for Windows) generating complete, ready-to-deploy static web packages (`index.html`, `sandbox.js`, `sandbox.wasm`, `sandbox.data`, and `export_manifest.json`).
- **Optional Platform-Toolchain Architecture:** Native desktop development requires zero Web tools (neither Emscripten nor Ninja is required). Web tools are only required when explicitly compiling for WebAssembly.
- **Unified SDK Distribution Architecture:** Consolidated SDK distribution layout (`dist/sdk/AionSDK/`) housing platform distributions (`Windows/`, `Linux/`, `Web/`, `Common/`) and bundling a prebuilt, hostable WebGL 2 demonstration (`Examples/Sandbox/Web/`).
- **Windowing & Input:** Cross-platform window creation, keyboard state, mouse positioning, mouse buttons, and scroll delta tracking via GLFW and HTML5 canvas event hooks.
- **Modern CMake Export:** Target-aware CMake discovery via `AionEngineConfig.cmake`, automatic platform routing, and helper macros (`aion_configure_web_executable`).

---

## Supported Platforms & Compilers

- **Windows (x86_64):** Supported with Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 using `/std:c++20`.
- **Linux (x86_64):** Supported with GCC 10+ or Clang 10+ using `-std=c++20`.
- **WebAssembly (wasm32):** Supported with Emscripten SDK (`emsdk` versions 3.1.50 through 3.1.64+) using `-std=c++20` and Ninja.

---

## Graphics API & Technical Limitations

- **Graphics APIs:** OpenGL 3.3 Core Profile (Desktop) and WebGL 2 / OpenGL ES 3.0 (WebAssembly).
- **Rendering Architecture:** Single-pass forward rendering.
- **Web Single-Threaded Execution:** WebAssembly builds run on the browser's main thread to avoid cross-origin isolation (`SharedArrayBuffer` / COOP / COEP) server requirements.
- **Skeletal Skinning Limit:** Skeletal animation shaders support up to 128 joints per model across desktop and Web targets. Models exceeding 128 joints log a diagnostic warning and fallback to unskinned rendering.
- **Browser Compatibility:** Web applications require a modern browser supporting WebGL 2 (Google Chrome, Microsoft Edge, Mozilla Firefox, Apple Safari 15+).

---

## SDK Installation Overview

1. Download the unified release archive for your platform from GitHub Releases (e.g. `AionSDK-1.2.0-linux-x86_64-Release.tar.gz` or `AionSDK-1.2.0-windows-x86_64.zip`), which contains native desktop and Web platform distributions in a single unified package.
2. Verify the archive against official checksums in [`checksums/SHA256SUMS.txt`](checksums/SHA256SUMS.txt):
   - Linux:
     ```bash
     sha256sum -c checksums/SHA256SUMS.txt
     ```
   - Windows (PowerShell):
     ```powershell
     Get-FileHash -Algorithm SHA256 .\AionSDK-1.2.0-windows-x86_64.zip
     ```
3. Extract the archive into your preferred installation directory (e.g., `C:\AionSDK` or `/opt/AionSDK`).
4. In your project's `CMakeLists.txt`, find and link Aion:
   ```cmake
   cmake_minimum_required(VERSION 3.16)
   project(MyGame CXX)

   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   find_package(AionEngine CONFIG REQUIRED)

   add_executable(MyGame main.cpp)
   target_link_libraries(MyGame PRIVATE Aion::AionEngine)

   if(EMSCRIPTEN)
       aion_configure_web_executable(MyGame
           ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets"
           INITIAL_MEMORY 128MB
       )
   endif()
   ```
5. Configure and build your project by pointing `CMAKE_PREFIX_PATH` to the unified SDK directory:
   - **Native Desktop:**
     ```bash
     cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/AionSDK
     cmake --build build --config Release
     ```
   - **WebAssembly (Emscripten):**
     ```bash
     emcmake cmake -S . -B build_web -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH -DCMAKE_PREFIX_PATH=/path/to/AionSDK
     cmake --build build_web
     ```

---

## Documentation

Detailed guides are available in the [`docs/`](docs/) directory:

- [Release Notes 1.2.0](RELEASE_NOTES_1.2.0.md) — Comprehensive 1.2.0 release summary, WebGL 2 capabilities, and migration guide.
- [Release Notes 1.1.0](RELEASE_NOTES_1.1.0.md) — 1.1.0 release notes covering audio and task subsystems.
- [Getting Started Guide](docs/getting-started.md) — Write and run your first Aion application.
- [Installation Guide](docs/installation.md) — Extract, verify, and understand the unified SDK package layout.
- [CMake Integration Guide](docs/cmake-integration.md) — CMake project configuration, exported targets, Emscripten options, and asset copying.
- [Troubleshooting Guide](docs/troubleshooting.md) — Diagnose and resolve common native and Web build/runtime errors.
- [Basic Application Example](examples/basic-application/) — Minimal, standalone reference project consuming the SDK.

---

## Example Project

A minimal, independently buildable application demonstrating SDK consumption is provided in [`examples/basic-application/`](examples/basic-application/).

To build and run the example against an installed SDK:

- **Linux:**
  ```bash
  cd examples/basic-application
  cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/AionSDK -DCMAKE_BUILD_TYPE=Release
  cmake --build build
  ./build/basic-application
  ```

- **Windows:**
  ```cmd
  cd examples\basic-application
  cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK
  cmake --build build --config Release
  .\build\Release\basic-application.exe
  ```

---

## License

The Aion SDK is open-source software licensed under the **[MIT License](LICENSE)**.

Third-party libraries distributed with or utilized by the SDK (GLFW, GLAD, GLM, Dear ImGui, nlohmann/json, stb_image, TinyGLTF, miniaudio) retain their respective original licenses and copyrights. Detailed attribution and license texts are provided in **[THIRD-PARTY-NOTICES.txt](THIRD-PARTY-NOTICES.txt)**.
