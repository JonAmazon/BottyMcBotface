#ifndef _H_OUTPUTSYSTEM
#define _H_OUTPUTSYSTEM

#include <cstdio>
#include <cstdlib>
#include "BrainBox.h"

#define OUTPUT_GRID 4

#define OUTPUT_DOWN 0
#define OUTPUT_UP 1

struct Latch
{
	int state;
	float activity;
	float decay;
	float sensitivity;
	float threshUp;
	float threshDown;

	void initialize();
	void update(float input);
};

class OutputSystem
{
private:
	Latch ** outState;
public:
	void initialize();
	void updateState(Population * p);

	Latch ** getOutputState(){return outState;}
};

#endif