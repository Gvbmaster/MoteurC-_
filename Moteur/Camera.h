#pragma once
#include "../Common/d3dUtil.h"
#include "Component.h"

class Camera : public Component {
    public:
        Camera();
        ~Camera();

        void setViewMatrix(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at, const DirectX::XMFLOAT3& up);
        void setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

        const DirectX::XMFLOAT4X4& getViewMatrix() const { return m_ViewMatrix; }
        const DirectX::XMFLOAT4X4& getProjectionMatrix() const { return m_ProjectionMatrix; }

        virtual void update(float deltaTime) override;

    private:
        DirectX::XMFLOAT4X4 m_ViewMatrix;
        DirectX::XMFLOAT4X4 m_ProjectionMatrix;
};
