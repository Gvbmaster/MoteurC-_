//***************************************************************************************
// InitApp.cpp by Frank Luna (C) 2015 All Rights Reserved.
//***************************************************************************************
#include "pch.h"
#include "framework.h"

#include "initApp.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Camera.h"






using namespace std;
using namespace DirectX;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return InitApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
}

InitApp* InitApp::mApp = nullptr;
InitApp* InitApp::GetApp()
{
	return mApp;
}

InitApp::InitApp(HINSTANCE hInstance)
	: mhAppInst(hInstance)
{
	// Only one InitApp can be constructed.
	assert(mApp == nullptr);
	mApp = this;
	mSwapChainBuffer[0] = nullptr;
	mSwapChainBuffer[1] = nullptr;
	mDepthStencilBuffer = nullptr;
}

InitApp::~InitApp()
{
	if (md3dDevice != nullptr)
		FlushCommandQueue();
}

HINSTANCE InitApp::AppInst()const
{
	return mhAppInst;
}

HWND InitApp::MainWnd()const
{
	return mhMainWnd;
}

float InitApp::AspectRatio()const
{
	return static_cast<float>(mClientWidth) / mClientHeight;
}

bool InitApp::Get4xMsaaState()const
{
	return m4xMsaaState;
}

void InitApp::Set4xMsaaState(bool value)
{
	if (m4xMsaaState != value)
	{
		m4xMsaaState = value;

		// Recreate the swapchain and buffers with new multisample settings.
		CreateSwapChain();
		OnResize();
	}
}

void InitApp::Update(const GameTimer& gt)
{

}

void InitApp::Draw(const GameTimer& gt, std::vector<Mesh*> vMesh) {
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(mDirectCmdListAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

	// Indicate a state transition on the resource usage.
	CD3DX12_RESOURCE_BARRIER barrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);

	mCommandList->ResourceBarrier(1, &barrierTransition);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Clear the back buffer and depth buffer.
	mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::LightSteelBlue, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	auto backBufferView = CurrentBackBufferView();
	auto depthStencilView = DepthStencilView();

	mCommandList->OMSetRenderTargets(1, &backBufferView, true, &depthStencilView);

	// Draw meshes
	ID3D12DescriptorHeap* descriptorHeaps[] = { mCbvHeap };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	meshManager.DrawMeshes(mCommandList, mCbvHeap, mRootSignature, vMesh);

	// Finish rendering setup
	barrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(
		CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	mCommandList->ResourceBarrier(1, &barrierTransition);

	// Done recording commands.
	ThrowIfFailed(mCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Swap the back and front buffers
	ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

	// Wait until frame commands are complete.
	FlushCommandQueue();
}


int InitApp::Run()
{
	int a = 0;
	MSG msg = { 0 };

	mTimer.Reset();
	Mesh mesh;
	Camera camera ;
	camera.setProjectionMatrix(0.25f * XM_PI, AspectRatio(), 1.0f, 1000.0f);

	ThrowIfFailed(mDirectCmdListAlloc->Reset());
	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

	mesh.CreateCubeMesh(meshManager, mCommandList, md3dDevice);
	meshManager.vMesh.push_back(&mesh);

	ThrowIfFailed(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	FlushCommandQueue();

	std::vector<D3D12_INPUT_ELEMENT_DESC> descs = meshManager.BuildShadersAndInputLayout(&mvsByteCode, &mpsByteCode);
	meshManager.BuildPSO(descs, mRootSignature, mvsByteCode, mpsByteCode, mBackBufferFormat, m4xMsaaState, m4xMsaaQuality, mDepthStencilFormat, md3dDevice);

	// Main message loop
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			mTimer.Tick();

			if (!mAppPaused)
			{
				// Mettre � jour la cam�ra
				camera.update(mTimer.DeltaTime());

				CalculateFrameStats();
				Update(mTimer);

				SetCameraMatrices(camera.getViewMatrix(), camera.getProjectionMatrix());

				Draw(mTimer, meshManager.vMesh);
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return (int)msg.wParam;
}


bool InitApp::Initialize()
{
	if (!InitMainWindow())
		return false;

	if (!InitDirect3D())
		return false;

	// Do the initial resize code.
	OnResize();

	return true;
}

void InitApp::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRtvHeap)));


	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap)));
}

