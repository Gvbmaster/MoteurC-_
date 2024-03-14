#pragma once
#include "MeshManager.h"

class Camera;

struct Vertex1;

class Mesh
{
public:
	Mesh() {};
	~Mesh() {};

	D3D12_VERTEX_BUFFER_VIEW vbv;
	D3D12_INDEX_BUFFER_VIEW ibv;
	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;
	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;
	
	D3D12_VERTEX_BUFFER_VIEW GetVertexBuffers();
	D3D12_INDEX_BUFFER_VIEW GetIbv();

	std::array<Vertex1, 8> CreateMeshVertices();
	std::array<std::uint16_t, 36> CreateMeshIndices();
	void CreateCubeMesh(MeshManager meshManager, ID3D12GraphicsCommandList* mCommandList, ID3D12Device* md3dDevice);
};


