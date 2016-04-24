#include "PicoEngine.h"
#include <io.h>
#include <fcntl.h>
#include <windowsx.h>

PicoInputState _GLOBAL_inputState;

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	RECT wndpos;
	int wndW,wndH;
	char keyin;
	
	switch(umessage)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}
		case WM_KEYDOWN:
		{
			if(wparam == VK_ESCAPE)
			{PostQuitMessage(0);}
			return 0;
			break;
		}

		case WM_INPUT: 
		{
			GetWindowRect(hwnd,&wndpos);
			wndW = (wndpos.right - wndpos.left);
			wndH = (wndpos.bottom - wndpos.top);
			UINT dwSize;

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dwSize, 
							sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, 
							sizeof(RAWINPUTHEADER));
			RAWINPUT* raw = (RAWINPUT*)lpb;

			if(raw->header.dwType == RIM_TYPEMOUSE) 
			{
				_GLOBAL_inputState._mousePosXDelta = raw->data.mouse.lLastX;
				_GLOBAL_inputState._mousePosYDelta = raw->data.mouse.lLastY;

				if(raw->data.mouse.usButtonFlags == RI_MOUSE_WHEEL)
				{_GLOBAL_inputState._mouseWheelDelta = (short)raw->data.mouse.usButtonData/120.0;}

                if(raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
                {
                    keyin = VK_LBUTTON;
                    _GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 1;
                }

                if(raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
                {
                    keyin = VK_LBUTTON;
                    _GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 0;
                }
                else if(raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_UP)
                {
                    keyin = VK_LBUTTON;
                    _GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 0;
                }
                else{}

                if(raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)
                {
                    keyin = VK_RBUTTON;
                    _GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 1;
                }
                else if(raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_UP)
                {
                    keyin = VK_RBUTTON;
                    _GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 0;
                }
                else{}
			}

			if(raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				keyin = raw->data.keyboard.VKey;
				if(raw->data.keyboard.Message == WM_KEYDOWN)
				{
					_GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 1;

					if(keyin == VK_ESCAPE)
					{
						PostQuitMessage(0);
						return 0;
					}
					
				}
				else if(raw->data.keyboard.Message == WM_KEYUP)
				{
					_GLOBAL_inputState._keyboardStateHasChanged = 1;
					_GLOBAL_inputState._keyboardState[keyin] = 0;
				}
				else{}
			}

			delete[] lpb;
			return 0;
			break;
		}

		default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
	return 0;
}

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev,  LPSTR lpCmd, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	bool quit = false;
	WNDCLASSEX wc;
	RAWINPUTDEVICE rid[2];
	PicoEngine engine;

	wc.style = CS_HREDRAW|CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = __TEXT("Window");
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);
	hwnd = CreateWindowEx(NULL,
						  wc.lpszClassName,
						  __TEXT("Window"),
						  WS_OVERLAPPEDWINDOW,
						  700,100,
						  1440,
						  1080,
						  NULL,NULL,
						  hInst,
						  NULL);

	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = 0;//RIDEV_NOLEGACY;  
	rid[0].hwndTarget = NULL;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = 0;//RIDEV_NOLEGACY;  
	rid[1].hwndTarget = NULL;

	RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE));

	AllocConsole();
    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, NULL, _IONBF, 1);
    *stdout = *hf_out;

	engine.initialize(hwnd,1440,1080);
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while(!quit)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{TranslateMessage(&msg); DispatchMessage(&msg);}

		if(msg.message == WM_QUIT){quit = true;}
		else
		{
			engine.runGameLoop();
		}
	}

	engine.release();

	return 0;
}