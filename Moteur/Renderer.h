//#include <d3d12.h>
//#include <DirectXMath.h>
//
//using namespace DirectX;
//
//class Renderer {
//public:
//    Renderer();
//    ~Renderer();
//
//    void Initialize(HWND hwnd, int screenWidth, int screenHeight);
//    void Render();
//
//    void SetCameraMatrices(const XMFLOAT4X4& viewMatrix, const XMFLOAT4X4& projectionMatrix);
//
//private:
//    HWND mHwnd;
//    int mScreenWidth;
//    int mScreenHeight;
//
//    ID3D12Device* mD3DDevice;
//    ID3D12CommandQueue* mCommandQueue;
//    ID3D12GraphicsCommandList* mCommandList;
//    ID3D12DescriptorHeap* mRtvHeap;
//    ID3D12Resource* mRenderTarget;
//    UINT mRtvDescriptorSize;
//
//    XMFLOAT4X4 mViewMatrix;
//    XMFLOAT4X4 mProjectionMatrix;
//
//};