# Aion SDK Installation Guide

This guide details how to install, verify, and set up the Aion SDK on your development workstation.

---

## 1. Prerequisites

### Windows (x86_64)
- **Operating System:** Windows 10 or Windows 11 (64-bit).
- **Compiler:** Microsoft Visual C++ (MSVC) from Visual Studio 2019 or Visual Studio 2022 with C++20 standard support (`/std:c++20`).
- **Build System:** CMake 3.16 or newer.

> **Note on Linux:** Linux SDK distribution is out of scope and not supported for the initial public release.

---

## 2. Obtaining Release Archives

Prebuilt SDK archives are distributed via GitHub Releases. The official Windows x64 distribution provides a single unified archive containing both Debug and Release configurations:

| Archive Filename | Description | Configuration |
|---|---|---|
| `AionSDK-1.1.0-windows-x86_64.zip` | Windows x64 Unified SDK Archive (Zip) | Debug & Release |
| `AionSDK-1.1.0-windows-x86_64.tar.gz` | Windows x64 Unified SDK Tarball | Debug & Release |

### Verifying Checksums

Verify the authenticity of your downloaded archive using the SHA-256 checksums provided in `checksums/SHA256SUMS.txt`.

In PowerShell:
```powershell
Get-FileHash -Algorithm SHA256 .\AionSDK-1.1.0-windows-x86_64.zip
```

Or using Command Prompt (`certutil`):
```cmd
certutil -hashfile AionSDK-1.1.0-windows-x86_64.zip SHA256
```

---

## 3. Extracting the SDK

Extract the archive into your preferred installation directory, for example `C:\AionSDK`:

Using PowerShell:
```powershell
Expand-Archive -Path AionSDK-1.1.0-windows-x86_64.zip -DestinationPath C:\AionSDK
```

Or using the built-in Windows `tar` utility:
```cmd
mkdir C:\AionSDK
tar -xf AionSDK-1.1.0-windows-x86_64.zip -C C:\AionSDK
```

After extraction, the SDK base directory is `C:\AionSDK\AionSDK`.

---

## 4. SDK Directory Layout

The extracted SDK directory contains:

```text
AionSDK/
├── LICENSE                          # MIT License for Aion Engine SDK
├── THIRD-PARTY-NOTICES.txt          # Third-party notices and licenses
├── include/                         # Public C++ header files
│   ├── Aion/                        # Core Aion engine headers
│   │   ├── Assets/
│   │   ├── Core/
│   │   ├── Importers/
│   │   ├── Math/
│   │   ├── Renderer/
│   │   └── Scene/
│   ├── GLFW/                        # GLFW windowing headers
│   ├── glad/                        # GLAD OpenGL loader headers
│   ├── glm/                         # GLM mathematics library headers
│   ├── nlohmann/                    # nlohmann/json parser headers
│   ├── stb/                         # stb_image headers
│   └── tiny_gltf.h                  # TinyGLTF model loader header
├── lib/                             # Static library binaries and CMake configuration
│   ├── Debug/                       # Debug configuration static libraries (MSVC /MDd)
│   │   ├── AionEngine.lib           # Core Aion engine static library
│   │   ├── glad.lib                 # GLAD loader library
│   │   └── glfw3.lib                # GLFW library
│   ├── Release/                     # Release configuration static libraries (MSVC /MD)
│   │   ├── AionEngine.lib           # Core Aion engine static library
│   │   ├── glad.lib                 # GLAD loader library
│   │   └── glfw3.lib                # GLFW library
│   └── cmake/
│       └── AionEngine/              # CMake package configuration
│           ├── AionEngineConfig.cmake
│           ├── AionEngineConfigVersion.cmake
│           ├── AionEngineTargets.cmake
│           ├── AionEngineTargets-debug.cmake
│           └── AionEngineTargets-release.cmake
└── share/
    └── Aion/
        ├── shaders/                 # Runtime GLSL shaders
        │   ├── standard.vert
        │   └── standard.frag
        └── vfs_manifest.json        # Deterministic VFS asset manifest
```

---

## 5. Next Steps

Proceed to the [CMake Integration Guide](cmake-integration.md) to integrate the extracted SDK into your project.
