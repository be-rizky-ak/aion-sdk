# Getting Started with Aion SDK

This guide walks you through building your first 3D application with the Aion C++20 engine framework using the Aion SDK 1.2.0 for native desktop (Windows/Linux) and WebAssembly (WebGL 2).

---

## 1. Core Architecture Overview

Aion is organized into modular subsystems:

- **Application (`Aion::Application`):** Manages the application lifecycle, window/canvas creation, graphics context initialization, event polling, and frame timing. On the Web, it seamlessly integrates with the browser animation loop (`emscripten_set_main_loop_arg`).
- **Scene (`Aion::Scene`):** Owns the scene graph, object hierarchies, and active camera references.
- **Entities & Transforms (`Aion::Object3D`, `Aion::Transform`):** Hierarchical objects supporting position, rotation (quaternions), and scale, with automatic parent-child matrix propagation.
- **Components (`Aion::Component`):** Extend entity behavior, such as `CameraComponent`, `CameraControllerComponent`, `MeshRenderer`, `AudioSource`, and `RotatorComponent`.
- **Renderer (`Aion::Renderer`):** Manages OpenGL 3.3 Core Profile forward rendering on desktop and WebGL 2 on WebAssembly, with depth testing, shader uniforms, frustum culling, and Image-Based Lighting (IBL).

---

## 2. Application Lifecycle

To create an application, subclass `Aion::Application` and override the lifecycle hooks:

```cpp
#include "Aion/Core/Application.h"
#include "Aion/Math/Vector3.h"
#include "Aion/Scene/Scene.h"
#include "Aion/Scene/Object3D.h"
#include <iostream>

class MyGameApp : public Aion::Application
{
public:
    MyGameApp()
        : Aion::Application(Aion::ApplicationSpecification{
            .Name = "My First Aion Game",
            .Width = 1280,
            .Height = 720,
            .VSync = true,
            .EnableConsoleLogging = true
        })
    {
    }

    void OnCreate() override
    {
        std::cout << "[MyGameApp] Initializing scene..." << std::endl;

        // Populate your scene
        auto myObject = std::make_shared<Aion::Object3D>("Player");
        myObject->Transform.Position = Aion::Vector3(0.0f, 0.0f, -5.0f);
        GetScene()->Add(myObject);
    }

    void OnUpdate() override
    {
        // Per-frame game logic
    }

    void OnRender() override
    {
        // Custom render passes (the engine automatically renders the active scene)
    }

    void OnDestroy() override
    {
        std::cout << "[MyGameApp] Shutting down..." << std::endl;
    }
};

int main(int argc, char** argv)
{
    MyGameApp app;
    app.Run();
    return 0;
}
```

---

## 3. Creating a Camera

Every scene requires a camera to view objects:

```cpp
#include "Aion/Scene/Object3D.h"
#include "Aion/Scene/CameraComponent.h"
#include "Aion/Scene/PerspectiveCamera.h"
#include "Aion/Scene/CameraControllerComponent.h"

void MyGameApp::OnCreate()
{
    // 1. Create camera object
    auto cameraObj = std::make_shared<Aion::Object3D>("MainCamera");

    // 2. Attach a perspective camera component (FOV, Aspect Ratio, Near, Far)
    float aspect = 1280.0f / 720.0f;
    cameraObj->AddComponent<Aion::CameraComponent>(
        std::make_unique<Aion::PerspectiveCamera>(45.0f, aspect, 0.1f, 100.0f)
    );

    // 3. (Optional) Attach camera controller for WASD and mouse rotation
    cameraObj->AddComponent<Aion::CameraControllerComponent>();

    // 4. Set position
    cameraObj->Transform.Position = Aion::Vector3(0.0f, 2.0f, 5.0f);

    // 5. Add to scene (first camera added becomes the active camera)
    GetScene()->Add(cameraObj);
}
```

---

## 4. Adding Primitive Meshes and Materials

Aion provides `MeshFactory` for creating primitive geometry:

```cpp
#include "Aion/Assets/MeshFactory.h"
#include "Aion/Scene/MeshRenderer.h"
#include "Aion/Renderer/Material.h"

void AddCube(Aion::Scene* scene)
{
    auto cubeObj = std::make_shared<Aion::Object3D>("Cube");

    auto mesh = std::shared_ptr<Aion::Mesh>(Aion::MeshFactory::CreateCube());
    auto material = std::make_shared<Aion::Material>();
    material->BaseColor = Aion::Vector4(0.8f, 0.2f, 0.2f, 1.0f); // Red
    material->MetallicFactor = 0.5f;
    material->RoughnessFactor = 0.5f;

    cubeObj->AddComponent<Aion::MeshRenderer>(mesh, material);
    cubeObj->Transform.Position = Aion::Vector3(0.0f, 0.0f, 0.0f);

    scene->Add(cubeObj);
}
```

---

## 5. Building and Running

### Native Desktop (Windows / Linux)
```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/AionSDK -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/MyGameApp
```

### WebAssembly (WebGL 2)
```bash
emcmake cmake -S . -B build_web -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=BOTH -DCMAKE_PREFIX_PATH=/path/to/AionSDK
cmake --build build_web
```
To run the Web build, serve the output directory using a local HTTP server:
```bash
python3 -m http.server 8080 -d build_web
# Open http://localhost:8080/MyGameApp.html in your browser
```

---

## 6. Next Steps

- Follow the [Installation Guide](installation.md) to extract and verify the SDK package.
- Follow the [CMake Integration Guide](cmake-integration.md) for detailed CMake configuration and Web options.
- Review [Troubleshooting](troubleshooting.md) if you encounter build or runtime issues.
