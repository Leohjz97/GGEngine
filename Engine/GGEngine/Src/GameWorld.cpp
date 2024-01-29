#include "Precompiled.h"
#include "GameWorld.h"
#include "GameObjectFactory.h"
#include "CameraService.h"
#include "RenderService.h"

#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace GGEngine;
using namespace GGEngine::Physics;

namespace
{
    CustomService TryServiceMake;
}

void GGEngine::GameWorld::SetCustomServiceMake(CustomService customService)
{
    TryServiceMake = customService;
}

void GameWorld::Initialize(uint32_t cap)
{
    ASSERT(!mInitialized, "GameWorld: is already initialized");
    mInitialized = true;

    for (auto& service : mServices)
    {
        service->Initialize();
    }

    mGameObjectSlots.resize(cap);
    mFreeSlots.resize(cap);
    std::iota(mFreeSlots.begin(), mFreeSlots.end(), 0);
}

void GameWorld::Terminate()
{
    ASSERT(!mUpdating, "GameWorld: can't terminate while updating");
    if (!mInitialized)
    {
        return;
    }

    for (auto& gameobject : mUpdateList)
    {
        DestoryGameObject(gameobject->GetHandle());
    }
    ProcessDestroyList();

    for (auto& service: mServices)
    {
        service->Terminate();
    }

    mInitialized = false;
}

void GameWorld::Update(float deltaTime)
{
    mUpdating = true;

    for (auto& gameObject : mUpdateList)
    {
        gameObject->Update(deltaTime);
    }
    
    for (auto& service : mServices)
    {
        service->Update(deltaTime);
    }

    mUpdating = false;

    ProcessDestroyList();
}

void GameWorld::Render()
{
    for (auto& service : mServices)
    {
        service->Render();
    }
}

void GameWorld::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    for (auto& service : mServices)
    {
        service->DebugUI();
    }
    for (auto& obj : mUpdateList)
    {
        obj->DebugUI();
    }
    PhysicsWorld::Get()->DebugUI();

    ImGui::End();
}

void GGEngine::GameWorld::EditorUI()
{
    ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    
    if (ImGui::Button("Save"))
    {
        SaveLevel("../../Assets/Templates/Level/Test_Level_save.json");
    }
    
    for (auto& obj : mUpdateList)
    {
        obj->EditorUI();
    }

    ImGui::End();
}

void GameWorld::LoadLevel(const std::filesystem::path& levelFile)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, levelFile.u8string().c_str(), "r");
    ASSERT(err == 0 && file != nullptr, "GameObject: failed to open level file %s", levelFile.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    fclose(file);

    rapidjson::Document doc;
    doc.ParseStream(readStream);

    auto services = doc["Services"].GetObj();
    for (auto& service : services)
    {
        const char* serviceName = service.name.GetString();
        //if (TryServiceMake(serviceName, service.value, *this))
        //{
        //    //it is a custom service
        //}
        //else 
        if (strcmp(serviceName,"CameraService") == 0)
        {
            auto cameraService = AddService<CameraService>();
        }
        else if (strcmp(serviceName, "RenderService") == 0)
        {
            auto renderService = AddService<RenderService>();
            if (service.value.HasMember("DirectionalLight"))
            {
                const auto& direction = service.value["DirectionalLight"].GetArray();
                const float x = direction[0].GetFloat();
                const float y = direction[1].GetFloat();
                const float z = direction[2].GetFloat();
                renderService->SetDirectionalLight({ x,y,z });
            }
        }else if (strcmp(serviceName, "MapService") == 0)
        {

        }
        else
        {
            ASSERT(false, "GameWorld: service %s is not defined", serviceName);
        }
    }

    uint32_t cap = static_cast<uint32_t>(doc["Capacity"].GetInt());
    Initialize(cap);

    if (doc.HasMember("Physics"))
    {
        const auto& physicsData = doc["Physics"].GetObj();

        if (physicsData.HasMember("Gravity"))
        {
            const auto& g = physicsData["Gravity"].GetArray();
            const auto& x = g[0].GetFloat();
            const auto& y = g[1].GetFloat();
            const auto& z = g[2].GetFloat();
            PhysicsWorld::Get()->SetGravity({ x,y,z });
        }
    }

    auto gameobjects = doc["GameObjects"].GetObj();
    for (auto& gameobject : gameobjects)
    {
        const char* templateFile = gameobject.value["Template"].GetString();
        auto obj = CreateGameObject(templateFile);

        if (obj != nullptr)
        {
            const char* name = gameobject.name.GetString();
            obj->SetName(name);

            if (gameobject.value.HasMember("Position"))
            {
                const auto& pos = gameobject.value["Position"].GetArray(); 
                const float x = pos[0].GetFloat();
                const float y = pos[1].GetFloat();
                const float z = pos[2].GetFloat();
                auto transform = obj->GetComponent<TransformComponent>();
                transform->position = { x,y,z };

                //RB
                auto rb = obj->GetComponent<RigidBodyComponent>();
                if (rb != nullptr)
                {
                    rb->SetPosition(transform->position);
                }
            }
        }
    }
}

