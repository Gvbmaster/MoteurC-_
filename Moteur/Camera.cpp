//#include "Camera.h"
//
//Camera::Camera()
//{   
//}
//
//Camera::~Camera()
//{
//}
//
//void Camera::setViewMatrix(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& at, const DirectX::XMFLOAT3& up)
//{
//    DirectX::XMStoreFloat4x4(&m_ViewMatrix, DirectX::XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up)));
//}
//
//void Camera::setProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ)
//{
//    DirectX::XMStoreFloat4x4(&m_ProjectionMatrix, DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ));
//}
