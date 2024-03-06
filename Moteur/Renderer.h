#pragma once
//#include "pch.h"
#include "framework.h"
#include "Utils.h"
#include "InitApp.h"
#include "vector"

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace DirectX;

struct Vertex1
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct Vertex2
{
	XMFLOAT3 Pos;
	XMFLOAT2 uv;
};



class Meshes{

public:
	std::vector<ID3D12GraphicsCommandList*> vMeshList;
	std::vector<D3D12_INPUT_ELEMENT_DESC> BuildShadersAndInputLayout(ID3DBlob** mvsByteCode, ID3DBlob** mpsByteCode);
	void CreateMeshCube(ID3D12Device* md3dDevice, ID3D12GraphicsCommandList* mCommandList);
	void DrawMeshes(ID3D12GraphicsCommandList* mCommandList);
	void BuildPSO(std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout, ID3D12RootSignature* mRootSignature, ID3DBlob* mvsByteCode, ID3DBlob* mpsByteCode, DXGI_FORMAT mBackBufferFormat, bool m4xMsaaState, UINT m4xMsaaQuality, DXGI_FORMAT mDepthStencilFormat, ID3D12Device* md3dDevice, ID3D12PipelineState* mPSO);


	Meshes(){};
	~Meshes() {};

};