#include "PicoTimer.h"

void PicoTimer::initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER*)&timerZero);
	timerStart = 0;
	timerEnd = 0;
}

void PicoTimer::resetTimer()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&timerEnd);
}

float PicoTimer::getDeltaTime()
{
	timerStart = timerEnd;
	QueryPerformanceCounter((LARGE_INTEGER*)&timerEnd);
	return float((timerEnd - timerStart))/ticksPerSecond;
}

float PicoTimer::getGlobalTime()
{
	__int64 tempTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&tempTime);
	return float((tempTime - timerZero))/ticksPerSecond;
}

void release()
{}