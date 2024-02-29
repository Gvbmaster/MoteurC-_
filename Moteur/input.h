#pragma once

#include <Windows.h>

class input
{
public:
    input();
    void init();
    void update();
    bool isKey(int index);
    bool isKeyPressed(int index);
    bool isKeyReleased(int index);
    LPPOINT GetMouseCords(LPPOINT mouseCords);

    enum class KeyState
    {
        none,
        pressed,
        holding,
        released,
    };
private:
    KeyState keys[256];
    LPPOINT mouseCords;
};

