#include <Windows.h>
#include <iostream>
#include "resource.h"

class BasicWindow{

public:

	//current instance, height and width of the window
	BasicWindow(HINSTANCE instance, int h, int w, WNDPROC wp){
	
		hInstance = instance;
		height = h;
		width = w;
		wndproc = wp;

	};

	int showWindow(int nCmdShow){
	
		if(!registerWindow())
			return 0;
		if(!createWindow(nCmdShow))
			return 0;
		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);

		//step 3, message loop
		while(GetMessage(&msg, NULL, 0, 0) > 0){
	
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		
		return msg.wParam;

	}

	~BasicWindow(){};

private:

	int registerWindow(){
	
		//step 1, register the window class
		wc.cbSize	= sizeof(WNDCLASSEX);
		wc.style	= 0;
		wc.lpfnWndProc = wndproc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
		wc.lpszClassName = L"myWindowClass";
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if(!RegisterClassEx(&wc)){
			MessageBox(NULL, L"Window registration failed.", L"Error", MB_ICONEXCLAMATION | MB_OK); 
			return 0;
		}
		return 1;
	};

	int createWindow(int nCmdShow){
	
		//step 2, create the window
		hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"myWindowClass", L"Title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, height, width, NULL, NULL, hInstance, NULL);
		
		if(hwnd == NULL){
			MessageBox(NULL, L"", L"", MB_ICONEXCLAMATION | MB_OK);
			return 0;
		}
		return 1;
	};

	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	HINSTANCE hInstance;
	WNDPROC wndproc;
	int height, width;
};