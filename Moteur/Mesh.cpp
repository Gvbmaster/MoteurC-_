#include "pch.h"
#include "Mesh.h"


std::array<Vertex1, 8> Mesh::CreateMeshVertices() {

	std::array<Vertex1, 8> vertices =
	{
		Vertex1({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex1({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex1({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex1({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex1({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex1({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex1({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex1({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};

	return vertices;
}

std::array<std::uint16_t, 36> Mesh::CreateMeshIndices()
{
	std::array<std::uint16_t, 36> indices =
	{
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
	
	ID3D12Resource* VertexBufferGPU = nullptr;
	ID3D12Resource* VertexBufferUploader = nullptr;
	std::array<Vertex1, 8> vertices = CreateMeshVertices();
	const UINT64 vbByteSize = (UINT)vertices.size() * sizeof(Vertex1);
	std::array<std::uint16_t, 36> indices = CreateMeshIndices();

	VertexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, vertices.data(), vbByteSize, VertexBufferUploader);

	const UINT ibByteSize = 36 * sizeof(std::uint16_t);
	ID3D12Resource* IndexBufferGPU = nullptr;
	ID3D12Resource* IndexBufferUploader = nullptr;

	mBoxGeo = std::make_unique<MeshGeometry>();
	mBoxGeo->Name = "boxGeo";
	ThrowIfFailed(D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU));


	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU));
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice,mCommandList, vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice,mCommandList, indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex1);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	mBoxGeo->DrawArgs["box"] = submesh;

	IndexBufferGPU = Utils::CreateDefaultBuffer(md3dDevice, mCommandList, indices.data(), ibByteSize, IndexBufferUploader);
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