#include "DXApp.h"
#include "Window.h"

DXApp::DXApp(HINSTANCE hInstance) :
    m_hInstance(hInstance),
    m_hwnd(nullptr),
    m_frameCount(2), 
    m_frameIndex(0),
    m_rtvDescriptorSize(0),
    m_d3d12Device(nullptr),
    m_dxgiFactory(nullptr),
    m_commandQueue(nullptr),
    m_swapChain(nullptr),
    m_rtvHeap(nullptr),
    m_commandAllocator(nullptr),
    m_commandList(nullptr)
{
    
}

DXApp::~DXApp() {
    Cleanup();
}

HRESULT DXApp::Initialize(UINT frameCount) {
    m_frameCount = frameCount;

    // Initialiser la fenêtre
    Window window;
    if (!window.init()) {
        return E_FAIL;
    }
    m_hwnd = window.gethwnd();

    // Initialiser DirectX 12
    HRESULT hr = S_OK;
    hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
    if (FAILED(hr)) {
        return hr;
    }

    hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_d3d12Device));
    if (FAILED(hr)) {
        return hr;
    }

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;
    hr = m_d3d12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (FAILED(hr)) {
        return hr;
    }
    // Initialiser la swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;

    // Remplir la description de la swap chain
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
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

    // Créer la swap chain
    IDXGISwapChain1* swapChain;
    hr = m_dxgiFactory->CreateSwapChainForHwnd(m_commandQueue.Get(), m_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain);
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
}

void DXApp::Cleanup() {
    m_d3d12Device.Reset();
    m_dxgiFactory.Reset();
}

void DXApp::Run() {
    MSG msg = {};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Render();
        }
    }
}

void DXApp::CreateRenderTargetView() {
    HRESULT hr;

    m_rtvDescriptorSize = m_d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = m_frameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = m_d3d12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
    if (FAILED(hr)) {
        Cleanup();
        return;
    }

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < m_frameCount; ++i) {
        hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
        if (FAILED(hr)) {
            Cleanup();
            return;
        }

        m_d3d12Device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);
        rtvHandle += m_rtvDescriptorSize;
    }
}


void DXApp::ClearBackBuffer() {
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), nullptr);

	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	m_commandList->Close();
}

void DXApp::Draw3DObjects() {
    ThrowIfFailed(m_commandAllocator->Reset());
    ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), nullptr));

    // Définir les shaders, les tampons de vertex, les constant buffers, etc.

    // Définir les états de rendu, tels que les états de rasterizer, de blend, de depth-stencil, etc.

    // Dessiner les objets 3D en utilisant les appels API DirectX 12 appropriés
    // Par exemple :
    m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_commandList->DrawIndexedInstanced(...); // Utilisez cette fonction pour dessiner des objets indexés
    // Ou m_commandList->DrawInstanced(...) si vous ne travaillez pas avec des index buffers

    // Terminer la commande de liste
    ThrowIfFailed(m_commandList->Close());

    // Exécuter la commande de liste
    ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
    m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Attendre que la commande de liste soit exécutée
    WaitForGPU();

    // Présenter la frame
    Present();
}


void DXApp::Render() {
    // Effacer l'arrière-plan avec une couleur spécifique
    ClearBackBuffer();
    Draw3DObjects();
    // Dessiner les effets visuels et l'interface utilisateur
    Draw();
    Present();
}

