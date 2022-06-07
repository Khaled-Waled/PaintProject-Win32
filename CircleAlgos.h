#include "LineAlgos.h"
#include "Header.h"
#include <math.h>
#include <corecrt_math_defines.h>

inline void DrawDiameter(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	DrawLine4(hdc, (xc + x), (yc - y), (xc - x), (yc + y), color);
}
inline void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	savePixel(xc + x, yc + y, color);

	SetPixel(hdc, xc - x, yc + y, color);
	savePixel(xc - x, yc + y, color);

	SetPixel(hdc, xc - x, yc - y, color);
	savePixel(xc - x, yc - y, color);

	SetPixel(hdc, xc + x, yc - y, color);
	savePixel(xc + x, yc - y, color);

	SetPixel(hdc, xc + y, yc + x, color);
	savePixel(xc + y, yc + x, color);

	SetPixel(hdc, xc - y, yc + x, color);
	savePixel(xc - y, yc + x, color);

	SetPixel(hdc, xc - y, yc - x, color);
	savePixel(xc - y, yc - x, color);

	SetPixel(hdc, xc + y, yc - x, color);
	savePixel(xc + y, yc - x, color);

}
inline void Draw4Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color)
{
	SetPixel(hdc, xc + x, yc + y, color);
	savePixel(xc+x, yc+y, color);
	SetPixel(hdc, xc - x, yc + y, color);
	savePixel(xc - x, yc + y, color);
	SetPixel(hdc, xc - x, yc - y, color);
	savePixel(xc - x, yc - y, color);
	SetPixel(hdc, xc + x, yc - y, color);
	savePixel(xc + x, yc - y, color);
}


inline void DrawCircleCartesian(HDC hdc, int xc, int yc, int R, COLORREF color)	//Direct Cartesian method
{
	OutputDebugStringA("Cartesian Circle");
	int x = 0, y = R;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
	while (x < y)
	{
		x++;
		y = Round(sqrt((R * R) - (x * x)));
		Draw8Points(hdc, xc, yc, x, y, color);
	}
}
inline void DrawCircleDPolar(HDC hdc, int xc, int yc, int R, COLORREF color) //Direct Polar method
{
	OutputDebugStringA("Direct Polar Cirlce");

	int x = R, y = 0;
	double t = 0;
	double dt = 1.0 / R;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
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
	OutputDebugStringA("Iterative Polar");

	double x = R, y = 0;
	double dt = 1.0 / R;
	double cdt = cos(dt);
	double sdt = sin(dt);
	Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
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
	OutputDebugStringA("Midpoint cirlce");

	double d = 1.0 - R;
	int x = -1 * R, y = 0;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
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
	Draw8Points(hdc, xc, yc, Round(x), Round(y), color);
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
inline void DrawEllipseDirect(HDC hdc, int xc, int yc, double A, double B, COLORREF color)
{
	OutputDebugStringA("Ellipse Direct");

	double x = 0, y = A;
	while (y >= 0)
	{
		x++;
		y = sqrt(B * B * (1 - ((x * x) / (A * A))));
		Draw4Points(hdc, xc, yc, round(x), round(y), color);
	}

	x = B, y = 0;
	while (x >= 0)
	{
		y++;
		x = sqrt(A * A * (1 - ((y * y) / (B * B))));
		Draw4Points(hdc, xc, yc, round(x), round(y), color);
	}
}

inline void DrawEllipseIterative(HDC hdc, int xc, int yc, double A, double B, COLORREF color)
{
	OutputDebugStringA("Ellipse Iterative");

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

inline void DrawEllipseMidPoint(HDC hdc, int xc, int yc, double rx, double ry, COLORREF color)
{
	OutputDebugStringA("Ellipse Midpoint");

	double dx, dy, d1, d2, x, y;
	x = 0.0;
	y = ry;

	//d of first region
	d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
	dx = 2 * ry * ry * x;
	dy = 2 * rx * rx * y;

	//first region
	while (dx < dy)
	{

		Draw4Points(hdc, xc, yc, x, y, color);
		if (d1 < 0)
		{
			x++;
			dx = dx + (2 * ry * ry);
			d1 = d1 + dx + (ry * ry);
		}
		else
		{
			x++;
			y--;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d1 = d1 + dx - dy + (ry * ry);
		}
	}

	//d of sec region
	d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);

	//Plotting points of second region
	while (y >= 0)
	{

		Draw4Points(hdc, xc, yc, x, y, color);

		if (d2 > 0)
		{
			y--;
			dy = dy - (2 * rx * rx);
			d2 = d2 + (rx * rx) - dy;
		}
		else
		{
			y--;
			x++;
			dx = dx + (2 * ry * ry);
			dy = dy - (2 * rx * rx);
			d2 = d2 + dx - dy + (rx * rx);
		}
	}
}