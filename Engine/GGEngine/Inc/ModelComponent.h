#pragma once

#include "Component.h"
namespace rj = rapidjson;
namespace GGEngine
{
    class ModelComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::ModelComponent);

        void Initialize() override;
        void Terminate() override;

        void SetFileName(const char* fileName) { mFileName = fileName; }
        void AddAnimation(const char* animationFileName) { mAnimationFileNames.push_back(animationFileName); }

        void ParseModelComponent(const rj::Value& data, GameObject& obj);

        Graphic::ModelID GetModelID() const { return mModelID; }
        const Graphic::Model& GetModel() const
        {
            return *Graphic::ModelManager::Get()->GetModel(mModelID);
        }

        const GGMath::Vector3& GetRotation() const { return mRotation; }
        void SetRotation(const GGMath::Vector3& rotation) { mRotation = rotation; }
    private:
        std::string mFileName;

        Graphic::ModelID mModelID = 0;
        using Animation = std::vector<std::string>;
        Animation mAnimationFileNames;
        GGMath::Vector3 mRotation = GGMath::Vector3::Zero;
    };
}