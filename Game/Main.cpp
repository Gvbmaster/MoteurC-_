#include "../Moteur/Window.h"
#include "../Moteur/DX12Initializer.h"
#include "../Moteur/Camera.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // Initialisation de la fen�tre
    Window window;
    window.init();
    HWND hwnd = window.gethwnd();

    // Initialisation de DirectX 12
    DX12Initializer dxInitializer(hwnd);
    HRESULT hr = dxInitializer.Initialize();
    if (FAILED(hr)) {
        return -1;
    }

    Camera camera;

    DirectX::XMFLOAT3 cameraPosition(0.0f, 0.0f, -5.0f);
    DirectX::XMFLOAT3 cameraTarget(0.0f, 0.0f, 0.0f);
    DirectX::XMFLOAT3 cameraUp(0.0f, 1.0f, 0.0f);

    DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&cameraPosition),
        DirectX::XMLoadFloat3(&cameraTarget),
        DirectX::XMLoadFloat3(&cameraUp)
    );

    viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

    camera.setViewMatrix(viewMatrix);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
