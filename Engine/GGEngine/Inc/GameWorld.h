#pragma once

#include"GameObject.h"
#include "Service.h"

namespace GGEngine
{
    using CustomService = std::function<bool(const char*, const rapidjson::Value& value, GameWorld&)>;

    class GameWorld final
    {
    public:
        static void SetCustomServiceMake(CustomService customService);

        void Initialize(uint32_t cap);
        void Terminate();

        void Update(float deltaTime);
        void Render();
        void DebugUI();
        void EditorUI();

        void LoadLevel(const std::filesystem::path& levelFile);
        void SaveLevel(const std::filesystem::path& levelFile);
        GameObject* CreateGameObject(const std::filesystem::path& templateFile);
        GameObject* GetGameObject(const GameObjectHandle& handle);
        GameObject* FindGameObjectByName(const std::string& name);
        void DestoryGameObject(const GameObjectHandle& handle);

        template<class ServiceType>
        ServiceType* AddService()
        {
            static_assert(std::is_base_of_v<Service, ServiceType>, "GameWorld: Service must be derived");
            ASSERT(!mInitialized, "GameWorld: cant ADD Service once Initialized");
            auto& newService = mServices.emplace_back(std::make_unique<ServiceType>());
            newService->mWorld = this;
            return static_cast<ServiceType*>(newService.get());
        }

        template<class ServiceType>
        ServiceType* GetService()
        {
            for (auto& service : mServices)
            {
                if (service->GetTypeID() == ServiceType::StaticGetTypeID())
                {
                    return static_cast<ServiceType*>(service.get());
                }
            }

            return nullptr;
        }

        template<class ServiceType>
        const ServiceType* GetService() const
        {
            for (auto& service : mServices)
            {
                if (service->GetTypeID() == ServiceType::StaticGetTypeID())
                {
                    return static_cast<ServiceType*>(service.get());
                }
            }

            return nullptr;
        }

    private:
        bool IsVaild(const GameObjectHandle& handle);
        void ProcessDestroyList();

        struct Slot
        {
            std::unique_ptr<GameObject> gameObject;
            uint32_t generation = 0;
        };

        using Services = std::vector<std::unique_ptr<Service>>;
        using GameObjectSlots = std::vector<Slot>;
        using GameObjectPtrs = std::vector<GameObject*>;

        Services mServices;

        GameObjectSlots mGameObjectSlots;
        std::vector<uint32_t> mFreeSlots;
        std::vector<uint32_t> mToBeDestroyed;

        GameObjectPtrs mUpdateList;

        bool mInitialized = false;
        bool mUpdating = false;
    };
}