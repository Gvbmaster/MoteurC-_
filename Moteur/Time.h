#pragma once
class Time
{
public:
	Time();
	void init();
	bool update();// si update est faux faire recommencer la boucle de jeu, ne pas faire draw etc
	float getDeltaTime();
	float getTotalTime();
	void setPause();
	void unPause();

private:
	float mDeltaTime;//s
	DWORD mTickMinusOne;//ms
	float mTotalTime;//s
	float mWindowsTime;//s
	bool mIsPaused;// flag 
};

// TimeGetTime

