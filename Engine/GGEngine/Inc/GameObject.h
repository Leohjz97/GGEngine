#pragma once
#include "Component.h"
#include "TypeID.h"
#include "GameObjectHandle.h"

namespace GGEngine
{
    class GameObject final
    {
    public:
        GameObject() = default;

        void Initialize();
        void Terminate();

        void Update(float deltaTime);
        void DebugUI();
        void EditorUI();

        template<class ComponentType>
        ComponentType* AddComponent()
        {
            ASSERT(!mInitialize, "GameObject: cannot add component after GameObject is initiazlized.");
            auto& newComponent = comps.emplace_back(std::make_unique<ComponentType>());
            newComponent->ower = this;
            return static_cast<ComponentType*>(newComponent.get());
        }

        template<class ComponentType>
        ComponentType* GetComponent()
        {
            //auto constThis = static_cast<const GameObject*>(this);
            //return const_cast<Component*>(constThis->GetComponent<ComponentType>());
            for (auto& comp : comps)
            {
                if (comp->GetTypeID() == ComponentType::StaticGetTypeID())
                {
                    return static_cast<ComponentType*>(comp.get());
                }
            }

            return nullptr;
        }

        template<class ComponentType>
        const ComponentType* GetComponent() const
        {
            for (auto& comp : comps)
            {
                if (comp->GetTypeID() == ComponentType::StaticGetTypeID())
                {
                    return static_cast<ComponentType*>(comp.get());
                }
            }

            return nullptr;
        }

        void SetName(const char* name) { mName = name; }
        const std::string& GetName()const { return mName; }

        GameWorld& GetWorld() { return *mWorld; }
        const GameWorld& GetWorld() const { return *mWorld; }
        const GameObjectHandle& GetHandle() const { return mHandle; }

        uint32_t GetUniqueID() const { return mUniquedID; }


        void Serialize(rapidjson::Document& doc, rapidjson::Value& val);
    private:
        friend class GameWorld;
        GameWorld* mWorld = nullptr;
        GameObjectHandle mHandle;
        
        using Component = std::vector<std::unique_ptr<Component>>;
        Component comps;

        std::string mName = "N/A";
        bool mInitialize = false;
        uint32_t mUniquedID = 0;
    };
}