void GGEngine::GameWorld::SaveLevel(const std::filesystem::path& levelFile)
{
    rapidjson::Document doc;
    rapidjson::Value objs(rapidjson::kObjectType);

    for (auto& obj : mUpdateList)
    {
        obj->Serialize(doc, objs);
    }
    
    doc.SetObject();
    doc.AddMember("GameObjects", objs, doc.GetAllocator());

    FILE* file = nullptr;
    auto err = fopen_s(&file, levelFile.u8string().c_str(), "w");
    ASSERT(err == 0 && file != nullptr, "GameObject: failed to open level file %s", levelFile.u8string().c_str());

    char writeBuffer[65536];
    rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));

    rapidjson::Writer<rapidjson::FileWriteStream>write(writeStream);
    doc.Accept(write);

    fclose(file);
}

GameObject* GameWorld::CreateGameObject(const std::filesystem::path& templateFile)
{
    ASSERT(mInitialized, "GameWorld: is not Initialized");

    if (mFreeSlots.empty())
    {
        ASSERT(false, "GameWorld: no free slots available");
        return nullptr;
    }

    const uint32_t freeSlot = mFreeSlots.back();
    mFreeSlots.pop_back();

    auto& slot = mGameObjectSlots[freeSlot];
    auto& newObject = slot.gameObject;
    newObject = std::make_unique<GameObject>();

    //attach components
    GameObjectFactory::Make(templateFile, *newObject);

    newObject->mWorld = this;
    newObject->mHandle.mIdx = freeSlot;
    newObject->mHandle.mGeneration = slot.generation;
    newObject->Initialize();
    mUpdateList.push_back(newObject.get());
    return newObject.get();
}


GameObject* GameWorld::GetGameObject(const GameObjectHandle& handle)
{
    if (!IsVaild(handle))
    {
        return nullptr;
    }

    return mGameObjectSlots[handle.mIdx].gameObject.get();
}



GameObject* GameWorld::FindGameObjectByName(const std::string& name)
{
    auto iter = mUpdateList.end();

    for (auto it = mUpdateList.begin(); it != mUpdateList.end(); ++it)
    {
        if ((*it)->GetName() == name) {
            iter = it;
            break;
        }
    }

    return iter == mUpdateList.end() ? nullptr : *iter;
}

void GameWorld::DestoryGameObject(const GameObjectHandle& handle)
{
    if (!IsVaild(handle))
    {
        return;
    }

    auto& slot = mGameObjectSlots[handle.mIdx];
    slot.generation++;
    mToBeDestroyed.push_back(handle.mIdx);
}

bool GameWorld::IsVaild(const GameObjectHandle& handle)
{
    if (handle.mIdx < 0 || handle.mIdx >= mGameObjectSlots.size())
    {
        return false;
    }
    if (mGameObjectSlots[handle.mIdx].generation != handle.mGeneration)
    {
        return false;
    }

    return true;
}

void GameWorld::ProcessDestroyList()
{
    ASSERT(!mUpdating, "GameWorld: cant destory when updating objects");
    for (auto& index : mToBeDestroyed)
    {
        auto& slot = mGameObjectSlots[index];

        GameObject* gameObject = slot.gameObject.get();
        ASSERT(!IsVaild(gameObject->GetHandle()), "GameWorld: Object is still alive");

        mUpdateList.erase(std::remove(mUpdateList.begin(), mUpdateList.end(), gameObject));
        gameObject->Terminate();
        slot.gameObject.reset();
        mFreeSlots.push_back(index);
    }

    mToBeDestroyed.clear();
}
