#include "pch.h"    
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Initialize(HWND hwnd, int screenWidth, int screenHeight);
    void Render();

    void SetCameraMatrices(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);

private:
    HWND mHwnd;
    int mScreenWidth;
    int mScreenHeight;

    ID3D12Device* mD3DDevice;
    ID3D12CommandQueue* mCommandQueue;
    ID3D12GraphicsCommandList* mCommandList;
    ID3D12DescriptorHeap* mRtvHeap;
    ID3D12Resource* mRenderTarget;
    UINT mRtvDescriptorSize;

    XMFLOAT4X4 mViewMatrix;
    XMFLOAT4X4 mProjectionMatrix;

    // Autres membres pour la gestion interne du rendu
};

Renderer::Renderer() {
    // Initialiser les membres de la classe
}

Renderer::~Renderer() {
    // Libérer les ressources
}

void Renderer::Initialize(HWND hwnd, int screenWidth, int screenHeight) {
    // Initialiser les périphériques DirectX, les tampons, etc.

    // Exemple de code pour initialiser un périphérique DirectX (à remplir)
    // ...

    mHwnd = hwnd;
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    // Initialiser les matrices de caméra
    // Par exemple, initialiser la vue comme l'identité et la projection avec une projection orthographique par défaut
    mViewMatrix = XMMatrixIdentity();
    mProjectionMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, 0.1f, 1000.0f); // Remplacer par la projection souhaitée
}

void Renderer::Render() {
    // Effectuer le rendu
    // Cela implique de configurer la pipeline graphique, de dessiner des objets, etc.
    // Vous devrez appeler SetCameraMatrices avant de dessiner des objets pour configurer la caméra

    // Exemple de code pour effectuer le rendu (à remplir)
    // ...
}

void Renderer::SetCameraMatrices(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix) {
    // Définir les matrices de vue et de projection pour la caméra
    mViewMatrix = viewMatrix;
    mProjectionMatrix = projectionMatrix;

    // En général, ces matrices seront envoyées aux shaders à l'aide de constant buffers ou de variables de shader
    // Mais pour cet exemple, nous les stockons simplement dans la classe Renderer pour une utilisation ultérieure
}

