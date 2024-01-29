#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

namespace
{
    std::unique_ptr<ModelManager> mModelManager;
}

void ModelManager::StaticInitialize()
{
    ASSERT(mModelManager == nullptr, "Model Manager -- system already initialized");
    mModelManager = std::make_unique<ModelManager>();
}

void ModelManager::StaticTerminate()
{
    mModelManager.reset();
}

ModelManager* ModelManager::Get()
{
    ASSERT(mModelManager != nullptr, "ModelManager -- not initialized");
    return mModelManager.get();
}

ModelID ModelManager::GetModelID(const std::filesystem::path& filepath)
{
    return std::filesystem::hash_value(filepath);
}

ModelID ModelManager::LoadModelID(const std::filesystem::path& filepath)
{
    const auto modelid = GetModelID(filepath);
    auto [iter, success] = mInventory.insert({ modelid,nullptr });
    if (success)
    {
        auto& modelPtr = iter->second;
        modelPtr = std::make_unique<Model>();
        ModelIO::LoadModel(filepath, *modelPtr);
        ModelIO::LoadSkeleton(filepath, *modelPtr);
        ModelIO::LoadMaterial(filepath, *modelPtr);
        ModelIO::LoadAnimations(filepath, *modelPtr);
    }

    return modelid;
}

void ModelManager::AddAnimation(ModelID id, const std::filesystem::path& filepath)
{
    auto model = mInventory.find(id);
    ASSERT(model != mInventory.end(), "ModelManager: need to load the model first");
    ModelIO::LoadAnimations(filepath, *model->second);
}

const Model* ModelManager::GetModel(ModelID id)
{
    auto model = mInventory.find(id);
    if (model != mInventory.end())
    {
        return model->second.get();
    }
    return nullptr;
}
