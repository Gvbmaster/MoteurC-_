#pragma once

#include "Utils.h"
#include "Component.h"

class Camera : public Component {
public:
    Camera(float fovY, float aspectRatio, float nearZ, float farZ);
    ~Camera();

    ComponentType getType() const override { return ComponentType::Camera; }
    void setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

    const DirectX::XMFLOAT4X4& getViewMatrix() const;
    const DirectX::XMFLOAT4X4& getProjectionMatrix() const;
    virtual void update(float deltaTime) override;

private:
    DirectX::XMFLOAT4X4 m_ViewMatrix;
    DirectX::XMFLOAT4X4 m_ProjectionMatrix;
};

