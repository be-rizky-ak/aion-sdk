# Changelog

All notable changes to the Aion SDK will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
