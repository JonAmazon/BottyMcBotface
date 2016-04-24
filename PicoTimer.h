#ifndef _H_PICOTIMER
#define _H_PICOTIMER

#include "PicoGlobal.h"
#include <windows.h>

class PicoTimer
{
public:
	void initialize();
	void release();

	void resetTimer();
	float getDeltaTime();
	float getGlobalTime();

private:
	__int64 ticksPerSecond;
	__int64 timerZero;
	__int64 timerStart;
	__int64 timerEnd;
};

#endif