#include "Precompiled.h"
#include "GameObject.h"

using namespace GGEngine;

static uint32_t gUniqueID = 0;

void GameObject::Initialize()
{
    ASSERT(!mInitialize, "GameObject: Already Initialized");
    mInitialize = true;

    for (auto& cp:comps)
    {
        cp->Initialize();
    }
    
    mUniquedID = ++gUniqueID;
}

void GameObject::Terminate()
{
    for (auto& cp : comps)
    {
        cp->Terminate();
    }
}

void GameObject::Update(float deltaTime)
{
    for (auto& cp : comps)
    {
        cp->Update(deltaTime);
    }
}

void GameObject::DebugUI()
{
    for (auto& cp : comps)
    {
        cp->DebugUI();
    }
}

void GameObject::EditorUI()
{
    //ImGui::Button("Save Template")
    //ImGui::CollapsingHeader(mName.c_str(),ImGuiTreeNodeFlags_None)
    //ImGui::InputText("GameObject##", buffer, sizeof(buffer))

    std::string gameobjName = std::string("GameObject##" + std::to_string(mUniquedID));
    if (ImGui::CollapsingHeader(gameobjName.c_str(), ImGuiTreeNodeFlags_None))
    {
        char buffer[256];
        strcpy_s(buffer, mName.c_str());
        std::string objName = std::string("Name##" + std::to_string(mUniquedID));
        if (ImGui::InputText(objName.c_str(), buffer, sizeof(buffer)))
        {
            mName = std::move(buffer);
        }

        for (auto& cp : comps)
        {
            cp->EditorUI();
        }
    }
}

void GameObject::Serialize(rapidjson::Document& doc, rapidjson::Value& val)
{
    rapidjson::Value gameObjectVal(rapidjson::kObjectType);

    for (auto& com :comps)
    {
        com->Serialize(doc, gameObjectVal);
    }

    rapidjson::Value name(mName.c_str(), mName.size(), doc.GetAllocator());
    val.AddMember(name, gameObjectVal, doc.GetAllocator());
}
