#pragma once
#include "Texture.h"

namespace GGEngine::Graphic
{
    using textureID = std::size_t;
    
    class TextureManger
    {
    public: 
        static void StaticInitalize(const std::filesystem::path& root);
        static void StaticTerminate();
        static TextureManger* Get();

    public:
        TextureManger() = default;
        ~TextureManger();

        TextureManger(const TextureManger&) = delete;
        TextureManger& operator=(const TextureManger&) = delete;
        TextureManger(const TextureManger&&) = delete;
        TextureManger&& operator=(const TextureManger&&) = delete;

        void SetRootDirectory(const std::filesystem::path& root)
        {
            mRootDirectory = std::move(root);
        }

        textureID LoadTexture(const std::filesystem::path& filename, bool useRoot = true);

        void BindVS(textureID id, uint32_t slot)const;
        void BindPS(textureID id, uint32_t slot)const;

    private:
        using Inventory = std::unordered_map<textureID, std::unique_ptr<Texture>>;
        Inventory mInventroy;

        std::filesystem::path mRootDirectory;
    };
}