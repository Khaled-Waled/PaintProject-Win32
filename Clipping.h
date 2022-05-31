#pragma once
#include <vector>
using namespace std;


void swap(double& x1, double& y1, double& x2, double& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}
struct Vertex
{
	double x, y;
	Vertex(int x1 = 0, int y1 = 0)
	{
		x = x1;
		y = y1;
	}
};
typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v, int edge);
typedef Vertex(*IntersectFunc)(Vertex& v1, Vertex& v2, int edge);

//Circle Clipping
void PointClipping(HDC hdc, double x, double y, double XC, double YC, double R)
{
	OutputDebugStringA("Circle Point Clipping");
	COLORREF outColor = RGB(255, 255, 255);
	COLORREF inColor = RGB(0, 0, 255);

	int newR = sqrt((x - XC) * (x - XC) + (y - YC) * (y - YC));

	if (newR < R)
		SetPixel(hdc, x, y, inColor);
}


void lineClipping(HDC hdc, double x1, double y1, double x2, double y2, double XC, double YC, double R, COLORREF c)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	OutputDebugStringA("Circle Line Clipping");
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
				PointClipping(hdc, x, y, XC, YC, R);
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
				PointClipping(hdc, x, y, XC, YC, R);

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
				PointClipping(hdc, x, y, XC, YC, R);

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
				PointClipping(hdc, x, y, XC, YC, R);
			}
		}
	}
}















//Square-Rectangle Clipping
void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom, COLORREF color)
{
	OutputDebugStringA("Square Point Clipping");

	if (x >= xleft && x <= xright && y >= ytop && y <= ybottom)
		SetPixel(hdc, x, y, color);
}

union OutCode
{
	unsigned All : 4;
	struct { unsigned left : 1, top : 1, right : 1, bottom : 1; };
};


OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
	OutCode out;
	out.All = 0;
	if (x < xleft)out.left = 1; else if (x > xright)out.right = 1;
	if (y < ytop)out.top = 1; else if (y > ybottom)out.bottom = 1;
	return out;
}

void VIntersect(double xs, double ys, double xe, double ye, int x, double* xi, double* yi)
{
	*xi = x;
	*yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}
void HIntersect(double xs, double ys, double xe, double ye, int y, double* xi, double* yi)
{
	*yi = y;
	*xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}

void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom)
{
	OutputDebugStringA("Square Line Clipping");

	double x1 = xs, y1 = ys, x2 = xe, y2 = ye;
	OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
	OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
	while ((out1.All || out2.All) && !(out1.All & out2.All))
	{
		double xi, yi;
		if (out1.All)
		{
			if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			x1 = xi;
			y1 = yi;
			out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
		}
		else
		{
			if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
			else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
			else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
			else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
			x2 = xi;
			y2 = yi;
			out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
		}
	}
	if (!out1.All && !out2.All)
	{
		MoveToEx(hdc, Round(x1), Round(y1), NULL);
		LineTo(hdc, Round(x2), Round(y2));
	}
}




VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect)
{
	VertexList OutList;
	Vertex v1 = p[p.size() - 1];
	bool v1_in = In(v1, edge);
	for (int i = 0; i < (int)p.size(); i++)
	{
		Vertex v2 = p[i];
		bool v2_in = In(v2, edge);
		if (!v1_in && v2_in)
		{
			OutList.push_back(Intersect(v1, v2, edge));
			OutList.push_back(v2);
		}
		else if (v1_in && v2_in) OutList.push_back(v2);
		else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
		v1 = v2;
		v1_in = v2_in;
	}
	return OutList;
}


bool InLeft(Vertex& v, int edge)
{
	return v.x >= edge;
}
bool InRight(Vertex& v, int edge)
{
	return v.x <= edge;
}
bool InTop(Vertex& v, int edge)
{
	return v.y >= edge;
}
bool InBottom(Vertex& v, int edge)
{
	return v.y <= edge;
}


Vertex VIntersect(Vertex& v1, Vertex& v2, int xedge)
{
	Vertex res;
	res.x = xedge;
	res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
	return res;
}
Vertex HIntersect(Vertex& v1, Vertex& v2, int yedge)
{
	Vertex res;
	res.y = yedge;
	res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
	return res;
}

void PolygonClip(HDC hdc, POINT* p, int n, int xleft, int ytop, int xright, int ybottom)
{
	OutputDebugStringA("Square PolygonClip");

	VertexList vlist;
	for (int i = 0; i < n; i++)vlist.push_back(Vertex(p[i].x, p[i].y));
	vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
	vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
	vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
	vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
	Vertex v1 = vlist[vlist.size() - 1];
	for (int i = 0; i < (int)vlist.size(); i++)
	{
		Vertex v2 = vlist[i];
		MoveToEx(hdc, Round(v1.x), Round(v1.y), NULL);
		LineTo(hdc, Round(v2.x), Round(v2.y));
		v1 = v2;
	}
}
