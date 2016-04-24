#ifndef _H_PICOINPUT
#define _H_PICOINPUT

#include "PicoGlobal.h"
#include <windows.h>

//PicoInputState: This structure serves as an interface between the PicoInput structure
//and the operating system. A global instance of PicoInputState is altered by the operating
//systems message pump.
struct PicoInputState
{
	float _mouseWheelDelta;
	float _mousePosXDelta;
	float _mousePosYDelta;

	char _keyboardStateHasChanged;
	char _keyboardState[PICO_NKEYS];

	void initialize()
	{
		_mouseWheelDelta = 0;
		_mousePosXDelta = 0;
		_mousePosYDelta = 0;
		_keyboardStateHasChanged = 0;
		for(int i = 0; i < PICO_NKEYS; ++i)
		{
			_keyboardState[i] = 0;
		}
	}

	void reset()
	{
		_mouseWheelDelta = 0;
		_mousePosXDelta = 0;
		_mousePosYDelta = 0;
		_keyboardStateHasChanged = 0;
	}
};

class PicoInput
{
public:
	void initialize();
	void update(PicoInputState* xinputState);
	void release();

	float getDeltaMposX(){return _dmposX;}
	float getDeltaMposY(){return _dmposY;}
	float getDeltaMwheel(){return _dmWheel;}

	int getKeyState(char keycode){return _keyState[keycode];}
	int getKeyWasPressed(char keycode){return ((1 == _keyState[keycode])&&(0 == _keyStatePrev[keycode]))?1:0;}
	int getKeyWasReleased(char keycode){return ((0 == _keyState[keycode])&&(1 == _keyStatePrev[keycode]))?1:0;}
private:
	float _dmposX,_dmposY;
	float _dmWheel;

	char _keyState[PICO_NKEYS];
	char _keyStatePrev[PICO_NKEYS];
};

//global instance of PicoRawInputState meant to interface with the operating systems
//message pump.
extern PicoInputState _GLOBAL_inputState;

#endif