#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define NTDDI_VERSION NTDDI_WINXP

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <Windows.h>

#include "resource.h"

// Main dialog window
HWND hMainDlg = NULL;

// Window procedure
INT_PTR CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_USER:
		switch (lParam)
		{
		case NIN_BALLOONUSERCLICK:
		case NIN_SELECT:
			if (hMainDlg != NULL)
			{
				ShowWindow(hMainDlg, SW_SHOW);
			}
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TEST_BTN:
			ShowWindow(hMainDlg, SW_HIDE);
			break;
		case IDC_EXIT_BTN:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_CLOSE)
		{
			ShowWindow(hMainDlg, SW_HIDE);
			break;
		}
		else
			return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_CLOSE:
		ShowWindow(hMainDlg, SW_HIDE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
	hMainDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_TEST_DIALOG), NULL, DlgProc);
	DWORD e = GetLastError();
	if (hMainDlg == NULL)
	{
		return 1;
	}
	SetWindowPos(hMainDlg, NULL, 800, 400, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);

	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(nid);
	nid.hWnd = hMainDlg;
	nid.uID = 0;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_USER;
	nid.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_TRAY_ICON), IMAGE_ICON, 0, 0, 0));
	wcscpy_s(nid.szTip, L"Click me");
	wcscpy_s(nid.szInfo, L"I dare you, I double dare you, click me one more time!");
	nid.uVersion = NOTIFYICON_VERSION;
	wcscpy_s(nid.szInfoTitle, L"Click me");
	nid.dwInfoFlags = NIIF_WARNING;
	Shell_NotifyIcon(NIM_ADD, &nid);
	Shell_NotifyIcon(NIM_SETVERSION, &nid);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hMainDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Shell_NotifyIcon(NIM_DELETE, &nid);
	DestroyIcon(nid.hIcon);

	return 0;
}
