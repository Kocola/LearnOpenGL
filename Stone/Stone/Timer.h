#ifndef					__TIMER_H__
#define					__TIMER_H__

#include "Common.h"

class Timer
{
public:
	Timer();

public:
	void start();

	GLfloat calcElapsedTime();

	GLfloat calcInvertal();

	void reset();

private:
	Timer(const Timer&) = delete;
	Timer& operator=(const Timer&) = delete;

private:
	GLfloat		_lastTime;
	GLfloat		_startTime;
};

#endif