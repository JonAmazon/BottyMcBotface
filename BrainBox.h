#ifndef _H_BRAINBOX
#define _H_BRAINBOX

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "StatRand.h"
#include "PicoVec.h"
#include "rabit.h"
#include "VisualSystem.h"
#include "AudioSystem.h"

#define	VISUALR 0
#define VISUALG 1
#define VISUALB 2
#define AUDIOR 3
#define AUDIOL 4
#define OUTPUT 5

#define EXC 0
#define INH 1

struct Neuron;
struct Synapse;
struct BeeBoid;
struct BrainBox;

struct CircularRegister
{
	Neuron * * data;
	int registerLen;
	int cursor;

	void initialize(int size)
	{
		registerLen = size;
		data = new Neuron *[registerLen];
		for(int i = 0; i < registerLen; ++i)
		{
			data[i] = NULL;
		}
		cursor = 0;
	}

	void destroy()
	{
		registerLen = 0;
		cursor = 0;
		delete [] data;
		data = NULL;
	}

	void writeData(Neuron * in)
	{
		data[cursor] = in;
		cursor = (cursor+1)%registerLen;
	}

	Neuron * readData(int k)
	{
		return data[(cursor+k)%registerLen];
	}
};

struct Synapse
{
	Neuron * preSynapticNeuron;
	Neuron * postSynapticNeuron;
};

struct Neuron
{
	int id;
	int type;
	int numBB;
	PicoVec4f pos;
	rabit<Synapse * > synapses;

	void initialize(int n);
	Synapse * addSynapse(float w);
};

struct Population
{
	rabit<Neuron *> neurons;

	void initialize();
	Neuron * addNeuron(int id);
};

struct BeeBoid
{
	int id;
	int type;
	Neuron * currentNeuron;
	CircularRegister visitedNeurons;

	void initialize(int n);
	void update();
	void killMe();
	void print();
};

struct BrainBox
{
	int numberOfPopulations;
	int numberOfNeurons;
	int beeBoidCounter;

	float visualSensitivity;
	float audioSensitivity;
	float updateRatio;

	Population * populations;
	rabit<BeeBoid *> beeBoids;
	stdRan * random;
	
	void spawnBeeBoid(Neuron * nstart);
	void killBeeBoid(BeeBoid * boid);

	void initializePre();
	void initializeVisualPopulation(VisualSystem * vs);
	void initializeAudioPopulation(AudioSystem * as);
	void initializeOutputPopulation(int N);
	void iniaializeBulkPopulation(Population * p, int N,
								  float x1, float y1, float z1,
								  float x2, float y2, float z2);
	void initializePost();
	void buildFromFile(const char * brainFile);

	void connectPopulations(Population * pre, Population * post);
	void spawnBoidsFromVisualInput(VisualSystem *vs);
	void spawnBoidsFromAudioInput(AudioSystem *as);
	void updateBeeBoids();
	void updateBeeBoidsRandom();
	void updateBeeBoidsInOrder(node<BeeBoid *> * n);

	void ouputToFile(const char * brainFile);
	void print();
};

#endif