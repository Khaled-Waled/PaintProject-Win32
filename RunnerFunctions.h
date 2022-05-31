#pragma once
#include <windows.h> 
#include <iostream>
#include "Header.h"
#include "LineAlgos.h"
#include "CircleAlgos.h"
#include "Splines.h"
using namespace std;


inline double eclidDis(Vector p1, Vector p2) {
	return sqrt(((p1[0] - p2[0]) * (p1[0] - p2[0])) + ((p1[1] - p2[1]) * (p1[1] - p2[1])));
}

inline void contLineDraw(HWND hwnd, HDC hdc, LPARAM lp, void (*DrawLine) (HDC, int, int, int, int, COLORREF), COLORREF color)
{
	static bool flag = false;
	static int p[4];
	static int idx = 0;

	int x;
	int y;

	x = LOWORD(lp);
	y = HIWORD(lp);
	p[idx] = x;
	p[idx + 1] = y;
	idx = idx + 2;

	if (idx % 4 == 0 && flag == false)
	{
		flag = true;
		hdc = GetDC(hwnd);
		DrawLine(hdc, p[0], p[1], p[2], p[3], color);
		ReleaseDC(hwnd, hdc);
	}

	if (idx % 2 == 0 && flag == true)
	{
		hdc = GetDC(hwnd);
		DrawLine(hdc, p[0], p[1], p[2], p[3], color);
		ReleaseDC(hwnd, hdc);
	}

	if (idx == 4)
		idx = 0;

}

inline void contCircleDraw(HWND hwnd, HDC hdc, LPARAM lp, void (*DrawCircle) (HDC, int, int, int, COLORREF), COLORREF color)
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
		DrawCircle(hdc, p[0][0], p[0][1], p[1][0], color);
		ReleaseDC(hwnd, hdc);
		index = 0;
	}
	else index++;
}

inline void contEllipseDraw(HWND hwnd, HDC hdc, LPARAM lp, COLORREF color, void (*DrawEllipse) (HDC, int, int, double, double, COLORREF))
{
	static Vector p[3];
	static int index = 0;

	int x;
	int y;

	x = LOWORD(lp);
	y = HIWORD(lp);
	p[index][0] = x;
	p[index][1] = y;

	if (index == 2)
	{
		hdc = GetDC(hwnd);
		double a = eclidDis(p[0], p[1]);
		double b = eclidDis(p[0], p[2]);

		DrawEllipse(hdc, p[0][0], p[0][1], a, b, color);
		ReleaseDC(hwnd, hdc);
		index = 0;
	}
	else
	{
		index++;
	}
}
inline void contSpline(HWND hwnd, HDC hdc, LPARAM lp, COLORREF color)
{
	const int maxPoints = 7;
	static Vector p[maxPoints];
	static int index = 0;

	int x;
	int y;

	x = LOWORD(lp);
	y = HIWORD(lp);
	p[index][0] = x;
	p[index][1] = y;

	if (index == maxPoints - 1)
	{
		hdc = GetDC(hwnd);
		DrawCardinalSpline(hdc, p, maxPoints, 0.5, color);
		ReleaseDC(hwnd, hdc);
		index = 0;
	}
	else
	{
		index++;
	}
}

