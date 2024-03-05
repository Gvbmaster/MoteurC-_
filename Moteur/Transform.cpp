#include "pch.h"
#include "framework.h"
#include "Transform.h"


void Transform::Identity() {
	vPos = { 0, 0, 0 };
	XMStoreFloat4x4(&mPos, XMMatrixIdentity());

	vSca = { 1, 1, 1 };
	mSca = mPos;

	vDir = { 1, 0, 0 };
	vRight = { 0, 1, 0 };
	vUp = { 0, 0, 1 };
	qRot = { 0, 0, 0, 1 };
	mRot = mPos;

	matrix = mPos;
}

void Transform::Translate(float front, float right, float up) {
	vPos.x += front;
	vPos.y += right;
	vPos.z += up;
	XMMATRIX m_mPos = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	XMStoreFloat4x4(&mPos, m_mPos);
	UpdateMatrix();
}

//Make Upscale from a ratio
void Transform::Upscale(float width, float depth, float height) {
	vSca = { width, depth, height };
	XMMATRIX m_mSca = XMLoadFloat4x4(&mSca);
	m_mSca *= XMMatrixScaling(vSca.x, vSca.y, vSca.z);
	XMStoreFloat4x4(&mSca, m_mSca);
	UpdateMatrix();
}

//Make rotate from yaw, pitch and roll in radians
void Transform::Rotate(float yaw, float pitch, float roll) {
	XMVECTOR v_vDir = XMLoadFloat3(&vDir);
	XMVECTOR v_vRight = XMLoadFloat3(&vRight);
	XMVECTOR v_vUp = XMLoadFloat3(&vUp);

	XMVECTOR v_Rot;
	XMVECTOR v_vTemp;
	v_vTemp = XMQuaternionRotationAxis(v_vDir, yaw);
	v_Rot = v_vTemp;
	v_vTemp = XMQuaternionRotationAxis(v_vRight, pitch);
	v_Rot = XMQuaternionMultiply(v_Rot, v_vTemp);
	v_vTemp = XMQuaternionRotationAxis(v_vUp, roll);
	v_Rot = XMQuaternionMultiply(v_Rot, v_vTemp);


	XMVECTOR v_qRot = XMLoadFloat4(&qRot);
	v_qRot = XMQuaternionMultiply(v_qRot, v_Rot);
	XMStoreFloat4(&qRot, v_qRot);

	XMMATRIX m_mRot = XMMatrixRotationQuaternion(v_qRot);
	XMStoreFloat4x4(&mRot, m_mRot);

	vRight.x = mRot._11;
	vRight.y = mRot._12;
	vRight.z = mRot._13;
	vUp.x = mRot._21;
	vUp.y = mRot._22;
	vUp.z = mRot._23;
	vDir.x = mRot._31;
	vDir.y = mRot._32;
	vDir.z = mRot._33;

	UpdateMatrix();
}


void Transform::UpdateMatrix() {
	XMMATRIX m_matrix = XMLoadFloat4x4(&mSca);
	XMMATRIX m_mPos = XMLoadFloat4x4(&mPos);
	XMMATRIX m_mRot = XMLoadFloat4x4(&mRot);
	m_matrix = m_matrix * m_mPos * m_mRot;
	XMStoreFloat4x4(&matrix, m_matrix);
}


void Transform::DisplayConsole(XMFLOAT4X4 mDis) {
	std::cout << mDis._11 << " " << mDis._12 << " " << mDis._13 << " " << mDis._14 << "\n";
	std::cout << mDis._21 << " " << mDis._22 << " " << mDis._23 << " " << mDis._24 << "\n";
	std::cout << mDis._31 << " " << mDis._32 << " " << mDis._33 << " " << mDis._34 << "\n";
	std::cout << mDis._41 << " " << mDis._42 << " " << mDis._43 << " " << mDis._44 << "\n";
}