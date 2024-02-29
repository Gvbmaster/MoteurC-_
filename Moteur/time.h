#pragma once
class time
{
public:
	time();
	void reset();
	void start();
	void stop();
	double getDeltaTime()const;

private:
	double mDeltaTime;
	double mSecondsPerCount;
};

