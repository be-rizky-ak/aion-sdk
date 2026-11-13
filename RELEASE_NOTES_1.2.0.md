# Aion Engine SDK 1.2.0 Release Notes

**Release Date:** 2026-11-12  
**Version:** 1.2.0  
**Target Platforms:**
- **Windows (x86_64):** MSVC 2019/2022 (`/std:c++20`)
- **Linux (x86_64):** GCC 10+ / Clang 10+ (`-std=c++20`)
- **WebAssembly (wasm32):** Emscripten 3.1.50 through 3.1.64+ (`-std=c++20`)
**Graphics APIs:**
- OpenGL 3.3 Core Profile (Desktop)
- WebGL 2 / OpenGL ES 3.0 (WebAssembly)

---

## 1. Release Overview

Aion Engine SDK version 1.2.0 is a major feature release introducing WebAssembly and WebGL 2 platform support alongside the existing native desktop platforms (Windows and Linux). With Aion 1.2.0, developers can build and distribute real-time 3D applications and games directly to modern web browsers, deployable to static web hosting with zero server-side infrastructure requirements.

The release also introduces an **Optional Platform-Toolchain Architecture** and a **Unified SDK Distribution Structure** (`dist/sdk/AionSDK/`). Native desktop developers do not need Emscripten or Ninja, while Web developers gain streamlined export automation via `build_web.ps1` (Windows) and `build_web.sh` (Linux/macOS).

Aion 1.2.0 maintains 100% backward compatibility with existing native applications built against Aion SDK 1.1.0 and 1.0.0.

---

## 2. Major New Capabilities

### 2.1 WebAssembly & WebGL 2 Platform Support
Aion now compiles to WebAssembly using the Emscripten SDK toolchain and renders using WebGL 2 (OpenGL ES 3.0):
- **Non-Blocking Browser Main Loop:** The application lifecycle (`Application::Run()`) integrates with `emscripten_set_main_loop_arg`, yielding control back to the browser between frames to ensure responsive UI, smooth event delivery, and zero browser tab freezing.
- **GLSL Shader Adaptation:** Built-in shader source translator (`AdaptShaderSourceForWebGL2`) converts desktop GLSL 330 core shaders to GLSL ES 3.00 at runtime, adding necessary float/sampler precision qualifiers (`precision highp float;`).
- **WebGL 2 Rendering Pipeline Adaptations:**
  - Desktop-only polygon wireframe mode (`glPolygonMode`) is safely bypassed under WebGL 2.
  - Framebuffer attachment calls map to `glDrawBuffers` compatible with ES 3.0.
  - Depth texture wrapping adapts from `GL_CLAMP_TO_BORDER` to `GL_CLAMP_TO_EDGE` with shader-level border clamping.
  - Shadow map filtering maintains consistent PCF filtering behavior across desktop and web targets.
- **Virtual File System (VFS) Preloading:** Engine shaders and application assets are bundled into Emscripten's virtual file system (`.data` package), enabling standard file I/O operations (`fopen`, `std::ifstream`, `tinygltf`) without browser network roundtrips during gameplay.
- **Synchronous Main-Thread Asset Parsing:** In Web builds, `AssetManager::LoadModelAsync` automatically falls back to deterministic main-thread decoding, ensuring full glTF/GLB importing without requiring `SharedArrayBuffer` or Cross-Origin Opener/Embedder Policy (COOP/COEP) server headers.

### 2.2 Cross-Platform Web Export Automation
Standalone build automation scripts provide a single-command export workflow:
- **`build_web.ps1` (Windows PowerShell):** Automatically locates Emscripten SDK installations (`$env:EMSDK`, `$env:USERPROFILE\emsdk`, `C:\emsdk`), discovers Ninja build generators across standard paths, detects generator configuration changes, and executes clean Web builds.
- **`build_web.sh` (Linux/macOS):** Automates Emscripten environment activation, Ninja generation, compilation, asset packaging, and export verification.
- **Isolated Static Web Export (`build_web/export/`):** Outputs ready-to-deploy static assets:
  - `index.html` & `sandbox.html`: Responsive HTML5 canvas wrapper with WebGL 2 detection, loading spinner, and canvas resize handling.
  - `sandbox.js`: Emscripten JavaScript runtime glue and module loader.
  - `sandbox.wasm`: Optimized WebAssembly binary.
  - `sandbox.data`: Preloaded asset package.
  - `export_manifest.json`: Provenance metadata including engine version, target name, build timestamp, and SHA-256 hashes.

