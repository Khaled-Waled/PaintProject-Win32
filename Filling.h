#pragma once
#include "Header.h"
#include "LineAlgos.h"

void DrawHermiteCurve2(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c)
{
	double a0 = p1[0], a1 = T1[0];
	double b0 = p1[1], b1 = T1[1];
	for (double t = 0; t <= 1; t += 0.0001)
	{
		double x = a0 + a1 * t;
		double y = b0 + b1 * t;
		if (y >= p2[1])
		{
			break;
		}
		SetPixel(hdc, Round(x), Round(y), c);
	}
}

void MyFloodHermite(HDC hdc, double x0, double y0, double x1, double y1, double x2, double y2, COLORREF borderColor, COLORREF fillColor)
{
	COLORREF tmpColor = GetPixel(hdc, x0, y0);
	static Vector points[4];

	//Points[0][0] x1, Points[0][1] y1, Points[1][0] x2, Points[1][1] y2, Points[2][0] x3, Points[2][1] y3, Points[3][0] x4, Points[3][1] y4
	points[0][0] = x0;
	points[0][1] = y1;
	points[1][0] = x0;
	points[1][1] = y2;

	double tmp = max(y2, y1);
	points[2][0] = x1 + 1;
	points[2][1] = tmp - 1;

	tmp = min(y2, y1);
	points[3][0] = x1 - 1;
	points[3][1] = tmp + 1;
	Vector T1(3 * (points[1][0] - points[0][0]), 3 * (points[1][1] - points[0][1]));
	Vector T2(3 * (points[3][0] - points[2][0]), 3 * (points[3][1] - points[2][1]));

	if (tmpColor == borderColor || tmpColor == fillColor) return;


	DrawHermiteCurve2(hdc, points[0], T1, points[1], T2, fillColor);
	MyFloodHermite(hdc, x0 + 1, y0, x0 + 1, y1, x0 + 1, y2, borderColor, fillColor);
	MyFloodHermite(hdc, x0 - 1, y0, x0 - 1, y1, x0 - 1, y2, borderColor, fillColor);
}

void DrawSquareRectangle(HDC hdc, double x1, double y1, double x3, double y3, COLORREF color)
{
	DrawLine1(hdc, x1, y1, x3, y1, color);
	DrawLine1(hdc, x3, y1, x3, y3, color);
	DrawLine1(hdc, x1, y1, x1, y3, color);
	DrawLine1(hdc, x3, y3, x1, y3, color);
}


void bezierCurve(HDC hdc, double x1, double y1, double x2, double y2, COLORREF color)
{
	double a0 = x1;
	double a1 = -3 * x1 + 3 * x2;


	double b0 = y1;
	double b1 = -3 * y1 + 3 * y2;

	for (double t = 0; t <= 1; t += 0.001)
	{
		double x = a0 + (a1 * t);
		double y = b0 + (b1 * t);
		if (x >= x2)
		{
			break;
		}
		SetPixel(hdc, Round(x), Round(y), color);

	}
}


void MyFloodBezier(HDC hdc, double x0, double y0, double x1, double y1, double x2, double y2, COLORREF borderColor, COLORREF fillColor)
{
	COLORREF tmpColor = GetPixel(hdc, x0, y0);
	static Vector points[4];
	points[0][0] = x1;
	points[0][1] = y0;
	points[1][0] = x2;
	points[1][1] = y0;
	if (tmpColor == borderColor || tmpColor == fillColor) return;
	bezierCurve(hdc, points[0][0], points[0][1], points[1][0], points[1][1], fillColor);
	MyFloodHermite(hdc, x0, y0 + 1, x1, y0 + 1, x2, y0 + 1, borderColor, fillColor);
	MyFloodHermite(hdc, x0, y0 - 1, x1, y0 - 1, x2, y0 - 1, borderColor, fillColor);
}


