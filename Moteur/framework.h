// header.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets
//

#pragma once
#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows
#include <windows.h>
#include <wrl.h>
#include <shellapi.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include "DDSTextureLoader.h"
#include "MathHelper.h"
#include <stdio.h>
#include <string>