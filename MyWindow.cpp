#include "MyWindow.h"

using namespace std;

extern HINSTANCE g_hInstance;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam) {
	switch (msgID)
	{
	case WM_SYSCOMMAND:
		if (OnSyscommand(hWnd, wParam))
			return 0;
		break;
	case WM_COMMAND:
		OnCommand(hWnd, wParam);
		break;
	case WM_DESTROY:
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

void OnIDOK(HWND hWndDlg) {
	string content1;
	HWND hEdit1;
	hEdit1 = GetDlgItem(hWndDlg, IDC_EDIT1);
	TCHAR content[256];
	GetDlgItemText(hWndDlg, IDC_EDIT1, content, 256);
	content1.resize(wcslen(content));
	for (int i = 0; i < wcslen(content); ++i)
		content1[i] = char(content[i]);
	compress(content1);
}

void OnIDOK1(HWND hWndDlg) {
	string content1;
	HWND hEdit1;
	hEdit1 = GetDlgItem(hWndDlg, IDC_EDIT1);
	TCHAR content[256];
	GetDlgItemText(hWndDlg, IDC_EDIT1, content, 256);
	content1.resize(wcslen(content));
	for (int i = 0; i < wcslen(content); ++i)
		content1[i] = char(content[i]);
	decompress(content1);
}

INT_PTR CALLBACK DlgProc(HWND hWndDlg, UINT msgID, WPARAM wParam, LPARAM lParam) {
	
	switch (msgID) {
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
			EndDialog(hWndDlg, 0);
		break;
	case WM_COMMAND:
		string content1;
		switch (LOWORD(wParam)) {
		case IDOK:
			OnIDOK(hWndDlg);
			EndDialog(hWndDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			break;
		case IDOK1:
			OnIDOK1(hWndDlg);
			EndDialog(hWndDlg, 0);
			break;
		case IDCANCEL1:
			EndDialog(hWndDlg, 0);
			break;
		}
		break;
	}
	
	return false;
}

void InitWnd(WNDCLASS& wnd) {
	wnd.cbClsExtra = 0;
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = CreateSolidBrush(RGB(183, 150, 200));
	wnd.hCursor = nullptr;
	wnd.hIcon = LoadIcon(g_hInstance, (PCTSTR)IDI_ICON1);
	wnd.hInstance = g_hInstance;
	wnd.lpfnWndProc = WndProc;
	wnd.lpszClassName = TEXT("Main");
	wnd.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
}

bool OnSyscommand(HWND hWnd, WPARAM wParam) {
	if (wParam == SC_CLOSE) {
		int nRet = MessageBox(hWnd, TEXT("\t确认要关闭吗？"), TEXT("提示"), MB_YESNO);
		if (nRet == IDNO)
			return true;
	}
	return false;
}

void OnCommand(HWND hWnd, WPARAM wParam) {
	switch (LOWORD(wParam))
	{
	case ID_CLOSE:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case ID_ABOUT:
		DialogBox(g_hInstance, (PCTSTR)IDD_DIALOG3, hWnd, DlgProc);
		break;
	case ID_COMPRESSOR:
		DialogBox(g_hInstance, (PCTSTR)IDD_DIALOG1, hWnd, DlgProc);
		break;
	case ID_DECOMPRESSOR:
		DialogBox(g_hInstance, (PCTSTR)IDD_DIALOG2, hWnd, DlgProc);
		break;
	case IDB_ONE:
		DialogBox(g_hInstance, (PCTSTR)IDD_DIALOG1, hWnd, DlgProc);
		break;
	case IDB_TWO:
		DialogBox(g_hInstance, (PCTSTR)IDD_DIALOG2, hWnd, DlgProc);
		break;
	}
}