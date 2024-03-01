#include <windows.h>
#include "time.h"

time::time()
{

}

void time::init()
{
	mWindowsTime = timeGetTime()/1000.0f;//s
	mTotalTime = 0;
	mDeltaTime = 0;
}

void time::update()
{
	float Tick = timeGetTime() / 1000.0f;//s
	mDeltaTime = Tick - mWindowsTime - mTotalTime;// a verif ??? 
	mTotalTime += mDeltaTime;
}

float time::getDeltaTime()
{
	return mDeltaTime;
}

float time::getTotalTime()
{
	return mTotalTime;
}