void InitApp::OnResize()
{
	assert(md3dDevice);
	assert(mSwapChain);
	assert(mDirectCmdListAlloc);

	// Flush before changing any resources.
	FlushCommandQueue();

	ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc, nullptr));

	// Release the previous resources we will be recreating.

	for (int i = 0; i < SwapChainBufferCount; ++i) {

		if (mSwapChainBuffer[i] ) {
			mSwapChainBuffer[i]->Release();
		}
	}
	if (mDepthStencilBuffer) {
		mDepthStencilBuffer->Release();
	}

	// Resize the swap chain.
	ThrowIfFailed(mSwapChain->ResizeBuffers(
		SwapChainBufferCount,
		mClientWidth, mClientHeight,
		mBackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mCurrBackBuffer = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < SwapChainBufferCount; i++)
	{
		ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i])));
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i], nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRtvDescriptorSize);
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mClientWidth;
	depthStencilDesc.Height = mClientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	// Correction 11/12/2016: SSAO chapter requires an SRV to the depth buffer to read from 
	// the depth buffer.  Therefore, because we need to create two views to the same resource:
	//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
	// we need to create the depth buffer resource with a typeless format.  
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	CD3DX12_HEAP_PROPERTIES toto(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(md3dDevice->CreateCommittedResource(&toto,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&mDepthStencilBuffer)));

	// Create descriptor to mip level 0 of entire resource using the format of the resource.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, &dsvDesc, DepthStencilView());

	// Transition the resource from its initial state to be used as a depth buffer.
	CD3DX12_RESOURCE_BARRIER transition = CD3DX12_RESOURCE_BARRIER::Transition(
		mDepthStencilBuffer,
		D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATE_DEPTH_WRITE
	);
	mCommandList->ResourceBarrier(1, &transition);

	// Execute the resize commands.
	ThrowIfFailed(mCommandList->Close());
	ID3D12CommandList* cmdsLists[] = { mCommandList };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommandQueue();

	// Update the viewport transform to cover the client area.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, mClientWidth, mClientHeight };
}

LRESULT InitApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			mAppPaused = true;
			mTimer.Stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if (md3dDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (mMinimized)
				{
					mAppPaused = false;
					mMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (mMaximized)
				{
					mAppPaused = false;
					mMaximized = false;
					OnResize();
				}
				else if (mResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing = true;
		mTimer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing = false;
		mTimer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}
		else if ((int)wParam == VK_F2)
			Set4xMsaaState(!m4xMsaaState);

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool InitApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow(L"MainWnd", mMainWndCaption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, 0);
	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}

bool InitApp::InitDirect3D()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		ID3D12Debug* debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&mdxgiFactory)));

// Try to create hardware device.
HRESULT hardwareResult = D3D12CreateDevice(
	nullptr,             // default adapter
	D3D_FEATURE_LEVEL_11_0,
	IID_PPV_ARGS(&md3dDevice));

// Fallback to WARP device.
if (FAILED(hardwareResult))
{
	IDXGIAdapter* pWarpAdapter;
	ThrowIfFailed(mdxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

	ThrowIfFailed(D3D12CreateDevice(
		pWarpAdapter,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice)));
}

ThrowIfFailed(md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE,
	IID_PPV_ARGS(&mFence)));

mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

// Check 4X MSAA quality support for our back buffer format.
// All Direct3D 11 capable devices support 4X MSAA for all render 
// target formats, so we only need to check quality support.

D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
msQualityLevels.Format = mBackBufferFormat;
msQualityLevels.SampleCount = 4;
msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
msQualityLevels.NumQualityLevels = 0;
ThrowIfFailed(md3dDevice->CheckFeatureSupport(
	D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
	&msQualityLevels,
	sizeof(msQualityLevels)));

m4xMsaaQuality = msQualityLevels.NumQualityLevels;
assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

#ifdef _DEBUG
LogAdapters();
#endif

CreateCommandObjects();
CreateSwapChain();
CreateRtvAndDsvDescriptorHeaps();

return true;
}

void InitApp::CreateCommandObjects()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	ThrowIfFailed(md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&mDirectCmdListAlloc)));

	ThrowIfFailed(md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc, // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(&mCommandList)));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	mCommandList->Close();
}

void InitApp::CreateSwapChain()
{
	// Release the previous swapchain we will be recreating.
	if (mSwapChain != NULL) {

		mSwapChain->Release();
	}

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = mBackBufferFormat;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = SwapChainBufferCount;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(mdxgiFactory->CreateSwapChain(
		mCommandQueue,
		&sd,
		&mSwapChain));
}

