#include "RealTimeNeuron.h"

void BrainBox::initializePre()
{
	numberOfPopulations = 6+5;
	activeQueue = 0;
	simulationTime = 0;

	visualSensitivityR = 8.0;
	visualSensitivityG = 10.0;
	visualSensitivityB = 8.0;
	audioSensitivity = 0.1;
	failureRate = 0.8;

	random = new stdRan(0);
	populations = new Population[numberOfPopulations];
	for(int i = 0; i < numberOfPopulations; ++i)
	{
		populations[i].neurons.initialize(random->int64());
	}
}

Neuron * BrainBox::addNeuron(Population * p, int t)
{
	Neuron * n = new Neuron;
	n->type = t;
	n->timeConstant = 20.0*random->doub() + 10.0;
	n->threshold = 1.0;
	n->thresholdMin = 0.1;
	n->threshTimeConst = 300.0;
	n->threshDelta = 0.1;
	n->potential = 0.0;
	n->input = 0.0;
	n->lastSpikeArrivalTime = 0;
	n->lastSpikeTime = 0;
	n->movable = false;

	p->neurons.addNode(1.0)->data = n;

	return n;
}

void BrainBox::initializeVisualPopulation(VisualSystem * vs)
{
	int rx = vs->getResX();
	int ry = vs->getResY();
	float xpos,ypos;
	Neuron * n;

	//Store neurons in RGB triplets (bitmap order)
	for(int y = 0; y < ry; ++y)
	{
		for(int x = 0; x < rx; ++x)
		{
			//xpos = (rx - x - 0.5)/rx;
			//ypos = (y+0.5)/ry;
			n = addNeuron(&populations[VISUALR],EXC);
			n->pos.x = random->doub();
			n->pos.y = 1.0;
			n->pos.z = random->doub();
			n->threshold = 2.0;
			n->thresholdMin = 2.0;
			n->threshDelta = 1.0;
			n->timeConstant = 0.1;
			n->movable = false;

			n = addNeuron(&populations[VISUALG],EXC);
			n->pos.x = random->doub();
			n->pos.y = 0.96;
			n->pos.z = random->doub();
			n->threshold = 2.0;
			n->thresholdMin = 2.0;
			n->threshDelta = 1.0;
			n->timeConstant = 0.1;
			n->movable = false;

			n = addNeuron(&populations[VISUALB],EXC);
			n->pos.x = random->doub();
			n->pos.y = 0.92;
			n->pos.z = random->doub();
			n->threshold = 2.0;
			n->thresholdMin = 2.0;
			n->threshDelta = 1.0;
			n->timeConstant = 0.1;
			n->movable = false;
		}
	}
}

void BrainBox::initializeAudioPopulation(AudioSystem * as)
{
	int nF = as->getNumberOfFreq();
	Neuron * n;

	for(int i = 0; i < 10*nF; ++i)
	{
		n = addNeuron(&populations[AUDIOR],EXC);
		n->pos.x = 1.0;
		n->pos.y = 0.5*random->doub() + 0.25;
		n->pos.z = 0.2*random->doub() + 0.4;
		n->timeConstant = 0.1;
		n->movable = false;
	}

	for(int i = 0; i < 10*nF; ++i)
	{
		n = addNeuron(&populations[AUDIOL],EXC);
		n->pos.x = 0.0;
		n->pos.y = 0.5*random->doub() + 0.25;
		n->pos.z = 0.2*random->doub() + 0.4;
		n->timeConstant = 0.1;
		n->movable = false;
	}
}

void BrainBox::initializeOutputPopulation(int N)
{
	Neuron * n;

	for(int i = 0; i < N; ++i)
	{
		n = addNeuron(&populations[OUTPUT],EXC);
		n->pos.x = random->doub();
		n->pos.y = 0.0;
		n->pos.z = random->doub();
		n->threshold = 10.0;
		n->thresholdMin = 10.0;
		n->threshTimeConst = 0.1;
		n->movable = false;
	}
}

