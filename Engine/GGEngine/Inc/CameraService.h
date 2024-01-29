#pragma once
#include "Service.h"

namespace GGEngine
{
    class CameraComponent;

    class CameraService final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::Camera);

        const Graphic::Camera& GetMain() const;
    private:
        friend class CameraComponent;
        void Register(const CameraComponent* cameraComponent);
        void Unregister(const CameraComponent* cameraComponent);

        using CameraEntries = std::vector<const CameraComponent*>;
        CameraEntries mCameraEntries;
        const CameraComponent* mMainCamera = nullptr;
     };

}