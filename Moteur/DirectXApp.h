#include "framework.h"
#include "Window.h" // Incluez la classe Window ici
#include "Time.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class DirectXApp
{
public:
    DirectXApp(HINSTANCE hInstance);
    DirectXApp(const DirectXApp& rhs) = delete;
    DirectXApp& operator=(const DirectXApp& rhs) = delete;
    ~DirectXApp();

    HINSTANCE AppInst()const;
    float     AspectRatio()const;

    int Run();

    virtual bool Initialize();
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    bool InitDirect3D();
    void CreateRtvAndDsvDescriptorHeaps();
    void OnResize();
   	void Update(const Time& gt) = 0;
   	void Draw(const Time& gt) = 0;

private:
    HINSTANCE mAppInstance;
    Window mMainWindow; // Utilisez la classe Window pour gérer la fenêtre principale
};
