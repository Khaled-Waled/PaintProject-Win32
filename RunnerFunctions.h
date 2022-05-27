#pragma once
#include <windows.h> 
#include <iostream>
#include "Header.h"
#include "LineAlgos.h"

inline void contLineDraw(HWND hwnd, HDC hdc, LPARAM lp, void (*DrawLine) (HDC, int,int,int,int, COLORREF))
{
	static int p[4];
	static int idx = 0;

	int x;
	int y;

	x = LOWORD(lp);
	y = HIWORD(lp);
	p[idx] = x;
	p[idx + 1] = y;
	idx = idx + 2;
	if (idx % 2 == 0)
	{
		hdc = GetDC(hwnd);
		DrawLine(hdc, p[0], p[1], p[2], p[3], RGB(255, 0, 0));
		ReleaseDC(hwnd, hdc);
	}
	if (idx == 4)
		idx = 0;

}
