#include "DX12Initializer.h"

DX12Initializer::DX12Initializer(HWND hwnd) : m_hwnd(hwnd), m_d3d12Device(nullptr), m_commandQueue(nullptr), m_swapChain(nullptr) {
}

DX12Initializer::~DX12Initializer() {
    Cleanup();
}

HRESULT DX12Initializer::Initialize() {
    HRESULT hr;
    IDXGIFactory4* dxgiFactory;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3d12Device));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;

    hr = m_d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = 800;
    swapChainDesc.Height = 600;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain1* swapChain;
    hr = dxgiFactory->CreateSwapChainForHwnd(m_commandQueue, m_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    hr = swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
    swapChain->Release();
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    dxgiFactory->Release();

    return S_OK;
}

void DX12Initializer::Cleanup() {
    if (m_swapChain) {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    if (m_commandQueue) {
        m_commandQueue->Release();
        m_commandQueue = nullptr;
    }

    if (m_d3d12Device) {
        m_d3d12Device->Release();
        m_d3d12Device = nullptr;
    }
}