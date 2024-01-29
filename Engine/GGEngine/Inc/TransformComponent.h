#pragma once
#include "Component.h"

namespace rj = rapidjson;
namespace GGEngine
{
    class TransformComponent final
        :public Component
        ,public Graphic::Transform
    {
    public:
        SET_TYPE_ID(ComponentID::Transform);

        void DebugUI() override;
        void EditorUI() override;

        void ParseTransformComponent(const rj::Value& data, GameObject& obj);

        void Serialize(rapidjson::Document& doc, rapidjson::Value& val);
    };
}