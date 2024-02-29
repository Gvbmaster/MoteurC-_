#include <windows.h>
#include "time.h"

time::time()
{

}

void time::reset()
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
	mDeltaTime = 0;
}

void time::start()
{

}