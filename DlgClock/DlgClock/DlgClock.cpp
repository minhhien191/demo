// DlgClock.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DlgClock.h"
#include "HenGio.h"
#include <iostream>
#include <vector>
#include <MMSystem.h>
#include <shellapi.h>
#include<commdlg.h>
using namespace std;
#define WM_USER_SHELLICON 1111 
#define MAX_LOADSTRING 100
#define ID_TIMER    1
int flagShow = 0;
// Global Variables:
NOTIFYICONDATA nidApp;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWnd1, hdlg1;
vector<CONGVIEC> cv;
int countCV = -1;
int pos;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	HenGioProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	dlgListProc(HWND, UINT, WPARAM, LPARAM);
void doPlayMusic(HWND hWnd);
void doLockKeyboard(HWND hWnd);
void doShow_Hiden(HWND hWnd2, HWND hWnd3);
void doInitRunning(TCHAR*lenh, TCHAR *key);
void doReRunning(TCHAR*lenh, TCHAR *key);
void doRemoveHook(HWND hWnd);
void OpenDlg(HWND hWnd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DLGCLOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DLGCLOCK));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLGCLOCK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{


   hInst = hInstance; // Store instance handle in our global variable
  
   hWnd1 = CreateWindow(szWindowClass, TEXT("Digital Clock"),
	   WS_OVERLAPPEDWINDOW,
	   600, 150,
	   400, 150,
	   NULL, NULL, hInstance, NULL);
   hdlg1 = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_LIST), hWnd1, dlgListProc);
   if (!hWnd1)
   {
      return FALSE;
   }
   nidApp.cbSize = sizeof(NOTIFYICONDATA);
   nidApp.hWnd = hWnd1;
   nidApp.uID = IDI_SMALL;
   nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   nidApp.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DLGCLOCK));
   //  StringCchCopy(nidApp.szTip, ARRAYSIZE(nidApp.szTip), L"Test application");
   nidApp.uCallbackMessage = WM_USER_SHELLICON;
   wcsncpy_s(nidApp.szTip, L"My Clock", sizeof(nidApp.szTip));
   Shell_NotifyIcon(NIM_ADD, &nidApp);

   doShow_Hiden(hWnd1, hdlg1);
   ShowWindow(hWnd1, SW_SHOW);
   SetWindowPos(hdlg1, hWnd1, 1000, 150, 0, 0, TRUE);
   ShowWindow(hdlg1, SW_SHOW);
   UpdateWindow(hWnd1);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


void initComboBox(HWND hDlg)
{

	//chuc nang
	SendMessage(GetDlgItem(hDlg, IDC_CHUCNANG), CB_ADDSTRING, 0, (LPARAM)L"Khóa bàn phím");
	SendMessage(GetDlgItem(hDlg, IDC_CHUCNANG), CB_ADDSTRING, 0, (LPARAM)L"Báo thức");
	SendMessage(GetDlgItem(hDlg, IDC_CHUCNANG), CB_ADDSTRING, 0, (LPARAM)L"Chạy Lệnh nhanh");
	// lenh
	SendMessage(GetDlgItem(hDlg, IDC_LENH), CB_ADDSTRING, 0, (LPARAM)L"calc");
	SendMessage(GetDlgItem(hDlg, IDC_LENH), CB_ADDSTRING, 0, (LPARAM)L"mspaint");
	SendMessage(GetDlgItem(hDlg, IDC_LENH), CB_ADDSTRING, 0, (LPARAM)L"notepad");
	SendMessage(GetDlgItem(hDlg, IDC_LENH), CB_ADDSTRING, 0, (LPARAM)L"iexplore");
	


	//gio bat dau
	TCHAR temp[3];
	for (int i = 0; i < 24; i++){
		wsprintf(temp, L"%.2d", i);
		SendMessage(GetDlgItem(hDlg, IDC_HH), CB_ADDSTRING, 0, (LPARAM)temp);
	}

	for (int i = 0; i < 24; i++){
		wsprintf(temp, L"%.2d", i);
		SendMessage(GetDlgItem(hDlg, IDC_HH2), CB_ADDSTRING, 0, (LPARAM)temp);
	}

	for (int i = 0; i < 60; i++){
		wsprintf(temp, L"%.2d", i);
		SendMessage(GetDlgItem(hDlg, IDC_MM), CB_ADDSTRING, 0, (LPARAM)temp);
	}

	for (int i = 0; i < 60; i++){
		wsprintf(temp, L"%.2d", i);
		SendMessage(GetDlgItem(hDlg, IDC_MM2), CB_ADDSTRING, 0, (LPARAM)temp);
	}


}

