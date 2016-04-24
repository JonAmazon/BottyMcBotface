#ifndef _H_PICOENGINE
#define _H_PICOENGINE

#include "PicoGlobal.h"
#include "PicoTimer.h"
#include "PicoInput.h"
#include "PicoSimulation.h"
#include "PicoRenderer.h"

class PicoEngine
{
public:
	void initialize(HWND hwndin, int widthin, int heightin);
	void runGameLoop();
	void release();
private:
	PicoTimer timer;
	PicoInput input;
	PicoSimulation simulation;
	PicoRenderer renderer;
};

#endif