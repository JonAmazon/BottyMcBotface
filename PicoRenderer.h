#ifndef _H_PICORENDERER
#define _H_PICORENDERER

#include "PicoGlobal.h"
#include "PicoSimulation.h"
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

class PicoRenderer
{
public:
	void initialize(HWND hwndin, int widthin, int heightin);
	void update(PicoSimulation* xsimulation);
	void release();
private:
	int width,height;
	float aspectRatio;
	HDC _hDC;
	HGLRC _hGLRC;

	void cameraView(PicoCamera* cam);
};

#endif