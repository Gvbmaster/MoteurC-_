#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <Windows.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

class DX12Initializer {
public:
    DX12Initializer(HWND hwnd);
    ~DX12Initializer();

    HRESULT Initialize();
    void Cleanup();

private:
    HWND m_hwnd;
    ID3D12Device* m_d3d12Device;
    ID3D12CommandQueue* m_commandQueue;
    IDXGISwapChain3* m_swapChain;
};