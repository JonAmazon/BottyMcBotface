#include "OutputSystem.h"

void Latch::initialize()
{
	decay = 0.985;
	sensitivity = 0.018;
	threshUp = 0.75;
	threshDown = 0.25;

	state = OUTPUT_DOWN;
	activity = 0.0;
}

void Latch::update(float input)
{
	activity *= decay;
	activity += sensitivity*input;

	switch(state)
	{
		case OUTPUT_DOWN:
			{
				if(activity > threshUp)
				{state = OUTPUT_UP;}
				break;
			}
		case OUTPUT_UP:
			{
				if(activity < threshDown)
				{state = OUTPUT_DOWN;}
				break;
			}
	}
}

void OutputSystem::initialize()
{
	outState = new Latch *[OUTPUT_GRID];
	for(int i = 0; i < OUTPUT_GRID; ++i)
	{
		outState[i] = new Latch[OUTPUT_GRID];
	}

	for(int i = 0; i < OUTPUT_GRID; ++i)
	{
		for(int j = 0; j < OUTPUT_GRID; ++j)
		{
			outState[i][j].initialize();
		}
	}
}

void OutputSystem::updateState(Population * p)
{
	int nsamp = (p->neurons).size(p->neurons.root);
	int x,y;
	Neuron * n;
	
	for(int i = 0; i < nsamp; ++i)
	{
		n = p->neurons.getRandomNode()->data;
		x = int((n->pos.x)*OUTPUT_GRID);
		y = int((n->pos.z)*OUTPUT_GRID);

		outState[x][y].update(1.0*(n->numBB));
	}
}