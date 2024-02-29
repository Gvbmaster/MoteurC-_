#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"
#include "../Moteur/Camera.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Initialisation de la fenêtre
    Window window;
    window.init();
    HWND hwnd = window.gethwnd();

    // Initialisation de DirectX 12
    DX12Initializer dxInitializer(hwnd);
    HRESULT hr = dxInitializer.Initialize();
    if (FAILED(hr)) {
        return -1;
    }

    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -5.0f);
    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&cameraPosition),
        DirectX::XMLoadFloat3(&cameraTarget),
        DirectX::XMLoadFloat3(&cameraUp)
    );

    viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            dxInitializer.Draw();
        }
    }

    return 0;
}
