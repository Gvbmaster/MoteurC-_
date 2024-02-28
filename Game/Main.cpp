#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"

#define MAX_LOADSTRING 100
// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Initialisation de la fenêtre
    Window window;
    window.init();

    // Obtenez le handle de la fenêtre
    HWND hwnd = window.gethwnd();

    // Initialisation de DirectX 12
    DX12Initializer dxInitializer(hwnd);
    HRESULT hr = dxInitializer.Initialize();
    if (FAILED(hr)) {
        return -1;
    }

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}