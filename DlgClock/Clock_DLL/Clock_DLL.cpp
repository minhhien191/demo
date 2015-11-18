// Clock_DLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <MMSystem.h>
#include<Windows.h>
#include <shellapi.h>
#include<vector>
using namespace std;
#define EXPORT  __declspec(dllexport)
struct ChayLenh
{
	int key;
	TCHAR Lenh[100];
};
HHOOK hHook = NULL, hHook1 = NULL,hHook2 = NULL,hHook3 = NULL,hHook4 = NULL;
HINSTANCE hinstLib;
HWND hWndClock, hWndList;
int flagWindow = 1;
int AltKeyDown = 0;
int flag = 1;
vector<ChayLenh> Run;
ChayLenh Running;

LRESULT CALLBACK Key(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		//if (wParam == WM_RBUTTONDOWN && (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000))
		if (wParam == WM_LBUTTONDOWN)
		{
			for (int i = 0; i < Run.size(); i++)
			{
				if (HIWORD(GetKeyState(Run[i].key)))
				{
					ShellExecute(hWndClock, NULL, Run[i].Lenh, NULL, NULL, SW_SHOWNORMAL);
					//return CallNextHookEx(hHook4, nCode, wParam, lParam);
				}
			}
		}
		return CallNextHookEx(hHook4, nCode, wParam, lParam);
	}
	return CallNextHookEx(hHook4, nCode, wParam, lParam);
}
LRESULT CALLBACK LockKeyBoard(int nCode, WPARAM wParam, LPARAM lParam)
{

	KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)lParam;
	if (wParam == WM_KEYDOWN)
	{
		switch (p->vkCode)
		{
		case VK_LCONTROL:
			AltKeyDown = 1; break;
		case 0x51:
			if (AltKeyDown == 1)
			{
				UnhookWindowsHookEx(hHook);
				UnhookWindowsHookEx(hHook3);
				hHook = NULL;
				hHook3 = NULL;
				return 1;
			}
		}
	}
	return 1;
}
LRESULT CALLBACK PlayMusic(int nCode, WPARAM wParam, LPARAM lParam)
{

	KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)lParam;
	if (wParam == WM_KEYDOWN)
	{
		switch (p->vkCode)
		{
		case VK_LCONTROL:
			AltKeyDown = 1; break;
		case 0x45:
			if (AltKeyDown == 1)
			{
				PlaySound(NULL, 0, 0);
				UnhookWindowsHookEx(hHook1);
				hHook1 = NULL;
				
			}
		}
	}
	return CallNextHookEx(hHook1, nCode, wParam, lParam);
}
LRESULT CALLBACK LockMouse(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 1;
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT*)lParam;
	if (nCode >= 0)
	{
		if (wParam == WM_LBUTTONDOWN)
		{
			for (int i = 0; i < Run.size(); i++)
			{
				if (HIWORD(GetKeyState(Run[i].key)))
				{
					ShellExecute(hWndClock, NULL, Run[i].Lenh, NULL, NULL, SW_SHOWNORMAL);
					//return CallNextHookEx(hHook4, nCode, wParam, lParam);
				}
			}
		}
		//if (wParam == WM_RBUTTONDOWN && (GetKeyState(VK_LCONTROL) & 0x8000 || GetKeyState(VK_RCONTROL) & 0x8000))
		if (wParam == WM_LBUTTONDOWN && HIWORD(GetKeyState(VK_LCONTROL)) || HIWORD(GetKeyState(VK_RCONTROL)))
		{
			if (flagWindow == 0)
			{
				ShowWindow(hWndClock, SW_SHOW);
				ShowWindow(hWndList, SW_SHOW);

				flagWindow = 1;
			}
			else
			{
				ShowWindow(hWndClock, SW_HIDE);
				ShowWindow(hWndList, SW_HIDE);

				flagWindow = 0;
			}
		}
	}

	//MessageBox(NULL, szBuf, L"KeyboardHook", MB_OK);

	return CallNextHookEx(hHook2, nCode, wParam, lParam);
}
EXPORT void _doLockKeyBoard(HWND hWnd)
{
	if (hHook != NULL) return;
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)LockKeyBoard, hinstLib, 0);
	if (hHook3 != NULL) return;
	hHook3 = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)LockMouse, hinstLib, 0);
}
EXPORT void _doPlayMusic(HWND hWnd)
{
	if (hHook1 != NULL) return;
	hHook1 = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)PlayMusic, hinstLib, 0);
}
EXPORT void _doShow_Hiden(HWND hWnd2, HWND hWnd3 )
{
	hWndClock = hWnd2;
	hWndList = hWnd3;
	if (hHook2 != NULL) return;
	hHook2 = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseProc, hinstLib, 0);
}
EXPORT void _doInitRunning(TCHAR *lenh, TCHAR *key)
{

	wsprintf(Running.Lenh,L"%s",lenh);
	Running.key = key[0];
	Run.push_back(Running);
}
EXPORT void _doReRunning(TCHAR *lenh, TCHAR *key)
{

	for (int i = 0; i < Run.size(); i++)
	{
		if (Run[i].key == key[0])
		{
			Run.erase(Run.begin() + i);
			return;
		}
	}
}
EXPORT void _doRemoveHook(HWND hWnd)
{
	if (hHook != NULL)
	{

		UnhookWindowsHookEx(hHook);
		hHook = NULL;
	}
	if (hHook1 != NULL)
	{
		UnhookWindowsHookEx(hHook1);
		hHook1 = NULL;
	}
	if (hHook2 != NULL)
	{
		UnhookWindowsHookEx(hHook2);
		hHook2 = NULL;
	}
	if (hHook3 != NULL)
	{
		UnhookWindowsHookEx(hHook3);
		hHook3 = NULL;
	}
	if (hHook4 != NULL)
	{
		UnhookWindowsHookEx(hHook3);
		hHook4 = NULL;
	}
}
