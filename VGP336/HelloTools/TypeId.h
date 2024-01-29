#pragma once
#include <GGEngine/Inc/TypeID.h>

enum class NewComponentID
{
    AnimationChangeComponenetID = static_cast<uint32_t>(GGEngine::ComponentID::Count) //get count id number, not start at 0
};

enum class NewServiceID
{
    AnimationChangeInputServiceID=static_cast<uint32_t>(GGEngine::ServiceID::Count)
};

