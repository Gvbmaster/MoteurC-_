#include "../Moteur/Windows.h"

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


    Windows windows;
    windows.init();
    
    //Moteur moteur;
    //moteur.Init();
    //moteur.Run();
    //return 0;

}