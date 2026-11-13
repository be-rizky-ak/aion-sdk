# Changelog

All notable changes to the Aion SDK will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.2.0] - 2026-11-12

### Added
- **WebAssembly & WebGL 2 Platform Support**:
  - Full support for compiling Aion Engine and applications to WebAssembly via Emscripten and rendering via WebGL 2 (OpenGL ES 3.0).
  - Emscripten OpenGL header mapping using `<GLES3/gl3.h>` and GLAD emulation when targeting Web.
  - Non-blocking browser main loop integration via `emscripten_set_main_loop_arg` in `Application::Run()`.
  - Automated shader adaptation (`AdaptShaderSourceForWebGL2`) converting GLSL 330 core shaders to GLSL ES 3.00 with precision qualifiers.
  - WebGL 2 rendering pipeline adjustments (graceful skipping of desktop-only `glPolygonMode` wireframe rendering, fallback from `glDrawBuffer` to `glDrawBuffers`, texture clamp adaptation to `GL_CLAMP_TO_EDGE` for depth textures, and shadow map filtering compatibility).
  - Synchronous main-thread model loading fallback for `AssetManager::LoadModelAsync` under Web to comply with single-threaded browser environments without `SharedArrayBuffer`.
  - Emscripten Virtual File System (VFS) integration with asset preloading (`--preload-file`).
- **Web Export Workflow & Automation Scripts**:
  - Cross-platform web build and packaging scripts: `build_web.sh` (Linux/macOS) and `build_web.ps1` (Windows PowerShell).
  - Automatic toolchain detection and environment configuration for Emscripten SDK (`emsdk`) and Ninja build generator.
  - Isolated web export directory (`build_web/export/`) producing ready-to-host static files (`index.html`, `sandbox.html`, `sandbox.js`, `sandbox.wasm`, `sandbox.data`, and `export_manifest.json`).
  - Responsive HTML5 shell template (`web/shell.html`) with WebGL 2 feature detection and interactive loading status indicator.
- **Unified SDK Distribution Architecture**:
  - Canonical SDK distribution root: `dist/sdk/AionSDK/`.
  - Structured platform subdirectories: `Windows/`, `Linux/`, `Web/`, and `Common/`.
  - Shared engine shaders, Web shell template, and developer examples housed in `Common/`.
  - Bundled complete prebuilt WebGL 2 Sandbox demonstration in `Examples/Sandbox/Web/`, enabling static web hosting without any compiler or SDK tools installed.
- **Cross-Platform CMake Package Integration**:
  - Target-aware CMake discovery via `AionEngineConfig.cmake` and `AionEngineConfigVersion.cmake` at the SDK root.
  - Automatic platform routing: native consumers link native static libraries (`AionEngine`, `glad`, `glfw`), while Emscripten consumers link the Web target.
  - Dedicated CMake helper module `cmake/platform/Web.cmake` providing `aion_configure_web_executable` for external downstream projects compiling to WebAssembly.

### Improvements
- **Optional Platform-Toolchain Architecture**:
  - Strict decoupling of native and Web development requirements.
  - Native Windows and Linux development workflows require zero Web tools (neither Emscripten nor Ninja is needed).
  - Packaging scripts support `--native-only` flag to build and stage native SDK packages cleanly without Web toolchains present.
  - Missing Web tools produce clear, actionable diagnostic messages only when Web export or packaging is requested.
- **CMake & Packaging Verification**:
  - Robust platform compatibility enforcement in SDK version checks to prevent accidental mixing of native and Web SDK distributions.
  - Packaging scripts (`package.bat`, `package.sh`) automatically clean competing or legacy SDK roots and verify archive checksums, file structure, source path cleanliness, and consumer linking.

### Compatibility and Requirements
- **Native Development Requirements**: Standard C++20 compiler (MSVC 2019/2022 on Windows, GCC 10+/Clang 10+ on Linux) and CMake 3.16+. Emscripten and Ninja are not required.
- **Web Development Requirements**: Emscripten SDK (`emsdk` versions 3.1.50 through 3.1.64+) and Ninja build generator (`-G Ninja`).
- **Backward Compatibility**: Fully backward compatible with Aion SDK 1.1.0 and 1.0.0. All existing native APIs and application code remain compatible without changes.

### Known Limitations
- **Web Single-Threaded Execution**: WebAssembly builds execute on the browser's main thread without multi-threading or worker pools, avoiding `SharedArrayBuffer` cross-origin isolation requirements.
- **Skeletal Skinning Limit**: Skeletal animation shaders support up to 128 joints per model across desktop and Web targets; models exceeding 128 joints fallback to unskinned rendering with a diagnostic warning.

## [1.1.0] - 2026-11-09

### Added
- **Audio Subsystem**:
  - Reusable, low-latency audio subsystem (`Aion::AudioSystem`, `AudioClip`, `AudioSource`, `AudioListener`, `AudioCategory`).
  - Decoding support for WAV, MP3, and OGG Vorbis audio formats via integrated miniaudio and stb_vorbis.
  - Spatial 3D audio listener and positional source components (`AudioListener`, `AudioSource`) with 3D attenuation and directional orientation.
  - Independent volume categories and mixing channels (`Master`, `Music`, `SFX`).
  - Concurrent one-shot sound effect playback (`AudioSystem::PlayOneShot`).
  - Audio clip caching (`AssetManager::LoadAudioClip`) and streaming for background music.
  - Graceful fallback and dummy device handling for headless Linux / CI environments without physical audio hardware.
  - Automated test suite `AudioSystemTest` validating audio subsystem lifecycle and controls.
