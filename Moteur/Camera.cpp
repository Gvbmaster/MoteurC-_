#include "pch.h"
#include "Camera.h"

Camera::Camera() : Component(nullptr) {
    DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixIdentity());
}

Camera::~Camera() {
}

void Camera::setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
    DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ));
}


const DirectX::XMFLOAT4X4& Camera::getViewMatrix() const {
    return m_ViewMatrix;
}

const DirectX::XMFLOAT4X4& Camera::getProjectionMatrix() const {
    return m_ProjectionMatrix;
}

void Camera::update(float deltaTime) {
    Component::update(deltaTime);
}