void DisplayDigit(HDC hdc, int iNumber)
{
	static BOOL  fSevenSegment[10][7] = {
		1, 1, 1, 0, 1, 1, 1,     // 0
		0, 0, 1, 0, 0, 1, 0,     // 1
		1, 0, 1, 1, 1, 0, 1,     // 2
		1, 0, 1, 1, 0, 1, 1,     // 3
		0, 1, 1, 1, 0, 1, 0,     // 4
		1, 1, 0, 1, 0, 1, 1,     // 5
		1, 1, 0, 1, 1, 1, 1,     // 6
		1, 0, 1, 0, 0, 1, 0,     // 7
		1, 1, 1, 1, 1, 1, 1,     // 8
		1, 1, 1, 1, 0, 1, 1 };  // 9
	static POINT ptSegment[7][6] = {
		7, 6, 11, 2, 31, 2, 35, 6, 31, 10, 11, 10,
		6, 7, 10, 11, 10, 31, 6, 35, 2, 31, 2, 11,
		36, 7, 40, 11, 40, 31, 36, 35, 32, 31, 32, 11,
		7, 36, 11, 32, 31, 32, 35, 36, 31, 40, 11, 40,
		6, 37, 10, 41, 10, 61, 6, 65, 2, 61, 2, 41,
		36, 37, 40, 41, 40, 61, 36, 65, 32, 61, 32, 41,
		7, 66, 11, 62, 31, 62, 35, 66, 31, 70, 11, 70 };
	int          iSeg;

	for (iSeg = 0; iSeg < 7; iSeg++)
	if (fSevenSegment[iNumber][iSeg])
		Polygon(hdc, ptSegment[iSeg], 6);
}

void DisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
	if (!fSuppress || (iNumber / 10 != 0))
		DisplayDigit(hdc, iNumber / 10);

	OffsetWindowOrgEx(hdc, -42, 0, NULL);
	DisplayDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -42, 0, NULL);
}

void DisplayColon(HDC hdc)
{
	POINT ptColon[2][4] = { 2, 21, 6, 17, 10, 21, 6, 25,
		2, 51, 6, 47, 10, 51, 6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void DisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st;

	GetLocalTime(&st);

	if (f24Hour)
		DisplayTwoDigits(hdc, st.wHour, fSuppress);
	else
		DisplayTwoDigits(hdc, (st.wHour ) ? st.wHour : 12, fSuppress);

	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wMinute, FALSE);
	DisplayColon(hdc);
	DisplayTwoDigits(hdc, st.wSecond, FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL   f24Hour, fSuppress;
	static HBRUSH hBrushRed;
	static int    cxClient, cyClient;
	HDC           hdc;
	PAINTSTRUCT   ps;
	TCHAR         szBuffer[2];

	int wmId, wmEvent;
	switch (message)
	{
	case WM_CREATE:
		hBrushRed = CreateSolidBrush(RGB(0, 0, 0));
		SetTimer(hWnd, ID_TIMER, 1000, NULL);
		// fall through
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_HENGIO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HENGIO), hWnd, HenGioProc);
			break;
		case IDM_EXIT:
			KillTimer(hWnd, ID_TIMER);
			DeleteObject(hBrushRed);
			PostQuitMessage(0);
		}
		break;
	case WM_SETTINGCHANGE:
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
		f24Hour = (szBuffer[0] == '1');

		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
		fSuppress = (szBuffer[0] == '0');

		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_TIMER:
	{
					 SYSTEMTIME st;
					 GetLocalTime(&st);
					 InvalidateRect(hWnd, NULL, TRUE);

					 for (int i = 0; i < cv.size(); i++)
					 {

						 if (cv[i].Phut == st.wMinute && cv[i].Gio == st.wHour)
						 {
							 if (cv[i].cn == 0)
							 {
								 PlaySound(TEXT("CSSD.wav"), NULL, SND_ASYNC||SND_LOOP);
								 doPlayMusic(hWnd1);
							 }
							 if (cv[i].cn == 2)
							 {
								 doLockKeyboard(hWnd1);
							 }
							 if (cv[i].cn == 1)
							 {
								 doReRunning(cv[i].Lenh, cv[i].key);
								 // goi ham go bo phim da dc cai dat
								 //ShellExecute(hWnd, NULL,cv[i].Lenh, NULL, NULL, SW_SHOWNORMAL);
							 }
							 TCHAR THONGDIEP[100];
							 wsprintf(THONGDIEP, L" %s - %s - %dh - %dp ", cv[i].Noidung, cv[i].ChucNang, cv[i].Gio, cv[i].Phut);
							 HWND hwndList = GetDlgItem(hdlg1, IDC_LIST1);
							 SendMessage(hwndList, LB_DELETESTRING, (WPARAM)i, 0);
							 cv.erase(cv.begin() + i);
							 MessageBox(NULL, THONGDIEP, L"Thông báo", MB_OKCANCEL);
							
						 }
					 }


					 return 0;
	}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetMapMode(hdc, MM_ISOTROPIC);
		SetWindowExtEx(hdc, 276, 100, NULL);
		SetViewportExtEx(hdc, cxClient, cyClient, NULL);

		SetWindowOrgEx(hdc, 155, 36, NULL);
		SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);

		SelectObject(hdc, GetStockObject(NULL_PEN));
		SelectObject(hdc, hBrushRed);

		DisplayTime(hdc, f24Hour, fSuppress);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_USER_SHELLICON:
		switch (LOWORD(lParam))
		{
		case WM_LBUTTONDOWN:
			if (flagShow == 0)
			{
				flagShow = 1;
				ShowWindow(hWnd1, SW_SHOW);
				ShowWindow(hdlg1, SW_SHOW);
			}
			else
			{
				flagShow = 0;
				ShowWindow(hWnd1, SW_HIDE);
				ShowWindow(hdlg1, SW_HIDE);
			}
			break;
		}
		break;

	case WM_DESTROY:
		doRemoveHook(hWnd);
		KillTimer(hWnd, ID_TIMER);
		DeleteObject(hBrushRed);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

