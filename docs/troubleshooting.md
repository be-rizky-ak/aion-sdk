# Troubleshooting Guide

This guide covers common issues encountered when integrating, building, and running projects with the Aion SDK 1.2.0 on native desktop platforms (Windows and Linux) and WebAssembly (WebGL 2), along with their diagnostic solutions.

---

## 1. CMake Configuration Issues

### Symptom: `Could not find a package configuration file provided by "AionEngine"` (Native Desktop)

```text
CMake Error at CMakeLists.txt:X (find_package):
  By not providing "FindAionEngine.cmake" in CMAKE_MODULE_PATH this project
  has asked CMake to find a package configuration file provided by
  "AionEngine", but CMake did not find one.
```

**Cause:** CMake cannot locate `AionEngineConfig.cmake`.  
**Solution:** Pass the path to the extracted unified `AionSDK` directory using `-DCMAKE_PREFIX_PATH`:
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/AionSDK
```
Ensure you point to the canonical `AionSDK` root directory, which contains `AionEngineConfig.cmake`, not a nested subfolder.

---

### Symptom: `Could not find a package configuration file provided by "AionEngine"` (Emscripten Web)

```text
CMake Error at CMakeLists.txt:X (find_package):
  Could not find a package configuration file provided by "AionEngine" ...
```

**Cause:** Emscripten's toolchain file sets `CMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY` by default, which restricts CMake's `find_package` lookups exclusively to the Emscripten sysroot. Host filesystem paths passed via `CMAKE_PREFIX_PATH` are ignored.  
**Solution:** Pass `-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH` when invoking `emcmake cmake`:
```bash
emcmake cmake -S . -B build_web \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH \
    -DCMAKE_PREFIX_PATH=/path/to/AionSDK
```

---

### Symptom: Compiler reports C++20 standard errors or concepts missing

```text
error: #error This file requires compiler and library support for the ISO C++ 2020 standard.
```

**Cause:** The compiler is operating in C++14 or C++17 mode, or the compiler does not support C++20.  
**Solution:**
1. Ensure your compiler meets the minimum prerequisites: MSVC 2019/2022 on Windows, GCC 10+ or Clang 10+ on Linux, or Emscripten 3.1.50+ for Web.
2. Ensure your `CMakeLists.txt` declares:
   ```cmake
   set(CMAKE_CXX_STANDARD 20)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)
   ```

---

## 2. WebAssembly & WebGL 2 Runtime Issues

### Symptom: Browser fails to load `.wasm` or `.data` (`Failed to fetch`, CORS, or incorrect MIME type)

```text
TypeError: Failed to fetch
Fetch API cannot load file:///path/to/app.wasm. URL scheme must be "http" or "https" for CORS request.
```
or
```text
wasm streaming compile failed: TypeError: Failed to execute 'compile' on 'WebAssembly': Incorrect response MIME type. Expected 'application/wasm'.
```

**Cause:** Modern web browsers block WebAssembly loading and XMLHttpRequest/Fetch requests when accessing pages directly via the `file://` protocol. Alternatively, the web server is returning an incorrect MIME type (such as `text/plain` or `application/octet-stream` for `.wasm`).  
**Solution:**
1. **Never open HTML files directly with `file://`**. Always serve via a local HTTP server:
   ```bash
   # Python 3 built-in HTTP server:
   python3 -m http.server 8080 -d build_web
   ```
   Or using Node.js:
   ```bash
   npx serve build_web
   ```
2. For production web servers (Nginx / Apache), ensure proper MIME types are configured:
   - `.wasm` -> `application/wasm`
   - `.data` -> `application/octet-stream`
   - `.js` -> `application/javascript`

---

### Symptom: `[Application] Critical Error: Application failed to initialize window or graphics context` (Web)

**Cause:** The browser fails to acquire a WebGL 2 context from the `<canvas>` element.  
**Solution:**
1. Verify that your browser supports WebGL 2 by opening `https://get.webgl.org/webgl2/`.
2. In Google Chrome / Edge, verify `chrome://gpu` to ensure hardware acceleration is active and WebGL 2 is not blocklisted.
3. In Apple Safari, ensure WebGL 2 is enabled under Develop > Experimental Features.

