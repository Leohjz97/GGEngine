#pragma once
#include "Component.h"
namespace rj = rapidjson;

namespace GGEngine
{
    class CameraComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Camera);

        void Initialize() override;
        void Terminate() override;

        void ParseCameraComponent(const rj::Value& data, GameObject& obj);

        Graphic::Camera& GetCamera() { return cam; }
        const Graphic::Camera& GetCamera() const { return cam; }

    private:
        Graphic::Camera cam;
    };
}