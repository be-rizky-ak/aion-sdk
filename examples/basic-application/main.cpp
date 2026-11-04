#include <cstring>
#include <iostream>
#include <memory>

#include "Aion/Assets/MeshFactory.h"
#include "Aion/Core/Application.h"
#include "Aion/Core/Window.h"
#include "Aion/Math/Math.h"
#include "Aion/Math/Quaternion.h"
#include "Aion/Math/Vector3.h"
#include "Aion/Math/Vector4.h"
#include "Aion/Renderer/Material.h"
#include "Aion/Scene/CameraComponent.h"
#include "Aion/Scene/MeshRenderer.h"
#include "Aion/Scene/Object3D.h"
#include "Aion/Scene/PerspectiveCamera.h"
#include "Aion/Scene/Scene.h"

/**
 * @brief Minimal example application consuming the public Aion SDK.
 */
class BasicApplication : public Aion::Application
{
public:
    BasicApplication(bool testMode = false)
        : Aion::Application(Aion::ApplicationSpecification{
            .Name = "Aion SDK - Basic Application",
            .Width = 800,
            .Height = 600,
            .VSync = true
        }),
        m_TestMode(testMode)
    {
    }

    void OnCreate() override
    {
        std::cout << "[BasicApplication] Application started. Initializing scene..." << std::endl;

        // 1. Setup Camera
        auto cameraObject = std::make_shared<Aion::Object3D>("MainCamera");
        cameraObject->AddComponent<Aion::CameraComponent>(
            std::make_unique<Aion::PerspectiveCamera>(45.0f, 800.0f / 600.0f, 0.1f, 100.0f));
        cameraObject->Transform.Position = Aion::Vector3(0.0f, 2.0f, 5.0f);
        cameraObject->Transform.Rotation = Aion::Quaternion::FromEuler(Aion::Math::Radians(-21.8f), 0.0f, 0.0f);
        GetScene()->Add(cameraObject);
        std::cout << "[BasicApplication] Camera initialized at position (0, 2, 5)." << std::endl;

        // 2. Setup Primitive Mesh and Material
        auto cubeObject = std::make_shared<Aion::Object3D>("SampleCube");
        auto cubeMesh = std::shared_ptr<Aion::Mesh>(Aion::MeshFactory::CreateCube());

        auto cubeMaterial = std::make_shared<Aion::Material>();
        cubeMaterial->BaseColor = Aion::Vector4(0.2f, 0.6f, 1.0f, 1.0f);
        cubeMaterial->MetallicFactor = 0.2f;
        cubeMaterial->RoughnessFactor = 0.6f;

        cubeObject->AddComponent<Aion::MeshRenderer>(cubeMesh, cubeMaterial);
        cubeObject->Transform.Position = Aion::Vector3(0.0f, 0.0f, 0.0f);
        GetScene()->Add(cubeObject);
        std::cout << "[BasicApplication] Primitive cube initialized at position (0, 0, 0)." << std::endl;

        std::cout << "[BasicApplication] Scene initialization complete." << std::endl;
    }

    void OnUpdate() override
    {
        if (m_TestMode)
        {
            m_TestFrameCount++;
            std::cout << "[BasicApplication] Running test frame " << m_TestFrameCount << "..." << std::endl;

            // In non-interactive test mode, verify execution and close cleanly after 2 frames
            if (m_TestFrameCount >= 2)
            {
                std::cout << "[BasicApplication] Test frames completed successfully. Exiting." << std::endl;
                if (GetWindow())
                {
                    GetWindow()->SetShouldClose(true);
                }
            }
        }
    }

    void OnDestroy() override
    {
        std::cout << "[BasicApplication] Application shutting down cleanly." << std::endl;
    }

private:
    bool m_TestMode = false;
    uint32_t m_TestFrameCount = 0;
};

int main(int argc, char** argv)
{
    bool testMode = false;
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--test") == 0 || std::strcmp(argv[i], "-t") == 0)
        {
            testMode = true;
        }
    }

    std::cout << "========================================" << std::endl;
    std::cout << " Aion SDK - Basic Application Example   " << std::endl;
    std::cout << "========================================" << std::endl;

    BasicApplication app(testMode);

    if (!app.IsRunning())
    {
        std::cerr << "[BasicApplication] Error: Failed to initialize application graphics context." << std::endl;
        std::cerr << "[BasicApplication] Ensure an OpenGL 3.3 compatible graphics context and display are available." << std::endl;
        return 1;
    }

    app.Run();

    std::cout << "[BasicApplication] Execution completed with status 0." << std::endl;
    return 0;
}
