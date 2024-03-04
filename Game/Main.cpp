#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"
#include "../Moteur/Camera.h"
#include "../Moteur/Renderer.h"
#include "../Common/d3dUtil.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int main(){
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

return 0;
}

//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//    _In_opt_ HINSTANCE hPrevInstance,
//    _In_ LPWSTR    lpCmdLine,
//    _In_ int       nCmdShow)
//{
//    // Initialisation de la fenêtre
//    Window window;
//    window.init();
//    HWND hwnd = window.gethwnd();
//
//    // Initialisation de DirectX 12
//    DX12Initializer dxInitializer(hwnd);
//    HRESULT hr = dxInitializer.Initialize();
//    if (FAILED(hr)) {
//        return -1;
//    }
//
//    Camera camera;
//
//    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -5.0f);
//    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
//    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);
//
//    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
//        DirectX::XMLoadFloat3(&cameraPosition),
//        DirectX::XMLoadFloat3(&cameraTarget),
//        DirectX::XMLoadFloat3(&cameraUp)
//    );
//
//    viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
//
//
//    MSG msg;
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return (int)msg.wParam;
//}
