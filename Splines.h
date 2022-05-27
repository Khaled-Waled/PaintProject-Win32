#pragma once
#include "Header.h"
void DrawHermiteCurve(HDC hdc, Vector p1, Vector T1, Vector p2, Vector T2, COLORREF color)
{
    double a0 = p1[0], a1 = T1[0],
        a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
        a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
    double b0 = p1[1], b1 = T1[1],
        b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
        b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), color);
    }
}

void DrawCardinalSpline(HDC hdc, Vector P[], int n, double c, COLORREF color)
{
    double c1 = c / 2;
    Vector* T = new Vector[n];
    for (int i = 1; i < n - 1; i++) T[i] = (P[i + 1] - P[i - 1]), T[i][0] *= c1, T[i][1] *= c1;
    T[0] = P[1] - P[0];
    T[0][0] *= c1, T[0][1] *= c1;
    T[n - 1] = P[n - 1] - P[n - 2];
    T[n - 1][0] *= c1, T[n - 1][1] *= c1;
    for (int i = 0; i < n - 1; i++) DrawHermiteCurve(hdc, P[i], T[i], P[i + 1], T[i + 1], color);
    delete[] T;
}