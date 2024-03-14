#pragma once

#include "Component.h"
#include "Transform.h"
#include <DirectXMath.h>

using namespace DirectX;

class Camera : public Component {
public:
    Camera();
    ~Camera();

    void Init(GameObject* gameObject, float fovY, float aspectRatio, float nearZ, float farZ);
    void Update(float aspectRatio);
    void setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

    const XMFLOAT4X4& getViewMatrix();
    const XMFLOAT4X4& getProjectionMatrix();
    const XMFLOAT4X4& getWorldMatrix();

    ComponentType getType() const override;
    Transform* getTransform() override;

private:
    float m_FovY;
    float m_NearZ;
    float m_FarZ;
    float m_AspectRatio;

    XMFLOAT4X4 m_ViewMatrix;
    XMFLOAT4X4 m_ProjectionMatrix;
};

