#ifndef _H_PICOSIMULATION
#define _H_PICOSIMULATION

#include "PicoGlobal.h"
#include "PicoInput.h"
#include "PicoTimer.h"
#include "MineCraftInstance.h"
#include "VisualSystem.h"
#include "AudioSystem.h"
#include "BrainBox.h"
#include "OutputSystem.h"

struct PicoCamera
{
	float _FoV,_distance;
	float _azimuth,_elevation;
	PicoVec4f _position;
};
class PicoSimulation
{
public:

	void initialize();
	void update(PicoInput* xinput,float dt);
	void release();

	PicoCamera* getxCamera(){return &_cam;}
	MineCraftInstance* getMineCraftInstance(){return &mcinstance;}
	VisualSystem* getVisualSystem(){return &visualSystem;}
	AudioSystem * getAudioSystem(){return &audioSystem;}
	OutputSystem * getOutputSystem(){return &outputSystem;}
	BrainBox* getBrainBox(){return &brainBox;}

private:
	float _timeAccum;
	float _mouseSensitivity;
	int moving;

	PicoTimer performanceTimer;
	MineCraftInstance mcinstance;
	VisualSystem visualSystem;
	AudioSystem audioSystem;
	OutputSystem outputSystem;
	BrainBox brainBox;

	PicoCamera _cam;

	void dispatchOutputToMinecraft();
};

#endif