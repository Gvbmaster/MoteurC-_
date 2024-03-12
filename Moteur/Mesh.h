#pragma once
#include "Renderer.h"

struct Vertex1;

class Mesh
{
public:
	D3D12_VERTEX_BUFFER_VIEW vbv;
	D3D12_INDEX_BUFFER_VIEW ibv;
	
	D3D12_VERTEX_BUFFER_VIEW GetVertexBuffers();
	D3D12_INDEX_BUFFER_VIEW GetIbv();

	Vertex1* CreateMeshVertices();
	std::uint16_t* CreateMeshIndices();
	void CreateCubeMesh(MeshManager meshManager, ID3D12GraphicsCommandList* mCommandList, ID3D12Device* md3dDevice);

	Mesh() {};
	~Mesh() {};

};