void BrainBox::iniaializeBulkPopulation(Population * p, int N,
								  float x1, float y1, float z1,
								  float x2, float y2, float z2)
{
	Neuron * n;
	for(int i = 0; i < N; ++i)
	{
		n = addNeuron(p,2*(random->int32()%2) - 1);
		n->pos.x = (x2-x1)*random->doub() + x1;
		n->pos.y = (y2-y1)*random->doub() + y1;
		n->pos.z = (z2-z1)*random->doub() + z1;
	}
}

void BrainBox::addSynapse(Neuron * pre, Neuron * post, float w)
{
	Synapse * sn = new Synapse;
	sn->weight = w;
	sn->preSynapticNeuron = pre;
	sn->postSynapticNeuron = post;

	pre->efferent.push_back(sn);
	post->afferent.push_back(sn);
}

void BrainBox::removeSynapse(Synapse * s)
{
	s->preSynapticNeuron->efferent.remove(s);
	s->postSynapticNeuron->afferent.remove(s);
	delete s;
}

void BrainBox::connectPopulations(Population * pre, Population * post)
{
	int nPre = pre->neurons.root->size;
	int nPost = post->neurons.root->size;
	int K;
	Neuron * selPre, * selPost;
	Synapse * syn;

	for(int i = 0; i < 2*nPre; ++i)
	{
		selPre = pre->neurons.getRandomNode()->data;
		K = (random->int32())%20+1;
		for(int j = 0; j < K; ++j)
		{
			selPost = post->neurons.getRandomNode()->data;
			addSynapse(selPre,selPost,0.5*random->doub());
		}
	}

	printf("Connect Populations :: finished connection population %d,%x to %d,%x.\n",nPre,pre,nPost,post);
}

void BrainBox::initializePost()
{
	connectPopulations(&populations[VISUALR],&populations[6]);
	connectPopulations(&populations[VISUALG],&populations[6]);
	connectPopulations(&populations[VISUALB],&populations[6]);

	connectPopulations(&populations[6],&populations[7]);
	connectPopulations(&populations[7],&populations[8]);
	connectPopulations(&populations[8],&populations[9]);
	connectPopulations(&populations[9],&populations[10]);

	connectPopulations(&populations[8],&populations[OUTPUT]);

	connectPopulations(&populations[AUDIOL],&populations[8]);
	connectPopulations(&populations[AUDIOR],&populations[8]);
}

void BrainBox::spawnSpikesFromVisualInput(VisualSystem *vs)
{
	int nsamp,rx,ry,pidx = 0;
	float posx,posy;
	PicoVec4f * img;
	Neuron * n;

	rx = vs->getResX();
	ry = vs->getResY();
	img = vs->getVision();
	nsamp = rx*ry/10;

	for(int i = 0; i < nsamp; ++i)
	{
		n = populations[VISUALR].neurons.getRandomNode()->data;
		posx = n->pos.x;
		posy = (1.0 - n->pos.z);

		pidx = int(ry*posy)*rx + int(rx*posx);
		n->input += visualSensitivityR*img[pidx].x;
		neuronQueue[activeQueue].push(n);
	}

	for(int i = 0; i < nsamp; ++i)
	{
		n = populations[VISUALG].neurons.getRandomNode()->data;
		posx = n->pos.x;
		posy = (1.0 - n->pos.z);

		pidx = int(ry*posy)*rx + int(rx*posx);
		n->input += visualSensitivityG*img[pidx].y;
		neuronQueue[activeQueue].push(n);
	}

	for(int i = 0; i < nsamp; ++i)
	{
		n = populations[VISUALB].neurons.getRandomNode()->data;
		posx = n->pos.x;
		posy = (1.0 - n->pos.z);

		pidx = int(ry*posy)*rx + int(rx*posx);
		n->input += visualSensitivityB*img[pidx].z;
		neuronQueue[activeQueue].push(n);
	}
	//visualSensitivity += 0.00001*(1500 - nSpike);
	//if(visualSensitivity > 30.0){visualSensitivity = 30.0;}
}

