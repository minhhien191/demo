#pragma once

#include "stdafx.h"
#include "DlgClock.h"

/*
* Return:
	Lỗi: -1


*/

int getHourBegin(HWND hDlg);
int getMinuteBegin(HWND hDlg);
int getHourEnd(HWND hDlg);
int getMinuteEnd(HWND hDlg);

/*
* Return:
	-1: Lỗi
	0: Báo thức
	1: Khóa bàn phím
	2: Tắt máy
*/
int getFunction(HWND hDlg);