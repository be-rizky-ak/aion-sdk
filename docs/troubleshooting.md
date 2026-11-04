# Troubleshooting Guide

This guide covers common issues encountered when integrating and building projects with the Aion SDK on Windows x64 with MSVC, along with their diagnostic solutions.

---

## 1. CMake Configuration Issues

### Symptom: `Could not find a package configuration file provided by "AionEngine"`

```text
CMake Error at CMakeLists.txt:X (find_package):
  By not providing "FindAionEngine.cmake" in CMAKE_MODULE_PATH this project
  has asked CMake to find a package configuration file provided by
  "AionEngine", but CMake did not find one.
```

**Cause:** CMake cannot locate `AionEngineConfig.cmake`.
**Solution:** Pass the path to the extracted `AionSDK` directory using `-DCMAKE_PREFIX_PATH`:
```cmd
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/path/to/AionSDK
```
Ensure you point to the `AionSDK` base directory, not the `lib/cmake/AionEngine/` subfolder.

---

### Symptom: Compiler reports C++20 standard errors or concepts missing

```text
error: #error This file requires compiler and library support for the ISO C++ 2020 standard.
```

**Cause:** The compiler is operating in C++14 or C++17 mode, or the compiler version is too old.
**Solution:**
1. Ensure your compiler is Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 with C++20 standard support (`/std:c++20`).
2. Ensure your `CMakeLists.txt` declares:
   ```cmake
   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

---

## 2. Windows MSVC Build Issues

### Symptom: Multi-Configuration generator build outputs (Debug vs. Release selection)

**Cause:** Visual Studio CMake generators default to multi-configuration project files where the active build configuration selects the target output.
**Solution:** The unified SDK includes both Debug and Release static libraries. When invoking `cmake --build build`, specify `--config Release` or `--config Debug`:
```cmd
cmake --build build --config Release
```
CMake automatically selects the corresponding static libraries from `lib/Release/` or `lib/Debug/`. In the Visual Studio IDE, selecting "Debug" or "Release" from the configuration dropdown automatically switches the linked libraries.

---

### Symptom: `error LNK2038: mismatch detected for 'RuntimeLibrary'`

```text
error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MD_DynamicRelease' doesn't match value 'MDd_DynamicDebug'
```

**Cause:** MSVC enforces matching C/C++ Runtime Library settings between application code and static libraries. Release, RelWithDebInfo, and MinSizeRel configurations use `/MD` (Release CRT), whereas Debug configurations use `/MDd` (Debug CRT).
**Solution:** The unified SDK avoids this conflict by providing separated directories:
- `lib/Debug/`: Compiled with `/MDd`.
- `lib/Release/`: Compiled with `/MD`.
Intermediate configurations `RelWithDebInfo` and `MinSizeRel` automatically map to `Release` via `MAP_IMPORTED_CONFIG`, ensuring consistent `/MD` runtime flags. Ensure your application's `CMAKE_MSVC_RUNTIME_LIBRARY` or compiler flags match standard MSVC defaults.

---

### Symptom: `error LNK2019: unresolved external symbol`

**Cause:** The application target did not link against `Aion::AionEngine`.
**Solution:** Ensure `target_link_libraries(YourTarget PRIVATE Aion::AionEngine)` is declared in your `CMakeLists.txt`. CMake will automatically link `AionEngine.lib`, `glad.lib`, `glfw3.lib`, and Windows system OpenGL libraries (`opengl32.lib`).

---

## 3. Runtime & Graphics Context Issues

### Symptom: `[Application] Critical Error: Application failed to initialize window or graphics context.`

**Cause:** The application was launched on a system or virtual machine without an active display or compatible OpenGL 3.3 graphics driver.
**Solution:** Ensure graphics drivers supporting OpenGL 3.3 Core Profile are installed (NVIDIA, AMD, or Intel). In virtualized environments without GPU acceleration, ensure a compatible software OpenGL rasterizer is available.
For automated verification, pass test flags (e.g., `--test`) if implemented in your application to render and exit cleanly without user interaction.

---

### Symptom: Shader Compilation Warnings at Runtime

```text
[FileSystem] Warning: Virtual path assets://shaders/standard.vert does not resolve to an existing file.
```

**Cause:** The application's working directory does not contain an `assets/` directory with `shaders/standard.vert`.
**Solution:**
- The engine automatically falls back to embedded standard shaders compiled into `AionEngine.lib`, so your application will continue rendering correctly.
- To supply external shaders, copy `share/Aion/shaders/` from the SDK into an `assets/shaders/` directory adjacent to your executable or working directory.

---

## 4. Model Importing & Animation Limitations

### Symptom: `[ModelImporter] Warning: Mesh skin exceeds 128 joints`

**Cause:** Aion's forward rendering skeletal animation shader supports a maximum of 128 joints per mesh.
**Solution:** Models with more than 128 joints will log a diagnostic warning and fallback to unskinned rendering. Ensure exported glTF/GLB models stay within the 128-joint limit.
