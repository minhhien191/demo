#include "stdafx.h"
#include "HenGio.h"


int getHourBegin(HWND hDlg)
{ 
	return GetDlgItemInt(hDlg, IDC_HH, NULL, NULL);
}
int getMinuteBegin(HWND hDlg)
{
	return GetDlgItemInt(hDlg, IDC_MM, NULL, NULL);
}
int getHourEnd(HWND hDlg)
{
	return GetDlgItemInt(hDlg, IDC_HH2, NULL, NULL);
}
int getMinuteEnd(HWND hDlg)
{
	return GetDlgItemInt(hDlg, IDC_MM2, NULL, NULL);
}
int getFunction(HWND hDlg)
{
	//TCHAR temp[50];
	//GetDlgItemText(hDlg, IDC_CHUCNANG, temp, 50);
	return SendMessage(GetDlgItem(hDlg,IDC_CHUCNANG), CB_GETCURSEL, 0, 0);
}