#pragma once

#include <Windows.h>

class Input
{

public:
    Input();
    void init();
    void update();
    bool isKey(int index);
    bool isKeyPressed(int index);
    bool isKeyReleased(int index);
    void GetMouseCords();
    int GetScreenWidth();
    int GetScreenHeight();

    enum class KeyState
    {
        none,
        pressed,
        holding,
        released,
    };

private:
    KeyState keys[256];
    POINT mouseCords;
    int WidthScreen;
    int HeightScreen;
};

