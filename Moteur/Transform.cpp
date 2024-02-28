#include "Transform.h"


void Transform::Identity() {
	vPos = { 0, 0, 0 };
	XMStoreFloat4x4(&mPos, XMMatrixIdentity());

	vSca = { 1, 1, 1 };
	XMStoreFloat4x4(&mSca, XMMatrixIdentity());

	vDir = { 1, 0, 0 };
	vRight = { 0, 1, 0 };
	vUp = { 0, 0, 1 };
	qRot = { 0, 0, 0, 1 };
	XMStoreFloat4x4(&mRot, XMMatrixIdentity());

	XMStoreFloat4x4(&matrix, XMMatrixIdentity());
}

void Transform::Translate(float front, float right, float up) {
	vPos.x += front;
	vPos.y += right;
	vPos.z += up;
	XMVECTOR v_vPos = XMLoadFloat3(&vPos);
	XMMATRIX m_mPos = XMLoadFloat4x4(&mPos);
	m_mPos *= XMMatrixTranslationFromVector(v_vPos);
	XMStoreFloat4x4(&mPos, m_mPos);
	UpdateMatrix();
	
}

void Transform::Upscale(float width, float depth, float height) {
	vSca = { width, depth, height };
	XMVECTOR v_vSca = XMLoadFloat3(&vSca);
	XMMATRIX m_mSca = XMLoadFloat4x4(&mSca);
	m_mSca *= XMMatrixScalingFromVector(v_vSca);
	XMStoreFloat4x4(&mSca, m_mSca);
	UpdateMatrix();
}


void Transform::Rotate(float yaw, float pitch, float roll) {
	XMMATRIX m_mRot = XMLoadFloat4x4(&mRot);
	m_mRot *= XMMatrixRotationRollPitchYaw(pitch,yaw,roll);
	XMStoreFloat4x4(&mRot, m_mRot);
	UpdateMatrix();
}


void Transform::UpdateMatrix() {
	XMMATRIX m_matrix = XMLoadFloat4x4(&mSca);
	XMStoreFloat4x4(&matrix, m_matrix);
}