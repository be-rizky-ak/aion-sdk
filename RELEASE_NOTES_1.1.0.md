# Aion Engine SDK 1.1.0 Release Notes

**Release Date:** 2026-11-09  
**Version:** 1.1.0  
**Target Architecture:** Windows (x86_64, MSVC 2019/2022)  
**Graphics API:** OpenGL 3.3 Core Profile  

---

## 1. Release Overview

Aion Engine SDK version 1.1.0 is the first feature update to the public Aion SDK, expanding engine capabilities while maintaining 100% backward compatibility with version 1.0.0. This release introduces a comprehensive audio subsystem supporting WAV, MP3, and OGG Vorbis, a multi-threaded task system for asynchronous computation, configurable console diagnostic logging, extended animation playback controls, equirectangular skybox background rendering, and streamlined multi-configuration SDK packaging.

All new features are provided through clean, modular C++20 public APIs with strict PIMPL encapsulation, ensuring third-party backend libraries (such as miniaudio) remain internal to the engine runtime without leaking into downstream consumer compilation units.

---

## 2. Major New Capabilities

### 2.1 Audio Subsystem
Aion now features a dedicated, low-latency audio subsystem supporting interactive spatial audio and background music streaming:
- **Supported Formats**: Uncompressed PCM WAV, compressed MP3, and OGG Vorbis (via internal miniaudio and stb_vorbis decoders).
- **Core Subsystem (`Aion::AudioSystem`)**: Manages the hardware audio output device, master volume, and category submixes. Gracefully falls back to headless/null operation when physical audio hardware is unavailable.
- **Audio Clip (`Aion::AudioClip`)**: Resource representation supporting both in-memory caching for short sound effects and streaming buffers for long music tracks.
- **Spatial Audio (`Aion::AudioSource` and `Aion::AudioListener`)**: 3D audio components that integrate directly into the `Object3D` scene graph with distance attenuation and directional orientation.
- **Volume Routing (`Aion::AudioCategory`)**: Independent channel control across `Master`, `Music`, and `SFX`.
- **One-Shot Playback**: Fire-and-forget sound effect invocation via `AudioSystem::PlayOneShot(...)`.

### 2.2 Task System & Concurrency
The new `Aion::TaskSystem` provides an asynchronous worker thread pool for offloading computationally intensive tasks from the main simulation thread:
- Automatic worker thread pooling based on hardware concurrency or user configuration.
- Thread-safe job queues and task dispatch returning standard `std::future<T>`.
- Deterministic initialization and shutdown lifecycle synchronization.

### 2.3 Configurable Console Diagnostic Logging
Aion 1.1.0 introduces centralized diagnostic log control (`Aion::Log`):
- Build-time default adaptation: Console logging is automatically enabled in `Debug` builds for developer productivity and suppressed in `Release` builds (`/SUBSYSTEM:WINDOWS` on Windows).
- Explicit CMake configure override: `-DAION_ENABLE_CONSOLE_LOG=ON` or `-DAION_ENABLE_CONSOLE_LOG=OFF`.
- Dynamic runtime control via `Aion::Log::SetConsoleLoggingEnabled(bool)` and `ApplicationSpecification.EnableConsoleLogging`.

### 2.4 Animation Component Extensions
`Aion::AnimationComponent` has been enhanced with essential gameplay controls:
- **Playback Speed**: `SetPlaybackSpeed(float)` and `GetPlaybackSpeed()` for slow-motion, fast-forward, or reverse animation effects.
- **State Control**: `Reset()` to rewind animation to the initial frame.
- **Queries**: `IsPlaying()` and `GetClipNames()` for runtime inspection and state synchronization.

---

## 3. Rendering and Asset Pipeline Improvements

- **Seamless Equirectangular Skybox**: Support for 360-degree panoramic skybox textures using an optimized fullscreen quad pass with automatic 2:1 aspect ratio validation.
- **Cubemap Render Protection**: Robust OpenGL state management disabling face culling and depth masking during cubemap passes, preventing visual artifacting.
- **SVG Vector Graphics**: Added `Aion::SVGImporter` for rasterizing Scalable Vector Graphics into runtime engine textures.
- **Clean Shutdown Lifecycle**: Explicit `TextureLibrary::Shutdown()` lifecycle ensures GPU textures are freed while the OpenGL context remains valid, eliminating termination segmentation faults.

---

## 4. Build System & SDK Packaging

- **Unified Windows SDK Distribution**: The public SDK is distributed as a unified Windows x64 archive (`AionSDK-1.1.0-windows-x86_64.zip`) containing both Debug and Release static libraries in dedicated subdirectories (`lib/Debug/` and `lib/Release/`).
- **CMake Target Mapping**: Modern CMake imported target `Aion::AionEngine` automatically selects the matching static library and C runtime (`/MDd` Debug vs `/MD` Release) based on the consuming project's configuration.
- **Dependency Isolation**: Downstream consumers link against `Aion::AionEngine` without requiring direct headers for internal third-party dependencies (`miniaudio`).

---

## 5. Compatibility & Semantic Versioning

- **Semantic Versioning**: Minor release `1.1.0` is strictly backward compatible with `1.0.0`.
- **Breaking API Changes**: **None**. All existing function signatures, component ownership semantics, and scene graph behaviors are preserved without modification.
- **ABI Considerations**: C++ standard remains C++20. Downstream consumers using version 1.0.0 can upgrade by recompiling with SDK 1.1.0 without modifying application code.

---

## 6. Known Limitations

- **Skeletal Mesh Joint Limit**: The standard skeletal skinning shader supports up to 128 joints per model. Models with more than 128 joints will log a diagnostic warning and fallback to unskinned static mesh rendering.

---

## 7. Migration Guide (Upgrading from 1.0.0 to 1.1.0)

Upgrading an existing project from Aion SDK 1.0.0 to 1.1.0 requires zero code changes for existing features:

1. **Update CMake Prefix Path**:
   Point `CMAKE_PREFIX_PATH` to the extracted Aion SDK 1.1.0 directory:
   ```cmd
   cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/AionSDK-1.1.0
   ```

2. **Optional: Enable Audio Subsystem**:
   Include public audio headers and initialize sound playback:
   ```cpp
   #include <Aion/Audio/AudioSystem.h>
   #include <Aion/Audio/AudioSource.h>

   // Initialize clip
   auto soundClip = Aion::AssetManager::LoadAudioClip("assets://audio/jump.wav");
   Aion::AudioSystem::PlayOneShot(soundClip, 1.0f);
   ```

3. **Optional: Control Release Console Output**:
   If console diagnostics are desired in Release builds, configure CMake with `-DAION_ENABLE_CONSOLE_LOG=ON` or set `ApplicationSpecification.EnableConsoleLogging = true`.
