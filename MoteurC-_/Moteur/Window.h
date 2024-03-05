#pragma once
#include <windows.h>

class Window
{
public:
    Window();
    bool init();
    ATOM MyRegisterClass(HINSTANCE hInstance);
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
    static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    HWND gethwnd();

private:
    HWND m_hWnd;
};