#include "BrainBox.h"

Synapse * Neuron::addSynapse(float w)
{
	Synapse *s = new Synapse;
	synapses.addNode(w)->data = s;
	return s;
}

void Neuron::initialize(int n)
{
	Synapse *s;

	id = n;
	numBB = 0;
	pos.x = 0.0;
	pos.y = 0.0;
	pos.z = 0.0;
	pos.w = 0.0;

	synapses.initialize(rand());
	s = addSynapse(1.0);
	s->preSynapticNeuron = this;
	s->postSynapticNeuron = NULL;
}

void Population::initialize()
{
	neurons.initialize(rand());
}

Neuron * Population::addNeuron(int id)
{
	Neuron * n = new Neuron;
	;

	neurons.addNode(1.0)->data = n;
	n->initialize(id);
	
	return n;
}

void BeeBoid::initialize(int n)
{
	id = n;
	currentNeuron = NULL;
	visitedNeurons.initialize(3);
}

//This function is clobbering the speed of the update routine.
//I'm de-referencing pointers several GB removed in ram and incurring
//at least 3 or 4 cache misses per update. How to fix?
void BeeBoid::update()
{
	node<Synapse *> *seln = currentNeuron->synapses.getRandomNode();
	Synapse *sel = seln->data;

	currentNeuron->synapses.alterWeight(seln,0.01);
	
	visitedNeurons.writeData(currentNeuron);
	if(sel)
	{
		//Decide whether or not to broadcast this boid
		if(currentNeuron->numBB < 1)
		{
			currentNeuron = NULL;
		}
		else
		{
			type = currentNeuron->type;
			currentNeuron->numBB--;
			currentNeuron = sel->postSynapticNeuron;

			if(currentNeuron)
			{
				if(type == EXC)
				{
					currentNeuron->numBB++;
				}
				else{currentNeuron->numBB = -1;}
			}
		}
	}
}

void BeeBoid::killMe()
{
	visitedNeurons.destroy();
}

void BeeBoid::print()
{
	if(currentNeuron)
	{
		printf("I'm Mr. BeeBoid (%d), look at me! I'm currently at neuron %d\n",id,currentNeuron->id);
	}
	else
	{
		printf("Hi i'm BeeBoid number %d, please kill me. BeeBoids are not born into this world fumbling for meaning Jerry!\n",id);
	}
}

void BrainBox::initializePre()
{
	numberOfPopulations = 7;
	numberOfNeurons = 0;
	beeBoidCounter = 0;

	visualSensitivity = 0.01;
	audioSensitivity = 0.2;
	updateRatio = 1.0;
	
	random = new stdRan(0);

	populations = new Population[numberOfPopulations];
	for(int p = 0; p < numberOfPopulations; ++p)
	{
		populations[p].initialize();
	}

	beeBoids.initialize(random->int64());
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
			n = populations[VISUALR].addNeuron(numberOfNeurons++);
			n->pos.x = random->doub();
			n->pos.y = 1.0;
			n->pos.z = random->doub();
			n->type = EXC;

			n = populations[VISUALG].addNeuron(numberOfNeurons++);
			n->pos.x = random->doub();
			n->pos.y = 0.96;
			n->pos.z = random->doub();
			n->type = EXC;

			n = populations[VISUALB].addNeuron(numberOfNeurons++);
			n->pos.x = random->doub();
			n->pos.y = 0.92;
			n->pos.z = random->doub();
			n->type = EXC;
		}
	}
}

void BrainBox::initializeAudioPopulation(AudioSystem * as)
{
	int nF = as->getNumberOfFreq();
	Neuron * n;

	for(int i = 0; i < 10*nF; ++i)
	{
		n = populations[AUDIOR].addNeuron(numberOfNeurons++);
		n->pos.x = 1.0;
		n->pos.y = 0.5*random->doub() + 0.25;
		n->pos.z = 0.2*random->doub() + 0.4;
		n->type = EXC;
	}

	for(int i = 0; i < 10*nF; ++i)
	{
		n = populations[AUDIOL].addNeuron(numberOfNeurons++);
		n->pos.x = 0.0;
		n->pos.y = 0.5*random->doub() + 0.25;
		n->pos.z = 0.2*random->doub() + 0.4;
		n->type = EXC;
	}
}

void BrainBox::initializeOutputPopulation(int N)
{
	float decayFactor = 5.0;
	Neuron * n;

	for(int i = 0; i < N; ++i)
	{
		n = populations[OUTPUT].addNeuron(numberOfNeurons++);
		n->pos.x = random->doub();
		n->pos.y = 0.0;
		n->pos.z = random->doub();
		n->type = EXC;

		n->addSynapse(decayFactor)->postSynapticNeuron = n;
	}
}

void BrainBox::iniaializeBulkPopulation(Population * p, int N,
								  float x1, float y1, float z1,
								  float x2, float y2, float z2)
{
	Neuron * n;
	for(int i = 0; i < N; ++i)
	{
		n = p->addNeuron(numberOfNeurons++);
		n->pos.x = (x2-x1)*random->doub() + x1;
		n->pos.y = (y2-y1)*random->doub() + y1;
		n->pos.z = (z2-z1)*random->doub() + z1;
		n->type = (random->int32()%3 == 0)?INH:EXC;
	}
}

