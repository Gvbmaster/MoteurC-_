#pragma once

#include "framework.h"

class DXApp {
public:
    DXApp(HINSTANCE hInstance);
    ~DXApp();

    HRESULT Initialize(UINT frameCount);
    void Cleanup();
    void Run();

private:
    HINSTANCE m_hInstance;
    HWND m_hwnd;
    UINT m_frameCount;
    UINT m_frameIndex;
    UINT m_rtvDescriptorSize;
    Microsoft::WRL::ComPtr<ID3D12Device> m_d3d12Device;
    Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2];
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

    void CreateRenderTargetView();
    void ClearBackBuffer();
    void Draw3DObjects();
    void Draw();
    void Present();
    void Render();
};
