#pragma once
#include "Header.h"
inline void DrawLine1(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) //Parametric
{
	OutputDebugStringA("Parametric Line");

	int dx = xe - xs;
	int dy = ye - ys;
	if (abs(dy) <= abs(dx))
	{
		double slope = (double)dy / dx;
		if (xs > xe)
		{
			swap(xs, ys, xe,ye);
		}
		for (int x = xs; x <= xe; x++)
		{
			int y = round(ys + (x - xs) * slope);
			SetPixel(hdc, x, y, color);
			savePixel(x, y, color);
		}
	}
	else
	{
		double islope = (double)dx / dy;
		if (ys > ye)
		{
			swap(xs, ys,xe,ye);
		}
		for (int y = ys; y <= ye; y++)
		{
			int x = round(xs + (y - ys) * islope);
			SetPixel(hdc, x, y, color);
			savePixel(x, y, color);
		}
	}
}
inline void DrawLine2(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)	//DDA
{
	OutputDebugStringA("DDA Line");

	int dx = x2 - x1;
	int dy = y2 - y1;


	SetPixel(hdc, x1, y1, c);

	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)
			swap(x1, y1, x2, y2);

		int x = x1;
		while (x <= x2)
		{
			x++;

			double y = y1 + (double)(x - x1) * dy / dx;
			SetPixel(hdc, x, Round(y), c);
			savePixel(x, y, c);
		}

	}
	else
	{
		if (y1 > y2)
			swap(x1, y1, x2, y2);

		int y = y1;
		while (y <= y2)
		{
			y++;
			double x = x1 + (double)(y - y1) * dx / dy;
			SetPixel(hdc, Round(x), y, c);
			savePixel(x, y, c);
		}

	}
}
inline void DrawLine3(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)	//DDA 2
{
	OutputDebugStringA("Line DDA2");

	int dx = x2 - x1;
	int dy = y2 - y1;


	SetPixel(hdc, x1, y1, c);
	savePixel(x1, y1, c);

	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)
			swap(x1, y1, x2, y2);

		double slope = (double)dy / dx;

		int x = x1;
		double y = y1;
		while (x <= x2)
		{
			x++;
			y += slope;
			SetPixel(hdc, x, Round(y), c);
			savePixel(x, Round(y), c);
		}

	}
	else
	{
		if (y1 > y2)
			swap(x1, y1, x2, y2);

		double invSlope = (double)dx / dy;

		int y = y1;
		double x = x1;
		while (y <= y2)
		{
			y++;
			x += invSlope;
			SetPixel(hdc, Round(x), y, c);
			savePixel(Round(x), y, c);
		}

	}
}
inline void DrawLine4(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)	//Midpoint
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	OutputDebugStringA("Line MidPoint");



	SetPixel(hdc, x1, y1, c);
	savePixel(x1, y1, c);


	if (abs(dy) <= abs(dx))
	{
		if (x1 > x2)
			swap(x1, y1, x2, y2);
		int x = x1;
		int y = y1;
		dx = x2 - x1;
		dy = y2 - y1;

		if (dy > 0)
		{
			while (x < x2)
			{
				double d = (y + 0.5 - y1) * dx - (x + 1.0 - x1) * dy;
				if (d < 0)
					y++;
				x++;
				SetPixel(hdc, x, y, c);
				savePixel(x, y, c);
			}
		}
		else
		{
			while (x < x2)
			{
				double d = (y - 0.5 - y1) * dx - (x + 1.0 - x1) * dy;
				if (d > 0)
					y--;
				x++;
				SetPixel(hdc, x, y, c);
				savePixel(x, y, c);
			}
		}
	}
	else
	{
		if (y1 > y2)
			swap(x1, y1, x2, y2);
		int x = x1;
		int y = y1;
		dx = x2 - x1;
		dy = y2 - y1;

		if (dx > 0)
		{
			while (y < y2)
			{
				double d = (y + 1.0 - y1) * dx - (x + 0.5 - x1) * dy;
				if (d > 0)
					x++;
				y++;
				SetPixel(hdc, x, y, c);
				savePixel(x, y, c);
			}
		}
		else
		{
			while (y < y2)
			{
				double d = (y + 1.0 - y1) * dx - (x - 0.5 - x1) * dy;
				if (d < 0)
					x--;
				y++;
				SetPixel(hdc, x, y, c);
				savePixel(x, y, c);
			}
		}
	}
}