INT_PTR CALLBACK HenGioProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList = GetDlgItem(hdlg1, IDC_LIST1);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{

	case WM_INITDIALOG:
		initComboBox(hDlg);
		EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_LENH), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_AMBAO), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_KEY), FALSE);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_AMBAO:
			OpenDlg(hDlg);
			break;
		case IDC_CHUCNANG:
			switch (HIWORD(wParam))
			{

			case CBN_SELCHANGE:
			{
								   EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
								   
								   if (getFunction(hDlg) == 1)
								   {
									   EnableWindow(GetDlgItem(hDlg, IDC_LENH), TRUE);
									   EnableWindow(GetDlgItem(hDlg, IDC_AMBAO), TRUE);
									   EnableWindow(GetDlgItem(hDlg, IDC_KEY), TRUE);
								   }
								   else
								   {

									   EnableWindow(GetDlgItem(hDlg, IDC_LENH), FALSE);
									   EnableWindow(GetDlgItem(hDlg, IDC_AMBAO), FALSE);
									   EnableWindow(GetDlgItem(hDlg, IDC_KEY), FALSE);
								   }
								 

			}
			}
			break;
		case IDOK:
		{
					 CONGVIEC x;
					 TCHAR s[100],Gio[20], Phut[20];
					 countCV++;
					 
					 GetWindowText(GetDlgItem(hDlg, IDC_NOIDUNG), x.Noidung, 20);
					 GetWindowText(GetDlgItem(hDlg, IDC_CHUCNANG), x.ChucNang, 20);
					 GetWindowText(GetDlgItem(hDlg, IDC_HH),Gio , 20);
					 GetWindowText(GetDlgItem(hDlg, IDC_MM), Phut, 20);
					 GetWindowText(GetDlgItem(hDlg, IDC_LENH), x.Lenh, 100);
					 GetWindowText(GetDlgItem(hDlg, IDC_KEY), x.key, sizeof(x.key));
					
					 x.Gio = _ttoi(Gio);
					 x.Phut = _ttoi(Phut);
					 x.pos = countCV;
					 x.cn = getFunction(hDlg);
					 if (x.cn == 1)
					 {
						 //MessageBox(NULL, x.key, L"ád", MB_OK);
						 doInitRunning(x.Lenh, x.key);
						 
						 // goi ham cai dat phim da dc cai dat
						 //ShellExecute(hWnd, NULL,cv[i].Lenh, NULL, NULL, SW_SHOWNORMAL);
					 }
					 /*
					 TCHAR temp[10];
					 wsprintf(temp, L"%d + %d", x.cn, getFunction(hdlg1));
					 MessageBox(NULL, temp, L"caption", MB_OK);
					 */
					 cv.push_back(x);
					 wsprintf(s, L"%d - %s - %s- %s - %dh : %dp",x.pos,x.Noidung,x.ChucNang,x.key,x.Gio,x.Phut);
					 SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)s);
					  //Test hàm getFuntion
					
					  EndDialog(hDlg, LOWORD(wParam));
					  return (INT_PTR)TRUE;
		}
		case IDCANCEL:
		{
					 EndDialog(hDlg, LOWORD(wParam));
					 return (INT_PTR)TRUE;
		}
			break;
		}
	}
	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		
	case WM_INITDIALOG:
		initComboBox(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK dlgListProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	int wmId, wmEvent;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		//doInstallHook(hDlg);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		
		case IDC_LIST1:
		{
						  HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);
						  pos = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
						  if (pos != LB_ERR)
							  EnableWindow(GetDlgItem(hDlg, IDC_XOA), TRUE);
						  else
							  EnableWindow(GetDlgItem(hDlg, IDC_XOA), FALSE);
						  //SendMessage(hwndList, LB_DELETESTRING, 0, 0);
						  return TRUE;
		}
		case IDC_THEM:
		{
						 DialogBox(hInst, MAKEINTRESOURCE(IDD_HENGIO), hWnd1, HenGioProc);
						 return TRUE;
		}
		case IDC_XOA:
		{
						HWND hwndList = GetDlgItem(hDlg, IDC_LIST1);
						SendMessage(hwndList, LB_DELETESTRING, (WPARAM)pos, 0);
						doReRunning(cv[pos].Lenh, cv[pos].key);
						cv.erase(cv.begin() + pos);
						
						EnableWindow(GetDlgItem(hDlg, IDC_XOA), FALSE);
						return TRUE;
		}
			default:
				break;

		}
		break;

	}


	return (INT_PTR)FALSE;
}

