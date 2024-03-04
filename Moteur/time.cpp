#include <windows.h>
#include <iostream>
#include "time.h"

time::time()
{

}

void time::init()
{
	mTotalTime = 0;
	mDeltaTime = 0;
	mTickMinusOne = 0;
}

bool time::update()
{
	float Tick = timeGetTime();
	if (Tick - mTickMinusOne < 8)
	{
		std::cout << "timer for frame under 0.008";
		return false;
	}

	else if (Tick - mTickMinusOne > 40)
	{
		std::cout << "timer for frame over 0.040";
		mDeltaTime = 0.040f;//s
		mTickMinusOne = Tick;//ms
		mTotalTime += mDeltaTime;
	}

	else if (mIsPaused == true)
	{
		mTickMinusOne = Tick;
		mDeltaTime = 0.f;
	}

	else
	{
		mDeltaTime = (Tick - mTickMinusOne) / 1000.0f;//s 
		mTickMinusOne = Tick;//ms
		mTotalTime += mDeltaTime;
	}

	return true;
}

float time::getDeltaTime()
{
	return mDeltaTime;
}

float time::getTotalTime()
{
	return mTotalTime;
}

void time::setPause()
{
	mIsPaused = true;
}

void time::unPause()
{
	mIsPaused = false;
}