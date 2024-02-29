#include "DX12Initializer.h"

DX12Initializer::DX12Initializer(HWND hwnd, UINT frameCount) :
    m_hwnd(hwnd),
    m_frameCount(frameCount),
    m_frameIndex(0),
    m_rtvDescriptorSize(0),
    m_d3d12Device(nullptr),
    m_commandQueue(nullptr),
    m_swapChain(nullptr),
    m_rtvHeap(nullptr),
    m_commandAllocator(),
    m_commandList(nullptr) {
}


DX12Initializer::~DX12Initializer() {
    Cleanup();
}

HRESULT DX12Initializer::Initialize() {
    HRESULT hr;

    // Créer une factory DXGI
    IDXGIFactory4* dxgiFactory;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    // Créer un périphérique DirectX 12
    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3d12Device));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    // Créer une file de commandes
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

    // Créer la description de la swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = 800;
    swapChainDesc.Height = 600;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = m_frameCount;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // Créer la chaîne d'échange
    IDXGISwapChain1* swapChain;
    hr = dxgiFactory->CreateSwapChainForHwnd(m_commandQueue, m_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    // Obtenir l'interface IDXGISwapChain3
    hr = swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
    swapChain->Release();
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    // Libérer la factory DXGI
    dxgiFactory->Release();

    // Créer les vues de rendu
    CreateRenderTargetView();

    // Obtenez la taille d'une vue de rendu
    m_rtvDescriptorSize = m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Créer le tas de descripteurs pour les vues de rendu
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = m_frameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDesc.NodeMask = 0;
    hr = m_d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr)) {
        Cleanup();
        return hr;
    }

    // Créer les tampons de commandes pour chaque frame
    for (UINT i = 0; i < m_frameCount; ++i) {
        hr = m_d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i]));
        if (FAILED(hr)) {
            Cleanup();
            return hr;
        }
    }

    return S_OK;
}



void DX12Initializer::Cleanup() {
    // Libérer les ressources
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

void DX12Initializer::CreateRenderTargetView() {
    HRESULT hr;

    UINT rtvDescriptorSize = m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < m_frameCount; ++i) {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        if (FAILED(hr)) {
            Cleanup();
            return;
        }

        m_d3d12Device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);

        rtvHandle.ptr += rtvDescriptorSize;
    }
}



void DX12Initializer::Draw() {
    // Vérifier que la chaîne d'échange est initialisée
    if (!m_swapChain) {
        return;
    }

    // Obtenir l'index de la frame courante
    UINT frameIndex = m_swapChain->GetCurrentBackBufferIndex();

    // Définir la couleur de fond (noir)
    FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    // Obtenez le tampon de rendu actuel
    ID3D12Resource* backBuffer;
    HRESULT hr = m_swapChain->GetBuffer(frameIndex, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) {
        return;
    }

    // Définir la cible de rendu
    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D.MipSlice = 0;
    rtvDesc.Texture2D.PlaneSlice = 0;

    // Créer une vue de rendu pour le tampon de rendu actuel
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
    rtvHandle.ptr = m_rtvHeap->GetCPUDescriptorHandleForHeapStart().ptr + frameIndex * m_rtvDescriptorSize;
    m_d3d12Device->CreateRenderTargetView(backBuffer, &rtvDesc, rtvHandle);

    // Libérer le tampon de rendu
    backBuffer->Release();

    // Obtenez la file de commandes
    ID3D12GraphicsCommandList* commandList;
    hr = m_d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
    if (FAILED(hr)) {
        return;
    }

    // Définir les vues de rendu
    commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Effacer le tampon de rendu avec la couleur de fond
    commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Terminer la liste de commandes
    commandList->Close();

    // Exécuter la liste de commandes
    ID3D12CommandList* ppCommandLists[] = { commandList };
    m_commandQueue->ExecuteCommandLists(1, ppCommandLists);

    // Présenter la frame
    m_swapChain->Present(1, 0);
}
