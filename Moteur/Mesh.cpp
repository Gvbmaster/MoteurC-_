#include "pch.h"
#include "Mesh.h"


Vertex1* Mesh::CreateMeshVertices() {

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

std::uint16_t* Mesh::CreateMeshIndices()
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

void Mesh::CreateCubeMesh(MeshManager meshManager, ID3D12GraphicsCommandList* mCommandList, ID3D12Device* md3dDevice) {
	const UINT64 vbByteSize = 8 * sizeof(Vertex1);
	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* VertexBufferUploader = nullptr;
	Vertex1* vertices = CreateMeshVertices();
	uint16_t* indices = CreateMeshIndices();
	VertexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, vertices, vbByteSize, VertexBufferUploader);

	const UINT ibByteSize = 36 * sizeof(std::uint16_t);
	ID3D12Resource* IndexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferUploader = nullptr;

	IndexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, indices, ibByteSize, IndexBufferUploader);
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = DXGI_FORMAT_R16_UINT;
	ibv.SizeInBytes = ibByteSize;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = sizeof(Vertex1);
	vbv.SizeInBytes = 8 * sizeof(Vertex1);
	meshManager.vMeshCommandList.push_back(mCommandList);

}

D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertexBuffers() {
	return vbv;
}
D3D12_INDEX_BUFFER_VIEW Mesh::GetIbv() {
	return ibv;
}