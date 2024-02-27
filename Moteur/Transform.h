#pragma once
#include "../Common/d3dUtil.h"
//#include "../Common/d3dApp.h"
//#include "../Common/MathHelper.h"
//#include "../Common/UploadBuffer.h"

using namespace DirectX;
struct Transform {
	XMFLOAT3 vSca;
	XMFLOAT4X4 mSca;

	XMFLOAT3 vDir;
	XMFLOAT3 vRight;
	XMFLOAT3 vUp;
	XMFLOAT4 qRot;
	XMFLOAT4X4 mRot;

	XMFLOAT4X4 matrix;
	void Identity();
	void Rotate(float yaw, float pitch, float roll);
};