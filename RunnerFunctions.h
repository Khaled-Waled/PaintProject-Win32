#pragma once
#include <windows.h> 
#include <iostream>
#include "Header.h"
#include "LineAlgos.h"
#include "CircleAlgos.h"

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

inline void contCircleDraw(HWND hwnd, HDC hdc, LPARAM lp, void (*DrawCircle) (HDC, int, int, int, COLORREF))
{
	static Vector p[10];
	static int index = 0;
	p[index] = Vector(LOWORD(lp), HIWORD(lp));
	if (index == 1)
	{
		int x2 = (p[0][0] - p[1][0]) * (p[0][0] - p[1][0]);
		int y2 = (p[0][1] - p[1][1]) * (p[0][1] - p[1][1]);
		p[1] = Vector(Round(sqrt(x2 + y2)), 0);
		hdc = GetDC(hwnd);
		DrawCircle(hdc, p[0][0], p[0][1], p[1][0], RGB(255, 0, 0));
		ReleaseDC(hwnd, hdc);
		index = 0;
	}
	else index++;
}
