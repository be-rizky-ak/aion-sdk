# Changelog

All notable changes to the Aion SDK will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
