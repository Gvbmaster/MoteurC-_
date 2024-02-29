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
    DX12Initializer(HWND hwnd, UINT frameCount);
    ~DX12Initializer();

    HRESULT Initialize();
    void Cleanup();
    void CreateRenderTargetView();
    void Draw();

private:
    HWND m_hwnd;
    UINT m_frameCount;
    UINT m_frameIndex;
    UINT m_rtvDescriptorSize;
    ID3D12Device* m_d3d12Device;
    ID3D12CommandQueue* m_commandQueue;
    IDXGISwapChain3* m_swapChain;
    ID3D12DescriptorHeap* m_rtvHeap;
    ID3D12Resource* m_renderTargets[2];
    ID3D12CommandAllocator* m_commandAllocator[60];
    ID3D12GraphicsCommandList* m_commandList;
};
