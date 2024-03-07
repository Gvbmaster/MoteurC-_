#pragma once
#include "Component.h"

class Camera : public Component {
public:
    Camera();
    ~Camera();

    void setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

    const DirectX::XMFLOAT4X4& getViewMatrix() const;
    const DirectX::XMFLOAT4X4& getProjectionMatrix() const;

    virtual void update(float deltaTime) override;

private:
    DirectX::XMFLOAT4X4 m_ProjectionMatrix;
};
