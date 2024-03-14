#include "pch.h"
#include "framework.h"
#include "Camera.h"
#include "GameObject.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(GameObject* gameObject, float fovY, float aspectRatio, float nearZ, float farZ)
{
	m_gameObject = gameObject;
	m_FovY = fovY;
	m_AspectRatio = aspectRatio;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	Update(aspectRatio);
}

void Camera::Update(float aspectRatio)
{
    Transform* transform = getTransform();

    if (transform != nullptr) {
        XMVECTOR position = XMLoadFloat3(&transform->vPos);
        XMVECTOR lookAt = position + XMLoadFloat3(&transform->vDir);
        XMVECTOR up = XMLoadFloat3(&transform->vUp);

        XMMATRIX viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
        XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

        XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(m_FovY, aspectRatio, m_NearZ, m_FarZ);
        XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
    }
}

void Camera::setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
    m_FovY = fovY;
    m_AspectRatio = aspectRatio;
    m_NearZ = nearZ;
    m_FarZ = farZ;

    XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(m_FovY, m_AspectRatio, m_NearZ, m_FarZ);
    XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
}


const XMFLOAT4X4& Camera::getViewMatrix()
{
    return m_ViewMatrix;
}

const XMFLOAT4X4& Camera::getProjectionMatrix()
{
    return m_ProjectionMatrix;
}

const XMFLOAT4X4& Camera::getWorldMatrix()
{
	Transform* transform = getTransform();
    if (transform != nullptr) {
		return transform->matrix;
	}
    else {
		return m_ProjectionMatrix;
	}
}

ComponentType Camera::getType() const
{
	return ComponentType::Camera;
}

Transform* Camera::getTransform()
{
	return dynamic_cast<Transform*>(m_gameObject->getComponent(ComponentType::Transform));
}