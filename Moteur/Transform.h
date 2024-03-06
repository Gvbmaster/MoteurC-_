#pragma once
#include "Utils.h"
#include <iostream>
//#include "../Common/d3dApp.h"
//#include "../Common/MathHelper.h"
//#include "../Common/UploadBuffer.h"

using namespace DirectX;
struct Transform {
	XMFLOAT3 vPos;
	XMFLOAT4X4 mPos;

	XMFLOAT3 vSca;
	XMFLOAT4X4 mSca;

	XMFLOAT3 vDir;
	XMFLOAT3 vRight;
	XMFLOAT3 vUp;
	XMFLOAT4 qRot;
	XMFLOAT4X4 mRot;

	XMFLOAT4X4 matrix;


	void Identity();

	void Translate(float front, float right, float up);
	void Upscale(float width, float depth, float height);
	void Rotate(float yaw, float pitch, float roll);

	void UpdateMatrix();
	void DisplayConsole(XMFLOAT4X4 mDis);
};