// hook
void doLockKeyboard(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	hinstLib = LoadLibrary(L"Clock_DLL.dll");
	
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doLockKeyBoard");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}
void doPlayMusic(HWND hWnd)
{
	typedef VOID(*MYPROC)(HWND);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	hinstLib = LoadLibrary(L"Clock_DLL.dll");

	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doPlayMusic");
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}
void doShow_Hiden(HWND hWnd2,HWND hWnd3)
{
	typedef VOID(*MYPROC)(HWND, HWND);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	hinstLib = LoadLibrary(L"Clock_DLL.dll");

	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doShow_Hiden");
		if (ProcAddr != NULL)
			ProcAddr(hWnd1,hdlg1);
	}
}
void doInitRunning(TCHAR*lenh, TCHAR *key)
{
	typedef VOID(*MYPROC)(TCHAR *, TCHAR *);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	hinstLib = LoadLibrary(L"Clock_DLL.dll");

	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doInitRunning");
		if (ProcAddr != NULL)
			ProcAddr(lenh,key );
	}
}
void doReRunning(TCHAR*lenh, TCHAR *key)
{
	typedef VOID(*MYPROC)(TCHAR *, TCHAR *);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;
	hinstLib = LoadLibrary(L"Clock_DLL.dll");

	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doReRunning");
		if (ProcAddr != NULL)
			ProcAddr(lenh, key);
	}
}


void doRemoveHook(HWND hWnd)
{
	// gọi hàm DLL theo kiểu Run-time
	// Định nghĩa prototype của hàm
	typedef VOID(*MYPROC)(HWND);

	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	// load DLL và lấy handle của DLL module
	hinstLib = LoadLibrary(L"DLL_Tuan9.dll");
	// Nếu load thành công, lấy địa chỉ của hàm DrawCircle trong DLL
	if (hinstLib != NULL) {
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doRemoveHook");
		//MessageBox(hWnd, L"remove hook keyboard", L"thong bao", MB_OK);
		// Nếu lấy được địa chỉ hàm, gọi thực hiện hàm
		if (ProcAddr != NULL)
			ProcAddr(hWnd);
	}
}

void OpenDlg(HWND hWnd)
{

	OPENFILENAME ofn; // CTDL dùng cho dialog open
	TCHAR szFile[256];
	//TCHAR szFilter[] = TEXT("Word file\0 * .doc\0Excel file\0 * .xls\0Text file\0 * .txt\0");
	TCHAR szFilter[] = TEXT("ALL file\0 *\0");
	szFile[0] = '\0';
	// Khởi tạo struct
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd; // handle của window cha
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile; // chuỗi tên file trả về
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn))
	{
		SetWindowText(GetDlgItem(hWnd, IDC_LENH), ofn.lpstrFile);
	}
	else
		return;

}