- **Task System and Concurrency**:
  - Multi-threaded worker pool (`Aion::TaskSystem`) with task queue synchronization, worker thread lifecycle, and `std::future` task tracking.
  - Safe thread shutdown and task completion barriers.
- **Configurable Console Logging**:
  - Centralized diagnostic sink (`Aion::Log`) with build-time and runtime toggling (`AION_ENABLE_CONSOLE_LOG`).
  - Per-configuration defaults: enabled in Debug builds, suppressed in Release builds.
  - Runtime override via `Aion::Log::SetConsoleLoggingEnabled` and `ApplicationSpecification.EnableConsoleLogging`.
- **Animation System Extensions**:
  - Configurable playback speed multiplier (`SetPlaybackSpeed`, `GetPlaybackSpeed`).
  - Playback control and telemetry queries (`Reset()`, `IsPlaying()`, `GetClipNames()`).
- **Asset Pipeline & Importers**:
  - SVG asset rasterization and texture generation (`Aion::SVGImporter`).
  - Standalone 8-bit and floating-point HDR image container abstraction (`Aion::Image`).

### Rendering
- Seamless 360-degree equirectangular skybox mapping and fullscreen quad background rendering pass.
- Aspect ratio (2:1) validation for equirectangular environment textures.
- Render state protection disabling face culling and depth test during cubemap rendering.
- Standalone `RenderCommand` viewport management and `VertexArray` state encapsulation.

### Fixed
- Ensured deterministic cleanup of `TextureLibrary` during renderer shutdown to prevent `glDeleteTextures` segfaults during static destruction after OpenGL context termination.
- Resolved black window rendering in external SDK consumers by standardizing camera aspect ratio and default look-at orientations.

### Build and SDK
- Pinned `external/miniaudio` Git submodule to release 0.11.26 and eliminated duplicate third-party header vendoring.
- Unified SDK packaging scripts (`package.sh` and `package.bat`) with multi-configuration staging (`dist/sdk/<Config>/AionSDK` and `dist/sandbox/<Config>/AionSandbox`).
- Automatic generation of release archives (`.tar.gz`, `.zip`) and SHA-256 checksums (`SHA256SUMS.txt`).
- Exported CMake target `Aion::AionEngine` automatically resolves `Threads::Threads` and `OpenGL::GL` dependencies for downstream consumers.

### Breaking Changes
- **None**. All changes in 1.1.0 are strictly backward compatible additions adhering to minor semantic versioning.

### Known Limitations
- **Skeletal Skinning Limit**: Skeletal animation shaders support up to 128 joints per model; models exceeding 128 joints fallback to unskinned rendering with a diagnostic warning.

## [1.0.0] - 2026-11-03

### Added
- **Core Engine Subsystems**:
  - Modular `Application` lifecycle architecture supporting `OnCreate`, `OnUpdate`, `OnRender`, `OnEvent`, and `OnDestroy` hooks.
  - Scene graph with hierarchical `Object3D` parent-child transforms, world/local matrix caching, and bounding volume computation.
  - Component-based architecture featuring `CameraComponent`, `CameraControllerComponent`, `MeshRenderer`, `ModelComponent`, `AnimationComponent`, and `RotatorComponent`.
  - Event dispatch system for window lifecycle, mouse, keyboard, and resize events.
- **Rendering Subsystem**:
  - OpenGL 3.3 Core Profile forward renderer with depth testing, viewport management, and render state guards.
  - Camera system supporting `PerspectiveCamera` and `OrthographicCamera` with frustum culling.
  - Primitive mesh generation via `MeshFactory` (Cube, Plane, Sphere, Capsule).
  - Material system supporting base color, metallic, roughness, and emissive properties.
  - Image-Based Lighting (IBL) environment map generation and procedural sky lighting.
  - Built-in GLSL 330 standard shaders (`share/Aion/shaders/standard.vert`, `standard.frag`) with embedded C++ fallback shaders.
- **Asset Pipeline**:
  - 3D model importer supporting glTF and binary GLB formats via TinyGLTF.
  - Skeletal mesh skinning support with up to 128 joints per model and animation clip blending.
  - 2D texture loading and GPU uploading via stb_image.
  - Virtual File System (VFS) with deterministic asset catalog manifests (`share/Aion/vfs_manifest.json`).
- **CMake SDK Integration & Distribution**:
  - Standard CMake package export providing `find_package(AionEngine CONFIG REQUIRED)`.
  - Modern CMake imported target `Aion::AionEngine` automatically linking required dependencies.
  - Semantic versioning config `AionEngineConfigVersion.cmake` with `SameMajorVersion` compatibility.
  - Multi-configuration packaging workflow supporting a unified Windows x64 SDK archive (`AionSDK-1.0.0-windows-x86_64.zip`) containing both Debug and Release static libraries in dedicated subdirectories (`lib/Debug/` and `lib/Release/`) with automatic CMake configuration mapping and MSVC runtime compatibility (MSVC only; Linux SDK distribution is out of scope for initial release).
- **Documentation & Examples**:
  - Comprehensive guides for getting started, installation, CMake integration, and troubleshooting.
  - Minimal, standalone external consumer example project (`examples/basic-application/`).
