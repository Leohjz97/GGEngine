#include"Precompiled.h"
#include"TransformComponent.h"
#include "GameObject.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void TransformComponent::DebugUI()
{
    Graphic::SimpleDraw::AddTransform(GetMatrix4());
}

void GGEngine::TransformComponent::EditorUI()
{
    ImGui::LabelText("Transform","");

    auto uniqueID = GetOwner().GetUniqueID();
    std::string posStr = "Position##" + std::to_string(uniqueID);
    std::string rotStr = "Rotation##" + std::to_string(uniqueID);
    std::string scaleStr = "Sacle##" + std::to_string(uniqueID);

    ImGui::DragFloat3("Position##", &position.x);
    ImGui::DragFloat3("Rotation##", &rotation.x);
    ImGui::DragFloat3("Scale##", &scale.x);
}

void GGEngine::TransformComponent::ParseTransformComponent(const rj::Value& data, GameObject& obj)
{
    auto transformComponent = obj.AddComponent<TransformComponent>();
    if (data.HasMember("Position"))
    {
        const auto& position = data["Position"].GetArray();
        const float x = position[0].GetFloat();
        const float y = position[1].GetFloat();
        const float z = position[2].GetFloat();
        transformComponent->position = { x, y, z };
    }
    if (data.HasMember("Rotation"))
    {
        const auto& position = data["Rotation"].GetArray();
        const float x = position[0].GetFloat() * Constants::DegToRad;
        const float y = position[1].GetFloat() * Constants::DegToRad;
        const float z = position[2].GetFloat() * Constants::DegToRad;
        transformComponent->rotation = GGMath::Quaternion::RotationEuler({ x, y, z });
    }
    if (data.HasMember("Scale"))
    {
        const auto& position = data["Scale"].GetArray();
        const float x = position[0].GetFloat();
        const float y = position[1].GetFloat();
        const float z = position[2].GetFloat();
        transformComponent->scale = { x, y, z };
    }
}

void GGEngine::TransformComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& val)
{
    rapidjson::Value compValue(rapidjson::kObjectType);

    rapidjson::Value postArray(rapidjson::kArrayType);
    postArray.PushBack(position.x, doc.GetAllocator());
    postArray.PushBack(position.y, doc.GetAllocator());
    postArray.PushBack(position.z, doc.GetAllocator());
    compValue.AddMember("Position", postArray, doc.GetAllocator());
    
    rapidjson::Value rotArray(rapidjson::kArrayType);
    rotArray.PushBack(rotation.x, doc.GetAllocator());
    rotArray.PushBack(rotation.y, doc.GetAllocator());
    rotArray.PushBack(rotation.z, doc.GetAllocator());
    rotArray.PushBack(rotation.w, doc.GetAllocator());
    compValue.AddMember("Rotation", rotArray, doc.GetAllocator());
    
    rapidjson::Value scaleArray(rapidjson::kArrayType);
    scaleArray.PushBack(scale.x, doc.GetAllocator());
    scaleArray.PushBack(scale.y, doc.GetAllocator());
    scaleArray.PushBack(scale.z, doc.GetAllocator());
    compValue.AddMember("Position", scaleArray, doc.GetAllocator());

    val.AddMember("TransformComponent", compValue, doc.GetAllocator());
}
