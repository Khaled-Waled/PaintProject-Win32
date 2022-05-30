#include "LineAlgos.h"
#include "Header.h"
#include <math.h>
#include <corecrt_math_defines.h>

inline void DrawDiameter(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	DrawLine5(hdc, (xc + x), (yc - y), (xc - x), (yc + y), color);
}
inline void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	SetPixel(hdc, xc + y, yc + x, color);
	SetPixel(hdc, xc - y, yc + x, color);
	SetPixel(hdc, xc - y, yc - x, color);
	SetPixel(hdc, xc + y, yc - x, color);
}
inline void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	SetPixel(hdc, xc + x, yc - y, color);
}


inline void DrawCircleCartesian(HDC hdc, int xc, int yc, int R, COLORREF color)	//Direct Cartesian method
{

	int x = 0, y = R;
	while (x < y)
	{
		x++;
		y = Round(sqrt((R * R) - (x * x)));
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}
inline void DrawCircleDPolar(HDC hdc, int xc, int yc, int R, COLORREF color) //Direct Polar method
{
	int x = R, y = 0;
	double t = 0;
	double dt = 1.0 / R;
	while (x > y)
	{
		t += dt;
		x = Round(R * cos(t));
		y = Round(R * sin(t));
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}
inline void DrawCircleIPolar(HDC hdc, int xc, int yc, int R, COLORREF color) //Iterative Polar
{
	double x = R, y = 0;
	double dt = 1.0 / R;
	double cdt = cos(dt);
	double sdt = sin(dt);

	while (x > y)
	{
		double x1 = x * cdt - y * sdt;
		y = x * sdt + y * cdt;
		x = x1;
		Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
	}
}
inline void DrawCircleMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	double d = 1.0 - R;
	int x = -1 * R, y = 0;
	while (-x > y)
	{
		if (d > 0)
		{
			d += (2.0 * x) + (2.0 * y) + 5.0;
			x++;

		}
		else
		{
			d += (2.0 * y) + 2.0;
		}
		y++;

		Draw8Points(hdc, xc, yc, x, y, color);
	}

}
inline void DrawCircleMMidpoint(HDC hdc, int xc, int yc, int R, COLORREF color)
{
	double d = 1.0 - R;
	double d1 = -2.0 * R - 1.0;
	double d2 = 2.0;
	double x = -1 * R, y = 0;
	while (x < y)
	{
		if (d > 0)
		{
			d += d1;
			d1 += 4;
			d2 += 2;
			x++;

		}
		else
		{
			d += d2;
			d1 += 2;
			d2 += 2;
		}
		y--;

		Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
	}
}
inline void DrawEllipseIterative(HDC hdc, int xc, int yc, double A, double B, COLORREF color)
{
	//Ellipse iterative polar
	// 1st Quad
	double x = A, y = 0;
	double dt = 1.0 / max(A, B);
	double cdt = cos(dt);
	double sdt = sin(dt);

	while (x > 0)
	{
		double x1 = x * cdt - (A / B) * y * sdt;
		y = y * cdt + (B / A) * x * sdt;
		x = x1;
		Draw4Points(hdc, xc, yc, Round(x), Round(y), color);
	}
}