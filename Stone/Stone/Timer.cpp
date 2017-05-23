#include "Timer.h"

Timer::Timer()
{
	start();
}

void Timer::start()
{
	_startTime = glfwGetTime();
	_lastTime = _startTime;
}

GLfloat Timer::calcElapsedTime()
{
	GLfloat currentTime = glfwGetTime();
	auto elapsedTime = currentTime - _startTime;
	return elapsedTime;
}

GLfloat Timer::calcInvertal()
{
	GLfloat currentTime = glfwGetTime();
	auto interval = currentTime - _lastTime;
	_lastTime = currentTime;
	return interval;
}

void Timer::reset()
{
	_lastTime = 0;
	_startTime = 0;
}