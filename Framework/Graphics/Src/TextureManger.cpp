#include "Precompiled.h"
#include "TextureManger.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

namespace {
    std::unique_ptr<TextureManger> sInstance;
}

void TextureManger::StaticInitalize(const std::filesystem::path& root)
{
    ASSERT(sInstance == nullptr, "Texture Manger is already Initialized");
    sInstance = std::make_unique<TextureManger>();
    sInstance->SetRootDirectory(root);
}

void TextureManger::StaticTerminate()
{
    sInstance.reset();
}

TextureManger* TextureManger::Get()
{
    ASSERT(sInstance != nullptr, "Texture manger is not initialized");
    return sInstance.get();
}

TextureManger::~TextureManger()
{
    for (auto& [id,texture]:mInventroy)
    {
        texture->Terminate();
    }
    mInventroy.clear();
}

textureID TextureManger::LoadTexture(const std::filesystem::path& filename, bool useRoot)
{
    auto textureID = std::filesystem::hash_value(filename);
    auto [iter, success] = mInventroy.insert({ textureID,nullptr });
    if (success)
    {
        auto& texturePtr = iter->second;
        texturePtr = std::make_unique<Texture>();
        texturePtr->Initialize((useRoot) ? mRootDirectory / filename : filename);
    }

    return textureID;
}

void TextureManger::BindVS(textureID id, uint32_t slot) const
{
    if (id == 0)
    {
        return;
    }

    auto iter = mInventroy.find(id);
    if (iter != mInventroy.end())
    {
        iter->second->BindVS(slot);
    }
}

void TextureManger::BindPS(textureID id, uint32_t slot) const
{
    if (id == 0)
    {
        return;
    }

    auto iter = mInventroy.find(id);
    if (iter != mInventroy.end())
    {
        iter->second->BindPS(slot);
    }
}
