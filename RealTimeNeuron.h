#ifndef _H_REALTIMENEURON
#define _H_REALTIMENEURON

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
#include <list>
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

#define EXC 1
#define INH -1

struct Neuron;
struct Synapse;
struct BrainBox;
struct Population;

struct Synapse
{
	float weight;
	Neuron * preSynapticNeuron;
	Neuron * postSynapticNeuron;
};

struct Neuron
{
	int type;
	float timeConstant;
	float threshTimeConst;
	float threshDelta;
	float threshold;
	float thresholdMin;
	float potential;
	float input;

	unsigned long long int lastSpikeArrivalTime;
	PicoVec4f pos;
	std::list<Synapse *> afferent;
	std::list<Synapse *> efferent;
};

struct Population
{
	rabit<Neuron *> neurons;
};

struct BrainBox
{
	int numberOfPopulations;
	int activeQueue;
	unsigned long long int simulationTime;

	float visualSensitivityR;
	float visualSensitivityG;
	float visualSensitivityB;
	float audioSensitivity;
	float failureRate;

	std::queue<Neuron *> renderQueue;
	std::queue<Neuron *> neuronQueue[2];
	Population * populations;
	stdRan * random;

	void addSynapse(Neuron * pre, Neuron * post, float w);
	Neuron * addNeuron(Population *p,int t);

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
	void spawnSpikesFromVisualInput(VisualSystem *vs);
	void spawnSpikesFromAudioInput(AudioSystem *as);

	float STDPfunction(long long int dt);
	void update();

	void ouputToFile(const char * brainFile);
	void print();
};

#endif