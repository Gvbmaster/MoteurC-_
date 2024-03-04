#include "Renderer.h"
void meshCube() {
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

	const UINT64 vbByteSize = 8 * sizeof(Vertex1);
	ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), vertices, vbByteSize, VertexBufferUploader);

	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(Vertex1);
	vbv.SizeInBytes = 8 * sizeof(Vertex1);
	D3D12_VERTEX_BUFFER_VIEW vertexBuffers[1] = { vbv };


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
	const UINT ibByteSize = 36 * sizeof(std::uint16_t);
	ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;
	ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;
	IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(), mCommandList.Get(), indices, ibByteSize, IndexBufferUploader);
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = ibByteSize;

	mCommandList->IASetVertexBuffers(0, 1, vertexBuffers);
	mCommandList->IASetIndexBuffer(&ibv);
}