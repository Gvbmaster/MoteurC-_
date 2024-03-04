#pragma once
#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"
#include "../Moteur/Camera.h"
#include "../Moteur/Renderer.h"
#include "../Common/d3dUtil.h"
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

std::vector<D3D12_INPUT_ELEMENT_DESC> desc1 =
{
{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};


std::vector<D3D12_INPUT_ELEMENT_DESC> desc2 =
{
{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};

void meshCube();