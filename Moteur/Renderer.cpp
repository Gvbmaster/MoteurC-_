#include "pch.h"
#include "Renderer.h"

std::vector<D3D12_INPUT_ELEMENT_DESC> Meshes::BuildShadersAndInputLayout(ID3DBlob** mvsByteCode, ID3DBlob** mpsByteCode) {
	*mvsByteCode = Utils::CompileShader(L"../Moteur/Shaders/color.hlsl", nullptr, "VS", "vs_5_0");
	*mpsByteCode = Utils::CompileShader(L"../Moteur/Shaders/color.hlsl", nullptr, "PS", "ps_5_0");

	std::vector<D3D12_INPUT_ELEMENT_DESC> desc1 =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	return desc1;
	
}


Vertex1 Meshes::CreateMeshVertices() {

	Vertex1 vertices[] =
	{
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) },
	{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) },
	{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) },
	{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) },
	{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) },
	{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) },
	{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) },
	{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) }
	};

	return vertices;
}

std::uint16_t Meshes::CreateMeshIndices()
{
	std::uint16_t indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,
		// back face
		4, 6, 5,
		4, 7, 6,
		// left face
		4, 5, 1,
		4, 1, 0,
		// right face
		3, 2, 6,
		3, 6, 7,
		// top face
		1, 5, 6,
		1, 6, 2,
		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	
	return indices;
}

void Meshes::DrawMeshes(ID3D12GraphicsCommandList* mCommandList, ID3D12Device* md3dDevice,Vertex1 vertices[], uint16_t indices[]) {
	const UINT64 vbByteSize = 8 * sizeof(Vertex1);
	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* VertexBufferUploader = nullptr;
	VertexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, vertices, vbByteSize, VertexBufferUploader);

	const UINT ibByteSize = 36 * sizeof(std::uint16_t);
	ID3D12Resource* IndexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferUploader = nullptr;
	IndexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, indices, ibByteSize, IndexBufferUploader);
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = ibByteSize;
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(Vertex1);
	vbv.SizeInBytes = 8 * sizeof(Vertex1);
	D3D12_VERTEX_BUFFER_VIEW vertexBuffers[1] = { vbv };
	vMeshList.push_back(mCommandList);
	for (int i = 0; i < vMeshList.size(); i++) {
		mCommandList->IASetVertexBuffers(0, 1, vertexBuffers);
		mCommandList->IASetIndexBuffer(&ibv);
		mCommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
	}
}

void Meshes::BuildPSO(std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout, ID3D12RootSignature* mRootSignature,ID3DBlob* mvsByteCode, ID3DBlob* mpsByteCode, DXGI_FORMAT mBackBufferFormat, bool m4xMsaaState, UINT m4xMsaaQuality, DXGI_FORMAT mDepthStencilFormat, ID3D12Device* md3dDevice, ID3D12PipelineState* mPSO)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.pRootSignature = mRootSignature;
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = mBackBufferFormat;
	psoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	psoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = mDepthStencilFormat;
	HRESULT hr = md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPSO));
	assert(hr);
}