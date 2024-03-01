#pragma once
class time
{
public:
	time();
	void init();
	void update();
	float getDeltaTime();
	float getTotalTime();
	void pause();// besoin ? 

private:
	float mDeltaTime;//s
	float mTotalTime;//s
	float mWindowsTime;//s
};

// timeGetTime

