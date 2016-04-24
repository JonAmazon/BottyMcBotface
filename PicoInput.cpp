#include "PicoInput.h"

void PicoInput::initialize()
{
	//ShowCursor(false);

	_dmposX = 0.0;
	_dmposY  = 0.0;
	_dmWheel = 0.0;
	for(int i = 0; i < PICO_NKEYS; ++i)
	{
		_keyState[i] = 0;
		_keyStatePrev[i] = 0;
	}
}

void PicoInput::update(PicoInputState* xinputState)
{
	
	_dmposX = xinputState->_mousePosXDelta;
	_dmposY = xinputState->_mousePosYDelta;
	_dmWheel = xinputState->_mouseWheelDelta;

	if(xinputState->_keyboardStateHasChanged)
	{
		for(int i = 0; i < PICO_NKEYS; ++i)
		{
			_keyStatePrev[i] = _keyState[i];
			_keyState[i] = xinputState->_keyboardState[i];
		}
	}
	else
	{
		for(int i = 0; i < PICO_NKEYS; ++i)
		{
			_keyStatePrev[i] = _keyState[i];
		}
	}


	xinputState->reset();
}

void PicoInput::release()
{
}