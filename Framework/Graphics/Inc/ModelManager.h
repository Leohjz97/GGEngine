#pragma once
#include "Model.h"

namespace GGEngine::Graphic
{
    using ModelID = std::size_t;

    class ModelManager final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static ModelManager* Get();

    public:
        ModelManager() = default;
        ~ModelManager() = default;

        ModelManager(const ModelManager&) = delete;
        ModelManager& operator=(const ModelManager&) = delete;
        ModelManager(const ModelManager&&) = delete;
        ModelManager&& operator=(const ModelManager&&) = delete;

        ModelID GetModelID(const std::filesystem::path& filepath);
        ModelID LoadModelID(const std::filesystem::path& filepath);
        void AddAnimation(ModelID id, const std::filesystem::path& filepath);
        const Model* GetModel(ModelID id);

    private:
        using ModelCache = std::map<ModelID, std::unique_ptr<Model>>;
        ModelCache mInventory;
    };
}