void InitApp::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	mCurrentFence++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	ThrowIfFailed(mCommandQueue->Signal(mFence, mCurrentFence));

	// Wait until the GPU has completed commands up to this fence point.
	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, L"", false, EVENT_ALL_ACCESS);

		// Fire event when GPU hits current fence.  
		ThrowIfFailed(mFence->SetEventOnCompletion(mCurrentFence, eventHandle));

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

ID3D12Resource* InitApp::CurrentBackBuffer()const
{
	return mSwapChainBuffer[mCurrBackBuffer];
}

D3D12_CPU_DESCRIPTOR_HANDLE InitApp::CurrentBackBufferView()const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
		mCurrBackBuffer,
		mRtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE InitApp::DepthStencilView()const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

void InitApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		wstring fpsStr = to_wstring(fps);
		wstring mspfStr = to_wstring(mspf);

		wstring windowText = mMainWndCaption +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;

		SetWindowText(mhMainWnd, windowText.c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void InitApp::LogAdapters()
{
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (mdxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void InitApp::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, mBackBufferFormat);

		ReleaseCom(output);

		++i;
	}
}

void InitApp::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	// Call with nullptr to get list count.
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}

void InitApp::BuildRootSignature()
{

	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[1];

	// Create a single descriptor table of CBVs.
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &cbvTable);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature)));
}

void InitApp::BuildDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
	cbvHeapDesc.NumDescriptors = 1;
	cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvHeapDesc.NodeMask = 0;
	ThrowIfFailed(md3dDevice->CreateDescriptorHeap(&cbvHeapDesc,
		IID_PPV_ARGS(&mCbvHeap)));
}

void InitApp::BuildConstantBuffers()
{
	mObjectCB = std::make_unique<UploadBuffer<ObjectConstants>>(md3dDevice, 1, true);

	UINT objCBByteSize = Utils::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = mObjectCB->Resource()->GetGPUVirtualAddress();
	// Offset to the ith object constant buffer in the buffer.
	int boxCBufIndex = 0;
	cbAddress += boxCBufIndex * objCBByteSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = Utils::CalcConstantBufferByteSize(sizeof(ObjectConstants));

	md3dDevice->CreateConstantBufferView(
		&cbvDesc,
		mCbvHeap->GetCPUDescriptorHandleForHeapStart());
}

void InitApp::SetCameraMatrices(XMFLOAT4X4 view, XMFLOAT4X4 proj)
{
	XMMATRIX viewMat = XMLoadFloat4x4(&view);
	XMMATRIX projMat = XMLoadFloat4x4(&proj);

	XMMATRIX viewProj = XMMatrixMultiply(viewMat, projMat);
	XMVECTOR viewMatDet = XMMatrixDeterminant(viewMat);
	XMMATRIX invView = XMMatrixInverse(&viewMatDet, viewMat);
	XMVECTOR projMatDet = XMMatrixDeterminant(projMat);
	XMMATRIX invProj = XMMatrixInverse(&projMatDet, projMat);
	XMVECTOR viewProjDet = XMMatrixDeterminant(viewProj);
	XMMATRIX invViewProj = XMMatrixInverse(&viewProjDet, viewProj);

	XMStoreFloat4x4(&mCameraCB.View, XMMatrixTranspose(viewMat));
	XMStoreFloat4x4(&mCameraCB.Proj, XMMatrixTranspose(projMat));
	XMStoreFloat4x4(&mCameraCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mCameraCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mCameraCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mCameraCB.InvViewProj, XMMatrixTranspose(invViewProj));

	mCameraCB.EyePosW = XMFLOAT3(0.0f, 0.0f, -10.0f);

	mCameraCB.NearZ = 1.0f;
	mCameraCB.FarZ = 1000.0f;

	mCameraCB.TotalTime = mTimer.TotalTime();
	mCameraCB.DeltaTime = mTimer.DeltaTime();

	mCameraCB.aspectRatio = AspectRatio();

	mCameraCB.FogStart = 5.0f;
	mCameraCB.FogRange = 150.0f;
	mCameraCB.FogColor = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	mCameraCB.Lights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	mCameraCB.Lights[0].Strength = XMFLOAT3(0.9f, 0.9f, 0.9f);

	mCameraCB.Lights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	mCameraCB.Lights[1].Strength = XMFLOAT3(0.5f, 0.5f, 0.5f);
}