---

### Symptom: Runtime Asset Not Found in Web Build (`assets://` does not resolve)

```text
[FileSystem] Warning: Virtual path assets://models/player.glb does not resolve to an existing file.
```

**Cause:** The asset was not bundled into Emscripten's Virtual File System (VFS) `.data` package during compilation.  
**Solution:** Use the `aion_configure_web_executable` helper macro in your `CMakeLists.txt` and specify the assets directory:
```cmake
if(EMSCRIPTEN)
    aion_configure_web_executable(MyGame
        ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    )
endif()
```
This automatically invokes `--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets` to package all demonstration assets into the `.data` bundle.

---

### Symptom: Threading / Worker Crash or `SharedArrayBuffer is not defined` (Web)

**Cause:** Browsers require Cross-Origin Isolation headers (`Cross-Origin-Opener-Policy: same-origin` and `Cross-Origin-Embedder-Policy: require-corp`) to enable `SharedArrayBuffer` for multi-threaded WebAssembly worker pools.  
**Solution:** Aion SDK 1.2.0 Web builds run single-threaded on the main browser thread. The asset pipeline (`AssetManager::LoadModelAsync`) automatically falls back to deterministic synchronous main-thread parsing in Web builds to ensure 100% compatibility with standard static web hosting without requiring custom server isolation headers. Avoid spawning raw `std::thread` instances in downstream Web code.

---

## 3. Windows MSVC Build Issues

### Symptom: `error LNK2038: mismatch detected for 'RuntimeLibrary'`

```text
error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MD_DynamicRelease' doesn't match value 'MDd_DynamicDebug'
```

**Cause:** MSVC enforces matching C/C++ Runtime Library settings between application code and static libraries. Debug uses `/MDd`, whereas Release uses `/MD`.  
**Solution:** The unified Windows SDK provides separated configurations:
- `Windows/lib/Debug/`: Compiled with `/MDd`.
- `Windows/lib/Release/`: Compiled with `/MD`.  
When using Visual Studio or CMake multi-config generators, CMake automatically links the corresponding static libraries based on the active build configuration (`--config Release` or `--config Debug`). `RelWithDebInfo` and `MinSizeRel` automatically map to `Release` via `MAP_IMPORTED_CONFIG`.

---

### Symptom: `error LNK2019: unresolved external symbol`

**Cause:** The application target did not link against `Aion::AionEngine`.  
**Solution:** Ensure `target_link_libraries(YourTarget PRIVATE Aion::AionEngine)` is declared in your `CMakeLists.txt`. CMake will automatically link `AionEngine.lib`, `glad.lib`, `glfw3.lib`, and Windows system OpenGL libraries (`opengl32.lib`).

---

## 4. Runtime & Graphics Context Issues (Desktop)

### Symptom: Application failed to initialize window or graphics context (Linux / Headless CI)

**Cause:** The application was launched on a system or container without an active X11 display or compatible GPU OpenGL driver.  
**Solution:** On headless Linux or CI systems, run the application using `xvfb-run`:
```bash
xvfb-run -a ./build/MyGameApp --test
```

---

### Symptom: Console Output Suppressed in Release Builds

**Cause:** In Release builds, Aion suppresses diagnostic console output by default to optimize performance and prevent console popping on Windows.  
**Solution:**
- To enable console logging at runtime, set `.EnableConsoleLogging = true` in `ApplicationSpecification`:
  ```cpp
  Aion::ApplicationSpecification spec{
      .Name = "My Game",
      .EnableConsoleLogging = true
  };
  ```
- Or enable it dynamically via `Aion::Log::SetConsoleLoggingEnabled(true)`.
- Or pass `-DAION_ENABLE_CONSOLE_LOG=ON` at CMake configuration time.

---

## 5. Model Importing & Animation Limitations

### Symptom: `[ModelImporter] Warning: Mesh skin exceeds 128 joints`

**Cause:** Aion's forward rendering skeletal animation shaders support a maximum of 128 joints per mesh across both desktop and Web targets.  
**Solution:** Models with more than 128 joints log a diagnostic warning and fallback to unskinned static mesh rendering. Ensure exported glTF/GLB models stay within the 128-joint limit.
