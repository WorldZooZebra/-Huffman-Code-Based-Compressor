#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include "resource1.h"
#include "compressor.h"

#define IDB_ONE 4001
#define IDB_TWO 4006

LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam);
void InitWnd(WNDCLASS& wnd);
bool OnSyscommand(HWND hWnd, WPARAM wParam);
void OnCommand(HWND hWnd, WPARAM wParam);
void OnIDOK(HWND hWndDlg);
void OnIDOK1(HWND hWndDlg);