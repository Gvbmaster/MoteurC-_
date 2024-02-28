#pragma once
#include "GameObject.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void initialize(HWND hwnd);
    void render(const std::vector<GameObject>& gameObjects);

private:
};