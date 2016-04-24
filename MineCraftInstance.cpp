#include "MineCraftInstance.h"

void MineCraftInstance::initialize(const wchar_t* windowNameIn)
{
	RECT rect;
	WINDOWPOS wpos;

	swprintf(windowName,__TEXT("%s"),windowNameIn);
	wprintf(__TEXT("looking for %s instance... \n"),windowName);
	hwnd = FindWindow(NULL,windowName);
	if(hwnd)
	{
		printf("Instance found!\n");
		printf("hwnd = %d\n",hwnd);

		SetWindowPos(hwnd,NULL,0,0,208,227,NULL);
		
		GetClientRect(hwnd,&rect);
		resX = rect.right;
		resY = rect.bottom;
		
		GetWindowRect(hwnd,&rect);
		posX = rect.left+4;
		posY = rect.top+23;

		printf("%d,%d,%d,%d\n",rect.bottom,rect.top,rect.right,rect.left);

		ZeroMemory(&bih,sizeof(BITMAPINFOHEADER));
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biPlanes = 1;
		bih.biBitCount = 32;
		bih.biWidth = resX;
		bih.biHeight = resY;
		bih.biCompression = BI_RGB;
		bih.biSizeImage = resX*resY;

		imageData = new BYTE[4*resX*resY];
		ZeroMemory(imageData,4*resX*resY);

		printf("Window dimensions %d,%d\n",resX,resY);
	}
	else{printf("Failed to find Minecraft instance!\n");system("PAUSE");}
}

void MineCraftInstance::captureWindow()
{
	hWindowDC = GetDC(NULL);
	hMemoryDC = CreateCompatibleDC(hWindowDC);

	hBitmap = CreateCompatibleBitmap(hWindowDC,resX,resY);
	hBitmapOld = (HBITMAP)SelectObject(hMemoryDC,hBitmap);
	BitBlt(hMemoryDC,0,0,resX,resY,hWindowDC,posX,posY,SRCCOPY);
	SelectObject(hMemoryDC,hBitmapOld);
	ZeroMemory(imageData,4*resX*resY);
	GetDIBits(hMemoryDC,hBitmap,0,resY,imageData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);

	ReleaseDC(hwnd,hWindowDC);
	DeleteDC(hMemoryDC);
	DeleteObject((HGDIOBJ)hBitmap);
	DeleteObject((HGDIOBJ)hBitmapOld);
}

void MineCraftInstance::sendKeyboardInput(int key, int pressed)
{
	if(pressed)
	{
		PostMessage(hwnd,WM_KEYDOWN,key,NULL);
	}
	else
	{
		PostMessage(hwnd,WM_KEYUP,key, (1<<0)|(1<<30)|(1<<31) );
	}
}

void MineCraftInstance::sendMouseWheelInput(int dir)
{
	PostMessage(hwnd,WM_MOUSEWHEEL,(WHEEL_DELTA*dir)<<16,NULL);
}

void MineCraftInstance::sendMouseMoveInput(float deltaX, float deltaY)
{
	short int horz = (0.5+deltaX)*resX;
	short int vert = (0.5+deltaY)*resY;

	LPARAM lparam = horz|(vert<<16);

	PostMessage(hwnd,WM_MOUSEMOVE,NULL,lparam);
}

void MineCraftInstance::sendMouseButtonInput(int key, int pressed)
{
	switch(key)
	{
		case LMBUTTON:
		{
			if(pressed)
			{
				PostMessage(hwnd,WM_LBUTTONDOWN,key,NULL);
			}
			else
			{
				PostMessage(hwnd,WM_LBUTTONUP,key, (1<<0)|(1<<30)|(1<<31) );
			}
			break;
		}
		case RMBUTTON:
		{
			if(pressed)
			{
				PostMessage(hwnd,WM_RBUTTONDOWN,key,NULL);
			}
			else
			{
				PostMessage(hwnd,WM_RBUTTONUP,key, (1<<0)|(1<<30)|(1<<31) );
			}
			break;
		}
	}
}