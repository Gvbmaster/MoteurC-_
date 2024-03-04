#include <windows.h>
#include <iostream>
#include "Time.h"

Time::Time()
{

}

void Time::init()
{
	mTotalTime = 0;
	mDeltaTime = 0;
	mTickMinusOne = 0;
}

bool Time::update()
{
	float Tick = timeGetTime();
	if (Tick - mTickMinusOne < 8)
	{
		std::cout << "Timer for frame under 0.008";
		return false;
	}

	else if (Tick - mTickMinusOne > 40)
	{
		std::cout << "Timer for frame over 0.040";
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

float Time::getDeltaTime()
{
	return mDeltaTime;
}

float Time::getTotalTime()
{
	return mTotalTime;
}

void Time::setPause()
{
	mIsPaused = true;
}

void Time::unPause()
{
	mIsPaused = false;
}