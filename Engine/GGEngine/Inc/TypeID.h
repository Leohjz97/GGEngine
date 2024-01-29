#pragma once
#include "Common.h"

namespace GGEngine
{
    enum class ComponentID
    {
        Transform,
        Mesh,
        Camera,
        FPSCameraController,
        ModelComponent,
        MeshComponent,
        ColliderComponent,
        RigidBodyComponet,
        Animator,
        CharacterController,
        ThirdPersonCamera,

        Count
    };

    enum class ServiceID
    {
        Camera,
        Render,
        Physics,

        Count
    };
}

#define SET_TYPE_ID(id)\
    static uint32_t StaticGetTypeID(){return static_cast <uint32_t>(id);}\
    uint32_t GetTypeID() const override {return StaticGetTypeID();}