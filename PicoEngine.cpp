#include "PicoEngine.h"

void PicoEngine::initialize(HWND hwndin,int widthin,int heightin)
{
	//initialize the high performance timer.
	timer.initialize(); 

	input.initialize();
	simulation.initialize();
	renderer.initialize(hwndin, widthin, heightin);
	
	//reset the timer before proceeding to ensure that a proper time delta is
	//passed to the simulation.
	timer.resetTimer();
}

void PicoEngine::runGameLoop()
{
	//First: The input handler is updated by polling the _GLOBAL_inputState
	//for any changes.

	input.update(&_GLOBAL_inputState);

	//Second: The simulation (game logic, physics, world state, etc...) is updated
	//with the elapsed frame time passed as the argument. 

	simulation.update(&input,timer.getDeltaTime());

	//Third: The renderer polls the simulation state and draw the art assets to the window.

	renderer.update(&simulation);
}

void PicoEngine::release()
{
	input.release();
	simulation.release();
	renderer.release();
}