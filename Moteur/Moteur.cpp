// Moteur.cpp : Définit les fonctions de la bibliothèque statique.
//

#include "pch.h"
#include "framework.h"
#include "InitApp.h"
#include "GameTimer.h"
// TODO: Il s'agit d'un exemple de fonction de bibliothèque
void fnMoteur()
{
}
#include <DirectXColors.h>
#include "Utils.h"

using namespace DirectX;

class InitDirect3DApp : public InitApp
{
public:
    InitDirect3DApp(HINSTANCE hInstance);
    ~InitDirect3DApp();

    bool Initialize()override;

private:
    void OnResize();
    void Update(const GameTimer& gt);
    void Draw(const GameTimer& gt);

};


