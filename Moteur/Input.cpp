#include "Input.h"
#include <iostream>

Input::Input()
{

}

void Input::init()
{
	WidthScreen = GetSystemMetrics(SM_CXSCREEN);
	HeightScreen = GetSystemMetrics(SM_CYSCREEN);
}

void Input::update()
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

bool Input::isKeyPressed(int index)
{
	return keys[index] == KeyState::pressed;
}

bool Input::isKeyReleased(int index)
{
	return keys[index] == KeyState::released;
}

bool Input::isKey(int index)
{
	return keys[index] == KeyState::pressed || keys[index] == KeyState::holding;
}


void Input::GetMouseCords()
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

int Input::GetScreenWidth()
{
	return WidthScreen;
}

int Input::GetScreenHeight()
{
	return HeightScreen;
}