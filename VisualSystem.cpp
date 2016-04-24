#include "VisualSystem.h"

void VisualSystem::initialize(int resXin,int resYin)
{
	resX = resXin;
	resY = resYin;
	health = 10.0;
	hunger = 10.0;
	RGBA = new PicoVec4f[resX*resY];
}

void VisualSystem::analyzeCapturedImage(BYTE* imageData)
{
	int pidx;
	float inH;
	ZeroMemory(RGBA,sizeof(PicoVec4f)*resX*resY);

	for(int xidx = 0; xidx < resX; xidx++)
	{
		for(int yidx = 0; yidx < resY; yidx++)
		{
			RGBA[yidx*resX+xidx].z += 1.0*imageData[yidx*(4*resX)+4*xidx];
			RGBA[yidx*resX+xidx].y += 1.0*imageData[yidx*(4*resX)+4*xidx+1];
			RGBA[yidx*resX+xidx].x += 1.0*imageData[yidx*(4*resX)+4*xidx+2];
			RGBA[yidx*resX+xidx].w += 255.0;
		}
	}

	for(int px = 0; px < resX; px++)
	{
		for(int py = 0; py < resY; py++)
		{
			RGBA[py*resX+px] /= RGBA[py*resX+px].w;
		}
	}

	for(int p = 0; p < resX*resY; ++p)
	{
		RGBA[p].w = (RGBA[p].x + RGBA[p].y + RGBA[p].z)/3.0;
	}

	inH = 0.0;
	for(int h = 0; h < 10; ++h)
	{
		pidx = HEART_POS_Y*resX + HEART_POS_X + HEART_POS_DX*h;
		if((RGBA[pidx].x < 0.2) && (RGBA[pidx].y < 0.2) && (RGBA[pidx].z < 0.2))
		{}
		else
		{inH += 0.5;}
		pidx += 2;
		if((RGBA[pidx].x < 0.2) && (RGBA[pidx].y < 0.2) && (RGBA[pidx].z < 0.2))
		{}
		else
		{inH += 0.5;}
	}
	health = 0.95*health + 0.05*inH;

	inH = 0.0;
	for(int h = 0; h < 10; ++h)
	{
		pidx = FOOD_POS_Y*resX + FOOD_POS_X + FOOD_POS_DX*h;
		if((RGBA[pidx].x < 0.2) && (RGBA[pidx].y < 0.2) && (RGBA[pidx].z < 0.2))
		{}
		else
		{inH += 1.0;}
	}
	hunger = 0.95*hunger + 0.05*inH;
}