#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"
#include "../Moteur/Camera.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Window window;
    window.init();
    HWND hwnd = window.gethwnd();

    DX12Initializer dxInitializer(hwnd);
    HRESULT hr = dxInitializer.Initialize();
    if (FAILED(hr)) {
        return -1;
    }

    Camera camera;
    camera.setViewMatrix();
    camera.setProjectionMatrix();

    MSG msg;
    while (true)
    {
        camera.update(/* delta time */);

        // Gestion des messages de la fenêtre
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Mise à jour de la logique du jeu
            // Rendu de la scène
        }
    }

    return (int)msg.wParam;
}
