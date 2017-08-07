#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hButton;
	static BOOL bFullScreen;
	switch (msg)
	{
	case WM_CREATE:
		hButton = CreateWindow(TEXT("BUTTON"), TEXT("フルスクリーン"), WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd, (HMENU)1000, ((LPCREATESTRUCT)lParam)->hInstance, 0);
		break;
	case WM_SIZE:
		MoveWindow(hButton, 10, 10, 256, 32, TRUE);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 1000)
		{
			HWND hTray = FindWindow(TEXT("Shell_TrayWnd"), NULL);
			if (hTray)
			{
				if (bFullScreen)
				{
					ShowWindow(hTray, SW_SHOW);

					APPBARDATA ABData;
					ABData.cbSize = sizeof(APPBARDATA);
					ABData.hWnd = hTray;
					ABData.lParam = FALSE;
					SHAppBarMessage(ABM_SETSTATE, &ABData);

					SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) | WS_BORDER | WS_DLGFRAME | WS_THICKFRAME);
					SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

					ShowWindow(hWnd, SW_RESTORE);

					bFullScreen = FALSE;
				}
				else
				{
					ShowWindow(hTray, SW_SHOW);

					APPBARDATA ABData;
					ABData.cbSize = sizeof(APPBARDATA);
					ABData.hWnd = hTray;
					ABData.lParam = ABS_AUTOHIDE;
					SHAppBarMessage(ABM_SETSTATE, &ABData);

					SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_BORDER & ~WS_DLGFRAME & ~WS_THICKFRAME);
					SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);

					ShowWindow(hWnd, SW_MAXIMIZE);

					bFullScreen = TRUE;
				}
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	ACCEL Accel[] = { { FVIRTKEY,VK_F11,1000 } };
	HACCEL hAccel = CreateAcceleratorTable(Accel, 1);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	DestroyAcceleratorTable(hAccel);
	return (int)msg.wParam;
}