### 2.3 Unified SDK Distribution Architecture
The official SDK structure has been consolidated into a canonical unified root:
```text
dist/sdk/AionSDK/
├── Windows/          # Native Windows distribution (include/, lib/, CMake/)
├── Linux/            # Native Linux distribution (include/, lib/, CMake/)
├── Web/              # WebAssembly distribution (include/, lib/, CMake/)
├── Common/           # Shared resources (shaders/, web/shell.html, Examples/)
├── Examples/         # Standalone developer demonstrations
│   └── Sandbox/
│       ├── Source/   # C++ demo source code
│       ├── Assets/   # Shaders and textures
│       ├── CMakeLists.txt
│       └── Web/      # Prebuilt, ready-to-host WebGL 2 demo
├── README.md         # SDK documentation
├── LICENSE           # MIT License
├── THIRD-PARTY-NOTICES.txt
├── AionEngineConfig.cmake        # Internal platform resolution entrypoint
├── AionEngineConfigVersion.cmake # Target-aware version check
├── AionSDKConfig.cmake           # Alias for find_package(AionSDK)
└── AionSDKConfigVersion.cmake
```
- **Prebuilt Web Demonstration Included:** Every official SDK distribution bundles the prebuilt WebGL 2 Sandbox in `Examples/Sandbox/Web/`. Players and distributors can host the Web example using any static HTTP server without installing Emscripten, CMake, or compilers.
- **Zero Legacy Mirrors:** Competing SDK roots (`dist/AionSDK`, `dist/sdk/Web`) have been eliminated.

### 2.4 Modular CMake Package Export & Platform Routing
- **Root CMake Package Configuration:** External projects specify `CMAKE_PREFIX_PATH` once pointing to `AionSDK/`. CMake automatically detects the active compiler toolchain and maps native Windows to `Windows/`, Linux to `Linux/`, and Emscripten to `Web/`.
- **Target Platform Compatibility Validation:** `AionEngineConfigVersion.cmake` validates both semantic version compatibility and target platform compatibility, preventing accidental cross-compilation misconfigurations (e.g., attempting to link native Windows libraries into an Emscripten build, or vice versa).
- **`aion_configure_web_executable` Helper:** Packaged in `cmake/platform/Web.cmake`, providing convenient macro configuration for downstream game projects targeting WebAssembly.

---

## 3. Optional Platform-Toolchain Architecture

Aion 1.2.0 establishes the **Optional Platform-Toolchain Architecture**:
> **Core Principle:** Developers only need the tools required by the platform they intend to build for.

- **Native Desktop Development:**
  - Standard MSVC 2019/2022 (Windows) or GCC 10+/Clang 10+ (Linux) with CMake 3.16+.
  - **Zero Web dependencies:** Neither Emscripten nor Ninja is required.
  - Native packaging (`package.bat --native-only` or `./package.sh --native-only`) builds and stages native libraries without inspecting Web tools.
- **Web Development:**
  - Emscripten SDK (`emsdk` 3.1.50 through 3.1.64+) and Ninja build generator.
  - Required only when executing `build_web.ps1`, `build_web.sh`, or packaging with Web deliverables.

---

## 4. Build System & Packaging Verification

Packaging scripts (`package.bat` and `package.sh`) automate:
1. Native compilation (Debug / Release) for `AionEngine` and `sandbox`.
2. WebGL 2 export generation and provenance manifest verification.
3. Unified SDK staging under `dist/sdk/AionSDK/`.
4. Release archive generation (`.tar.gz`, `.zip`) and SHA-256 checksum generation (`SHA256SUMS.txt`).
5. Automated post-packaging verification:
   - Checksum integrity validation (`sha256sum -c`).
   - Structural archive validation (mandatory headers, licenses, shaders, manifests).
   - Source path cleanliness (zero build host leaks).
   - External CMake consumer build against the staged SDK.
   - Packaged Sandbox example compilation and linking against the staged SDK.
   - Headless runtime smoke test under `xvfb-run`.

---

## 5. Compatibility & Semantic Versioning

- **Semantic Versioning:** Aion 1.2.0 is a minor feature release following SemVer rules (`1.2.0`).
- **Breaking API Changes:** **None**. All existing C++20 APIs (`Application`, `Scene`, `Object3D`, `AudioSystem`, `TaskSystem`, `Renderer`, `AssetManager`) remain 100% backward compatible.
- **ABI Considerations:** Native binaries targeting Aion 1.1.0 or 1.0.0 can upgrade to 1.2.0 with a clean recompilation.

---

## 6. Known Limitations

- **Web Single-Threaded Execution:** WebAssembly builds run in single-threaded mode on the main browser thread. Multi-threaded task processing (`TaskSystem` worker pool) is bypassed under Web to avoid requiring `SharedArrayBuffer` isolation headers (`COOP`/`COEP`).
- **Skeletal Skinning Limit:** Skeletal animation shaders support up to 128 joints per model across desktop and Web targets. Models exceeding 128 joints log a diagnostic warning and fallback to unskinned rendering.
- **Browser WebGL 2 Support Required:** Web applications require a modern browser supporting WebGL 2 (Chrome, Edge, Firefox, Safari 15+).

---

## 7. Migration Guide (Upgrading from 1.1.0 to 1.2.0)

Upgrading from 1.1.0 to 1.2.0 requires zero code changes for native desktop applications:

### Native Desktop Projects
Point `CMAKE_PREFIX_PATH` to the unified SDK root:
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/dist/sdk/AionSDK
cmake --build build
```

### Compiling Downstream Projects for WebAssembly
Use `emcmake` pointing to the same unified SDK root:
```bash
emcmake cmake -S . -B build_web -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH -DCMAKE_PREFIX_PATH=/path/to/dist/sdk/AionSDK
cmake --build build_web
```

In your application's `CMakeLists.txt`:
```cmake
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
