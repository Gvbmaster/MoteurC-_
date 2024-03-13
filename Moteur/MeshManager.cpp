#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"



std::vector<D3D12_INPUT_ELEMENT_DESC> MeshManager::BuildShadersAndInputLayout(ID3DBlob** mvsByteCode, ID3DBlob** mpsByteCode) {
	*mvsByteCode = Utils::CompileShader(L"../Moteur/Shaders/color.hlsl", nullptr, "VS", "vs_5_0");
	*mpsByteCode = Utils::CompileShader(L"../Moteur/Shaders/color.hlsl", nullptr, "PS", "ps_5_0");

	std::vector<D3D12_INPUT_ELEMENT_DESC> desc1 =
	{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	return desc1;
	
}


MeshManager::MeshManager()
{
}


void MeshManager::DrawMeshes(ID3D12GraphicsCommandList* mCommandList, ID3D12DescriptorHeap* mCbvHeap, ID3D12RootSignature* mRootSignature, std::vector<Mesh*> vMesh) {

	for (int i = 0; i < vMesh.size(); i++) {
		

		mCommandList->SetGraphicsRootSignature(mRootSignature);
		mCommandList->SetPipelineState(mPso);
		mCommandList->IASetVertexBuffers(0, 1, &vMesh[i]->vbv);
		mCommandList->IASetIndexBuffer(&vMesh[i]->ibv);
		mCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mCommandList->SetGraphicsRootDescriptorTable(0, mCbvHeap->GetGPUDescriptorHandleForHeapStart());

		mCommandList->DrawIndexedInstanced(
			vMesh[i]->DrawArgs["box"].IndexCount,
			1, 0, 0, 0);
	}
}

void MeshManager::BuildPSO(std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout, ID3D12RootSignature* mRootSignature,ID3DBlob* mvsByteCode, ID3DBlob* mpsByteCode, DXGI_FORMAT mBackBufferFormat, bool m4xMsaaState, UINT m4xMsaaQuality, DXGI_FORMAT mDepthStencilFormat, ID3D12Device* md3dDevice)
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
	HRESULT hr = md3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPso));
	assert(hr== S_OK);
}