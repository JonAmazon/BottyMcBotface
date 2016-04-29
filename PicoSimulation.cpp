#include "PicoSimulation.h"

void PicoSimulation::initialize()
{
	_timeAccum = 0.0;
	_mouseSensitivity = 0.5;
	
	
	mcinstance.initialize(__TEXT("Minecraft 1.9.2"));
	visualSystem.initialize(mcinstance.getResX(),mcinstance.getResY());
	audioSystem.initialize();
	brainBox.initializePre();
	brainBox.initializeVisualPopulation(&visualSystem);
	brainBox.initializeAudioPopulation(&audioSystem);
	brainBox.initializeOutputPopulation(800);
	brainBox.iniaializeBulkPopulation(&brainBox.populations[6], 2000000,
									  0.3,0.2,0.3,0.7,0.8,0.7);
	brainBox.initializePost();
	outputSystem.initialize();
	//brainBox.print();

	performanceTimer.initialize();
	selNeuronPot.initialize();
	selNeuronThresh.initialize();
	//TEMPORARY FOR TESTING
	_cam._position.setToZero();

	_cam._distance = 17.0;
	_cam._azimuth = 0.0;
	_cam._elevation = 0.0;

	_cam._FoV = 75.0;
	/////////////////////

	selNeuron = brainBox.populations[6].neurons.getRandomNode()->data;
	brainBox.neuronQueue[brainBox.activeQueue].push(selNeuron);
	audioSystem.startAudioStream();
	performanceTimer.resetTimer();
}

void PicoSimulation::update(PicoInput* xinput,float dtin)
{
	_timeAccum += dtin;
	if(_timeAccum > 0.5){_timeAccum = 0.5;}

	printf("Time since last frame is %f ms\n",performanceTimer.getDeltaTime()*1000);

	mcinstance.captureWindow();
	visualSystem.analyzeCapturedImage(mcinstance.getImageData());
	audioSystem.captureAudioFrames();
	audioSystem.analyzeCapturedAudio();

	if(visualSystem.getHealth() < 0.25)
	{
		printf("You died, attempting respawn\n");
		mcinstance.sendMouseMoveInput(0.0,0.15);
		mcinstance.sendMouseButtonInput(LMBUTTON,1);
		mcinstance.sendMouseButtonInput(LMBUTTON,0);
	}
	else
	{
		performanceTimer.resetTimer();
		brainBox.spawnSpikesFromVisualInput(&visualSystem);
		brainBox.spawnSpikesFromAudioInput(&audioSystem);
		printf("Parsing input streams took %f ms\n",performanceTimer.getDeltaTime()*1000);
		brainBox.update();
		printf("Updateing spikes took %f ms\n",performanceTimer.getDeltaTime()*1000);

		outputSystem.updateState(&brainBox.populations[OUTPUT]);
		dispatchOutputToMinecraft();
		brainBox.neuronQueue[brainBox.activeQueue].push(selNeuron);
		selNeuronPot.write(selNeuron->potential);
		selNeuronThresh.write(selNeuron->threshold);
	}

	if(xinput->getKeyState(VK_RBUTTON))
    {
	    _cam._azimuth += _mouseSensitivity*(-xinput->getDeltaMposX());
	    _cam._elevation += _mouseSensitivity*(-xinput->getDeltaMposY());
	    _cam._distance += -0.5*xinput->getDeltaMwheel();
    }

	if(xinput->getKeyWasPressed(VK_UP))
	{
		selNeuron = brainBox.populations[6].neurons.getRandomNode()->data;
		brainBox.neuronQueue[brainBox.activeQueue].push(selNeuron);
		selNeuronPot.clear();
		selNeuronThresh.clear();
	}
}

void PicoSimulation::release()
{
}

void PicoSimulation::dispatchOutputToMinecraft()
{
	float sensitivity = 0.1,dx = 0,dy = 0;

	//0 0 = 'W'
	//0 1 = 'A'
	//0 2 = 'S'
	//0 3 = 'D'

	//3 0 = 'mouse up'
	//3 1 = 'mouse left'
	//3 2 = 'mouse down'
	//3 3 = 'mouse right'

	//2 0 = 'LMBUTTON'
	//2 1 = 'RMBUTTON'
	//2 2 = 'mouse wheel up'
	//2 3 = 'mouse wheel down'

	//1 0 = 'SPACE'
	//1 1 = 'E'

	Latch ** o = outputSystem.getOutputState();
	
	mcinstance.sendKeyboardInput('W',o[0][0].state);
	mcinstance.sendKeyboardInput('A',o[0][1].state);
	mcinstance.sendKeyboardInput('S',o[0][2].state);
	mcinstance.sendKeyboardInput('D',o[0][3].state);

	dx = sensitivity*(o[3][1].state - o[3][3].state);
	dy = sensitivity*(o[3][0].state - o[3][2].state);
	mcinstance.sendMouseMoveInput(dx,dy);

	mcinstance.sendMouseButtonInput(LMBUTTON,o[2][0].state);
	mcinstance.sendMouseButtonInput(RMBUTTON,o[2][1].state);
	mcinstance.sendMouseWheelInput(o[2][2].state);
	mcinstance.sendMouseWheelInput(-o[2][3].state);

	mcinstance.sendKeyboardInput(VK_SPACE,o[1][0].state);
	//mcinstance.sendKeyboardInput('E',o[1][1].state);
}