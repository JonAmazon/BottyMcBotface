#ifndef _H_PICOSIMULATION
#define _H_PICOSIMULATION

#include "PicoGlobal.h"
#include "PicoInput.h"
#include "PicoTimer.h"
#include "MineCraftInstance.h"
#include "VisualSystem.h"
#include "AudioSystem.h"
#include "RealTimeNeuron.h"
#include "OutputSystem.h"

#define CIRCREGLEN 100

struct CircularRegister
{
	int cursor;
	float * data;

	void initialize()
	{
		cursor = 0;
		data = new float[CIRCREGLEN];
		for(int i = 0; i < CIRCREGLEN; ++i)
		{
			data[i] = 0.0;
		}
	}
	void write(float d)
	{
		data[cursor] = d;
		cursor = (cursor+1)%CIRCREGLEN;
	}
	float read(int idx)
	{
		return data[(cursor+idx)%CIRCREGLEN];
	}
	void clear()
	{
		for(int i = 0; i < CIRCREGLEN; ++i)
		{
			data[i] = 0.0;
		}
	}
};

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

	Neuron * getSelNeuron(){return selNeuron;}
	CircularRegister * getSelNeuronPot(){return &selNeuronPot;}
	CircularRegister * getSelNeuronThresh(){return &selNeuronThresh;}

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

	Neuron * selNeuron;
	CircularRegister selNeuronPot;
	CircularRegister selNeuronThresh;

	PicoCamera _cam;

	void dispatchOutputToMinecraft();
};

#endif