#pragma once
#include"TypeID.h"

namespace GGEngine
{
    class GameObject;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(const Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(const Component&&) = delete;

        virtual void Initialize() {}
        virtual void Terminate() {}
        virtual void Update(float deltTime) {}
        virtual void DebugUI() {}
        virtual void EditorUI() {};
        virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& val) {};

        virtual uint32_t GetTypeID() const = 0;

        GameObject& GetOwner() { return *ower; }
        const GameObject& GetOwner()const { return *ower; }

    private:
        friend class GameObject;
        GameObject* ower = nullptr;
    };
}