void BrainBox::spawnSpikesFromAudioInput(AudioSystem *as)
{
	int nF = as->getNumberOfFreq();
	int fidx = 0;
	float p;
	float * L = as->getLeftSpectrum();
	float * R = as->getRightSpectrum();
	Neuron * n;

	for(int i = 0; i < nF; ++i)
	{
		n = populations[AUDIOL].neurons.getRandomNode()->data;
		fidx = int(nF*(n->pos.y - 0.25)/0.5);

		p = random->doub();
		if(p < audioSensitivity*L[fidx])
		{
			n->input += 1.0; 
			neuronQueue[activeQueue].push(n);
		}
	}

	for(int i = 0; i < nF; ++i)
	{
		n = populations[AUDIOR].neurons.getRandomNode()->data;
		fidx = int(nF*(n->pos.y - 0.25)/0.5);

		p = random->doub();
		if(p < audioSensitivity*R[fidx])
		{
			n->input += 1.0; 
			neuronQueue[activeQueue].push(n);
		}
	}
}

float STDP(float dt)
{
	float timeScale = 100.0;
	return 0.1*expf(-dt*dt/(2*timeScale*timeScale));
}

void BrainBox::update()
{
	int nSpike = 0;
	int synDeleted = 0;
	float learningRate = 0.001;
	float posDelta = 0.01;
	Neuron * sel,* selPost,* selPre;
	Synapse * selSyn;
	std::list<Synapse *>::iterator iterSyn;
	PicoVec4f dP;
	

	printf("Handling %d potential spikes this frame\n",neuronQueue[activeQueue].size());
	while(!neuronQueue[activeQueue].empty())
	{
		sel = neuronQueue[activeQueue].front();
		sel->potential *= expf(-float(simulationTime - sel->lastSpikeArrivalTime)/(sel->timeConstant));
		sel->potential += sel->input;
		sel->input = 0.0;
		sel->threshold = sel->thresholdMin + (sel->threshold - sel->thresholdMin)*expf(-float(simulationTime - sel->lastSpikeArrivalTime)/(sel->threshTimeConst));
		sel->lastSpikeArrivalTime = simulationTime;

		//Check if this neuron is spiking right now.
		if(sel->potential > sel->threshold)
		{
			nSpike++;
			sel->potential = 0.0;
			sel->lastSpikeTime = simulationTime;

			//Iterate through the synapses and transmit
			for(iterSyn = sel->efferent.begin(); iterSyn != sel->efferent.end(); ++iterSyn)
			{
				if(random->doub() < 1.0-failureRate)
				{
					selPost = (*iterSyn)->postSynapticNeuron;
					selPost->input += (sel->type)*((*iterSyn)->weight);
					neuronQueue[(activeQueue+1)%2].push(selPost);
				}
			}
			for(iterSyn = sel->afferent.begin(); iterSyn != sel->afferent.end(); ++iterSyn)
			{
				selPre = (*iterSyn)->preSynapticNeuron;
				(*iterSyn)->weight += learningRate*STDP(float(simulationTime-selPre->lastSpikeTime)) - learningRate*(*iterSyn)->weight;
				if((*iterSyn)->weight < 0.0){(*iterSyn)->weight = 0.0;}
				if((*iterSyn)->weight > 1.0){(*iterSyn)->weight = 1.0;}
			}

			sel->threshold += sel->threshDelta;
			renderQueue.push(sel);
		}
		neuronQueue[activeQueue].pop();
	}
	
	simulationTime++;
	activeQueue = (activeQueue+1)%2;
	printf("deleted %d synapses\n",synDeleted);
	printf("Handled %d spikes this frame\n",nSpike);
}