#include "../Moteur/Window.h"

#define MAX_LOADSTRING 100
// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{


    Window window;
    window.init();
    
    //Moteur moteur;
    //moteur.Init();
    //moteur.Run();

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
       TranslateMessage(&msg);
       DispatchMessage(&msg);
    }

    return (int)msg.wParam;

    // return 0;
}