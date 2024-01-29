#pragma once
#include "Service.h"

namespace GGEngine
{
    class CameraService;
    class ModelComponent;
    class MeshComponent;
    class TransformComponent;

    class RenderService final : public Service
    {
    public:
        SET_TYPE_ID(ServiceID::Render);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void Render() override;
        void DebugUI() override;

        void SetDirectionalLight(const GGMath::Vector3& direction);

    private:
        friend class ModelComponent;
        void Register(const ModelComponent* modelComponent);
        void UnRegister(const ModelComponent* modelComponent);

        friend class MeshComponent;
        void Register(const MeshComponent* meshComponent);
        void UnRegister(const MeshComponent* meshComponent);

        struct Entry
        {
            const ModelComponent* modelComponent = nullptr;
            const MeshComponent* meshComponent = nullptr;
            const TransformComponent* transforComponent = nullptr;
            Graphic::RenderGroup renderGroup;
        };

        using RenderEntities = std::vector<Entry>;
        RenderEntities mRenderEntities;

        const CameraService* mCameraService = nullptr;

        Graphic::DirectionaLight mDirectionalLight;
        Graphic::StandardEffect mStandardEffect;
        Graphic::ShadowEffect mShadowEffect;

        float mFPS;

    };

}