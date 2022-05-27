#pragma once
#include <Windows.h>
struct Vector
{
	int v[2];
	Vector(int x = 0, int y = 0)
	{
		v[0] = x; v[1] = y;
	}
	int& operator[](int i) {
		return v[i];
	}
};

inline int Round(double x)
{
	return (int)(x + 0.5);
}
inline void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}