#include "MyWindow.h"



using namespace std;

HINSTANCE g_hInstance = { 0 };

int CALLBACK WinMain(HINSTANCE hIns, HINSTANCE hPreIns, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hIns;

	WNDCLASS wnd = { 0 };
	InitWnd(wnd);
	RegisterClass(&wnd);

	HMENU hMenu = LoadMenu(hIns, (PCTSTR)IDR_MENU1);
	HWND hWnd = CreateWindow(TEXT("Main"), TEXT("iKUN Compressor"), WS_OVERLAPPEDWINDOW^ WS_MAXIMIZEBOX^ WS_THICKFRAME,
		400, 100, 700, 500, nullptr, hMenu, hIns, nullptr);
	HWND hButton1= CreateWindow(TEXT("Button"), TEXT("—πÀı"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		40, 175, 250, 150, hWnd, (HMENU)IDB_ONE, hIns, NULL);
	HWND hButton2 = CreateWindow(TEXT("Button"), TEXT("Ω‚—πÀı"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		390, 175, 250, 150, hWnd, (HMENU)IDB_TWO, hIns, NULL);

	ShowWindow(hWnd, SW_SHOW);
	ShowWindow(hButton1, SW_SHOW);
	ShowWindow(hButton2, SW_SHOW);
	UpdateWindow(hWnd);
	UpdateWindow(hButton1);
	UpdateWindow(hButton2);

	MSG nMsg = { 0 };
	while (true) {
		if (PeekMessage(&nMsg, nullptr, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&nMsg, nullptr, 0, 0))
				break;
			TranslateMessage(&nMsg);
			DispatchMessageW(&nMsg);
		}
	}

	return 0;
}