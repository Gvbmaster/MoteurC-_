#include "DirectXApp.h"

DirectXApp::DirectXApp(HINSTANCE hInstance) : mAppInstance(hInstance)
{
}

DirectXApp::~DirectXApp()
{
    
}

HINSTANCE DirectXApp::AppInst() const
{
    return mAppInstance;
}

float DirectXApp::AspectRatio() const
{
    RECT clientRect;
    GetClientRect(m_hWnd.gethwnd(), &clientRect);

    int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    if (height == 0)
        return 0.0f;

    return static_cast<float>(width) / static_cast<float>(height);
}



HWND DirectXApp::MainWnd() const
{
	return m_hWnd;
}

bool DirectXApp::Init()
{
	if (!InitMainWindow())
	{
		return false;
	}

	return true;
}