#ifndef _H_MINECRAFTINSTANCE
#define _H_MINECRAFTINSTANCE

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "Windows.h"

#define UP 0
#define DOWN 1

#define LMBUTTON 0
#define RMBUTTON 1

class MineCraftInstance
{
private:
	wchar_t windowName[256];
	HWND hwnd;

	int resX,resY;
	int posX,posY;
	HDC hWindowDC;
	HDC hMemoryDC;
	HBITMAP hBitmap,hBitmapOld;
	BITMAPINFOHEADER bih;

	BYTE *imageData;

public:
	void initialize(const wchar_t* windowNameIn);

	int getResX(){return resX;}
	int getResY(){return resY;}
	BYTE* getImageData(){return imageData;}

	void sendKeyboardInput(int key,int pressed);
	void sendMouseMoveInput(float deltaX, float deltaY);
	void sendMouseWheelInput(int dir);
	void sendMouseButtonInput(int key, int pressed);

	void captureWindow();
};

#endif