void BrainBox::initializePost()
{
	connectPopulations(&populations[VISUALR],&populations[6]);
	connectPopulations(&populations[VISUALG],&populations[6]);
	connectPopulations(&populations[VISUALB],&populations[6]);


	connectPopulations(&populations[6],&populations[6]);
	connectPopulations(&populations[6],&populations[OUTPUT]);

	connectPopulations(&populations[AUDIOL],&populations[6]);
	connectPopulations(&populations[AUDIOR],&populations[6]);
}

void BrainBox::connectPopulations(Population * pre, Population * post)
{
	int nPre = pre->neurons.root->size;
	int nPost = post->neurons.root->size;
	int K;
	Neuron * selPre, * selPost;
	Synapse * syn;

	for(int i = 0; i < nPre; ++i)
	{
		selPre = pre->neurons.getRandomNode()->data;
		K = (random->int32())%10+1;
		for(int j = 0; j < K; ++j)
		{
			selPost = post->neurons.getRandomNode()->data;
			syn = selPre->addSynapse(1.0);
			syn->preSynapticNeuron = selPre;
			syn->postSynapticNeuron = selPost;
		}
	}

	printf("Connect Populations :: finished connection population %d,%x to %d,%x.\n",nPre,pre,nPost,post);
}

void BrainBox::spawnBoidsFromVisualInput(VisualSystem *vs)
{
	int nsamp,rx,ry,pidx = 0, nSpawnedR = 0, nSpawnedG = 0, nSpawnedB = 0;
	float posx,posy,p;
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
		p = random->doub();
		if(p < visualSensitivity*img[pidx].x)
		{
			if(nSpawnedR < 1200)
			{
				spawnBeeBoid(n); nSpawnedR++;
			}
		}
	}

	for(int i = 0; i < nsamp; ++i)
	{
		n = populations[VISUALG].neurons.getRandomNode()->data;
		posx = n->pos.x;
		posy = (1.0 - n->pos.z);

		pidx = int(ry*posy)*rx + int(rx*posx);
		p = random->doub();
		if(p < visualSensitivity*img[pidx].y)
		{
			if(nSpawnedG < 1200)
			{
				spawnBeeBoid(n); nSpawnedG++;
			}
		}
	}

	for(int i = 0; i < nsamp; ++i)
	{
		n = populations[VISUALB].neurons.getRandomNode()->data;
		posx = n->pos.x;
		posy = (1.0 - n->pos.z);

		pidx = int(ry*posy)*rx + int(rx*posx);
		p = random->doub();
		if(p < visualSensitivity*img[pidx].z)
		{
			if(nSpawnedB < 1200)
			{
				spawnBeeBoid(n); nSpawnedB++;
			}
		}
	}

	visualSensitivity += 0.0001*(3000 - (nSpawnedR + nSpawnedR + nSpawnedB));
	if(visualSensitivity > 1.2){visualSensitivity = 1.2;}
}

void BrainBox::spawnBoidsFromAudioInput(AudioSystem *as)
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
		{spawnBeeBoid(n);}
	}

	for(int i = 0; i < nF; ++i)
	{
		n = populations[AUDIOR].neurons.getRandomNode()->data;
		fidx = int(nF*(n->pos.y - 0.25)/0.5);

		p = random->doub();
		if(p < audioSensitivity*R[fidx])
		{spawnBeeBoid(n);}
	}
}

void BrainBox::spawnBeeBoid(Neuron * nstart)
{
	BeeBoid * bb;
	
	bb = new BeeBoid;
	bb->initialize(beeBoidCounter++);
	bb->currentNeuron = nstart;
	bb->type = EXC;
	beeBoids.addNode(1.0)->data = bb;
	nstart->numBB++;
}

void BrainBox::updateBeeBoidsRandom()
{
	int i,nsamp;
	node<BeeBoid *> * n;
	BeeBoid * b;

	nsamp = int(updateRatio*beeBoids.size());
	i = 0;
	while(i < nsamp)
	{
		n = beeBoids.getRandomNode();
		if(n)
		{
			b = n->data;
			if(b->currentNeuron == NULL)
			{
				beeBoids.deleteNode(n);
				n->data = NULL;
				killBeeBoid(b);
				delete n;
			}
			else
			{
				b->update();
			}
		}
		i++;
	}
}

void BrainBox::updateBeeBoids()
{
	updateBeeBoidsInOrder(beeBoids.root);
}

void BrainBox::updateBeeBoidsInOrder(node<BeeBoid *> * n)
{
	BeeBoid * b;
	if(n)
	{
		updateBeeBoidsInOrder(n->left);
		updateBeeBoidsInOrder(n->right);
		b = n->data;
		if(b->currentNeuron == NULL)
		{
			beeBoids.deleteNode(n);
			n->data = NULL;
			killBeeBoid(b);
			delete n;
		}
		else
		{
			b->update();
		}
	}
}

void BrainBox::killBeeBoid(BeeBoid * bb)
{
	if(bb)
	{
		bb->killMe();
		delete bb;
		bb = NULL;
	}
	else
	{
		printf("Can't kill dead boid, what are you on about?!\n");
	}
}

void BrainBox::print()
{
}