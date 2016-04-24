#ifndef _H_VISUALSYSTEM
#define _H_VISUALSYSTEM

#include "PicoVec.h"
#include <Windows.h>

#define HEART_POS_Y 34
#define HEART_POS_X 12
#define HEART_POS_DX 8

#define FOOD_POS_Y 34
#define FOOD_POS_X 114
#define FOOD_POS_DX 8

class VisualSystem
{
private:
	int resX,resY;	
	float health,hunger;
	PicoVec4f *RGBA;

public:
	void initialize(int resXin,int resYin);
	void analyzeCapturedImage(BYTE* imageData);

	int getResX(){return resX;}
	int getResY(){return resY;}
	float getHealth(){return health;}
	float getHunger(){return hunger;}

	PicoVec4f* getVision(){return RGBA;}
};

#endif