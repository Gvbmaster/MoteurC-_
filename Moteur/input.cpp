#include "input.h"
#include <iostream>

input::input()
{

}

void input::init()
{
	WidthScreen = GetSystemMetrics(SM_CXSCREEN);
	HeightScreen = GetSystemMetrics(SM_CYSCREEN);
}

void input::update()
{
	for (int i = 0; i < 256; ++i) 
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			if (keys[i] == KeyState::pressed)
			{
				keys[i] = KeyState::holding;
			}
			else if (keys[i] == KeyState::none || keys[i] == KeyState::released)
			{
				keys[i] = KeyState::pressed;
			}
		}

		else
		{
			if (keys[i] == KeyState::pressed || keys[i] == KeyState::holding)
			{
				keys[i] = KeyState::released;
			}

			else if (keys[i] == KeyState::released)
			{
				keys[i] = KeyState::none;
			}
		}
	}
}

bool input::isKeyPressed(int index)
{
	return keys[index] == KeyState::pressed;
}

bool input::isKeyReleased(int index)
{
	return keys[index] == KeyState::released;
}

bool input::isKey(int index)
{
	return keys[index] == KeyState::pressed || keys[index] == KeyState::holding;
}


void input::GetMouseCords()
{
	if (GetCursorPos(&mouseCords)== 0)
	{
		std::cout << "error with GetMouseCords";
	}
	else
	{
		std::cout << "mouse cords updated";
	}
}

int input::GetScreenWidth()
{
	return WidthScreen;
}

int input::GetScreenHeight()
{
	return HeightScreen;
}