#include <windows.h> 
#include <iostream>
#include "RunnerFunctions.h"
#include "Header.h"
#include "LineAlgos.h"
#include <stdio.h>
#define LOAD_ID 0x2
#define Clear_ID 0x6
#define Red_B_ID 0x3
#define Grn_B_ID 0x4
#define Blu_B_ID 0x5
#include "Clipping.h"
#include "Filling.h"
#include <fstream>
#include <string>
#include "Filling2.h"


using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
int command = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HIGHT = 720;
const COLORREF AllColors[3] = { RGB(200,50,50), RGB(0,100,0), RGB(0,0,200) };
COLORREF workingColor = AllColors[0];
static string a;
static string b;
static char c;
static char d;
static char e;


//Prototypes
void saveScreen(HDC);
void loadScreen(HDC);
void clearScreen(HDC, HWND);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wc = { 0 };
	wc.lpszClassName = TEXT("Application");
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_CROSS);
	g_hinst = hInstance;
	RegisterClass(&wc);
	hwnd = CreateWindow(wc.lpszClassName, TEXT("Choose option"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, SCREEN_WIDTH, SCREEN_HIGHT, 0, 0, hInstance, 0);
	while (GetMessage(&msg, NULL, 0, 0))

	{
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);
	static HWND hwndCombo;
	static HWND hwndButtonLoad;
	static HWND clearButton;
	static HWND hwnd_SL_Title;
	static HWND hwndColorTile;
	static HWND hwndButtonRed;
	static HWND hwndButtonGreen;
	static HWND hwndButtonBlue;


	const TCHAR* items[] =
	{
		TEXT("0- Change the background of window to be white"),
		TEXT("1- DDA Line Algorithm"),
		TEXT("2- Midpoint Line Algorithm"),
		TEXT("3- Parametric Line Algorithm"),
		TEXT("4- Direct Circle Algorithm"),
		TEXT("5- Polar Circle Algorithm"),
		TEXT("6- iterative Polar Circle Algorithm"),
		TEXT("7- Midpoint Circle Algorithm"),
		TEXT("8- Midpoint Circle Algorithm (Modified)"),
		TEXT("9- Fill Circle (lines)"),
		TEXT("10- Fill Circle (circles)"),
		TEXT("11- Fill Square (Vertical)"),
		TEXT("12- Fill Rectangle (horizontal)"),
		TEXT("13- Convex Polygon Fill"),
		TEXT("14- NON-Convex Polygon Fill"),
		TEXT("15- Recursive Flood Fill"),
		TEXT("16- NON-Recursive Flood Fill"),
		TEXT("17- Cardinal Spline Curve"),
		TEXT("18- Direct Ellipse Drawing"),
		TEXT("19- Polar Ellipse Drawing"),
		TEXT("20- Midpoint Ellipse Drawing"),
		TEXT("21- Rectangle Clipping (point)"),
		TEXT("22- Rectangle Clipping (Line)"),
		TEXT("23- Rectangle Clipping (Polygon)"),
		TEXT("24- Square Clipping (point)"),
		TEXT("25- Square Clipping (Line)"),
		TEXT("26- Circle Clipping (point)"),
		TEXT("27- Circle Clipping (Line)"),
		TEXT("28- Draw Square / Rectangle"),
	};
	switch (msg)
	{
	case WM_CREATE:
		//create combo box
		hwndCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 10, 10, 350, 110, hwnd, NULL, g_hinst, NULL);
		//populate combo box
		for (int i = 0; i < 29; i++)
			SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);


		//Create save-load title
		hwnd_SL_Title = CreateWindow(
			TEXT("static"),
			TEXT("Load"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			SCREEN_WIDTH * 0.9,	// x position
			10,					// y position
			120,				// Width
			20,					// Height
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

		//Create Save button
		


		//Create Load button
		hwndButtonLoad = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Load"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH * 0.9,	// x position 
			60,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU)LOAD_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);


		hwndButtonLoad = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Clear"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH * 0.9,	// x position 
			30,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU)Clear_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);

		//Create color title
		hwndColorTile = CreateWindow(
			TEXT("static"),
			TEXT("Choose color"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			SCREEN_WIDTH * 0.9,	// x position
			150,				// y position
			120,				// Width
			20,					// Height
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

		// Button to Set Color to red
		hwndButtonRed = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Red"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH * 0.9,	// x position 
			170,				// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU)Red_B_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);


		// Button to Set Color to Green
		hwndButtonGreen = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Green"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH * 0.9,	// x position 
			200,				// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU)Grn_B_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);


		// Button to Set Color to Green
		hwndButtonBlue = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Blue"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH * 0.9,	// x position 
			230,				// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU)Blu_B_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);

		break;

	case WM_COMMAND:
		//respond to combo box selection
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			TCHAR strText[255] = TEXT("\0");
			//get position of selected item
			LRESULT sel = SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);
			//get selected item text
			SendMessage(hwndCombo, CB_GETLBTEXT, sel, (LPARAM)strText);

			if (sel == 0) //Change the background of window to be white

			{
				HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
				InvalidateRect(hwnd, NULL, TRUE);
			}
			command = sel;
			SetFocus(hwnd);
		}
		if ((HIWORD(wParam) == BN_CLICKED) && (lParam != 0))
		{
			switch (LOWORD(wParam))
			{
			
			case Clear_ID:
				clearScreen(hdc, hwnd);
				break;
			case LOAD_ID:
				loadScreen(hdc);
				break;

			case Red_B_ID:
				workingColor = AllColors[0];
				break;

			case Grn_B_ID:
				workingColor = AllColors[1];
				break;

			case Blu_B_ID:
				workingColor = AllColors[2];
				break;

			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (command == 1) //DDA Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine3, workingColor);

		}
		else if (command == 2)	//Midpoint Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine4, workingColor);
		}
		else if (command == 3) //Parametric Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine1, workingColor);
		}
		else if (command == 4) //Direct Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleCartesian, workingColor);
		}
		else if (command == 5) //Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleDPolar, workingColor);
		}
		else if (command == 6) //iterative Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleIPolar, workingColor);
		}
		else if (command == 7) //Midpoint Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMidpoint, workingColor);
		}
		else if (command == 8) //Midpoint Circle Algorithm (Modified)
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMMidpoint, workingColor);
		}
		else if (command == 9) //Fill Circle (lines)
		{
			static double xc, yc, r, rx, ry, qx, qy;
			static int counter = 0;
			if (counter == 0)
			{
				xc = LOWORD(lParam);
				yc = HIWORD(lParam);
				counter++;
			}
			else if (counter == 1)
			{
				rx = LOWORD(lParam);
				ry = HIWORD(lParam);
				r = sqrt((abs(rx - xc) * abs(rx - xc)) + (abs(ry - yc) * abs(ry - yc)));
				DrawCircleCartesian(hdc, rx, ry, r, RGB(255, 0, 0));
				counter++;
			}
			else if (counter == 2)
			{
				qx = LOWORD(lParam);
				qy = HIWORD(lParam);
				if (qx < rx)
				{
					if (qy < ry)
					{
						fillingCircleWithLines(hdc, rx, ry, r, RGB(0, 255, 0), 4);
					}
					else
					{
						fillingCircleWithLines(hdc, rx, ry, r, RGB(0, 255, 0), 3);
					}
				}
				else
				{
					if (qy < ry)
					{
						fillingCircleWithLines(hdc, rx, ry, r, RGB(0, 255, 0), 1);
					}
					else
					{
						fillingCircleWithLines(hdc, rx, ry, r, RGB(0, 255, 0), 2);
					}
				}
				counter = 0;

			}
			
		}
		else if (command == 10) //Fill Circle (circles)
		{
			static double xc, yc, r, rx, ry, qx, qy;
			static int counter = 0;
			if (counter == 0)
			{
				xc = LOWORD(lParam);
				yc = HIWORD(lParam);
				counter++;
			}
			else if (counter == 1)
			{
				rx = LOWORD(lParam);
				ry = HIWORD(lParam);
				r = sqrt((abs(rx - xc) * abs(rx - xc)) + (abs(ry - yc) * abs(ry - yc)));
				DrawCircleCartesian(hdc, rx, ry, r, RGB(255, 0, 0));
				counter++;
			}
			else if (counter == 2)
			{
				qx = LOWORD(lParam);
				qy = HIWORD(lParam);
				if (qx < rx)
				{
					if (qy < ry)
					{
						fillingCircleWithCircles(hdc, rx, ry, r, RGB(0, 255, 0), 4);
					}
					else
					{
						fillingCircleWithCircles(hdc, rx, ry, r, RGB(0, 255, 0), 3);
					}
				}
				else
				{
					if (qy < ry)
					{
						fillingCircleWithCircles(hdc, rx, ry, r, RGB(0, 255, 0), 1);
					}
					else
					{
						fillingCircleWithCircles(hdc, rx, ry, r, RGB(0, 255, 0), 2);
					}
				}
				counter = 0;
			}
			
		}
		else if (command == 11) //Fill Square (Vertical)
		{
			static double hermitePoints[4];
			static int hermiteCounter = 0;
			if (hermiteCounter == 4)
			{
				hermiteCounter++;
				MyFloodHermite(hdc, LOWORD(lParam), HIWORD(lParam), hermitePoints[0], hermitePoints[1], hermitePoints[2], hermitePoints[3], AllColors[0], workingColor);
				hermiteCounter = 0;
			}
			else
			{
				hermitePoints[hermiteCounter] = LOWORD(lParam);
				hermitePoints[hermiteCounter + 1] = HIWORD(lParam);
				hermiteCounter += 2;
			}
		}
		else if (command == 12) //Fill Rectangle (horizontal)
		{
			static double hermitePoints[4];
			static int hermiteCounter = 0;
			if (hermiteCounter == 4)
			{
				hermiteCounter++;
				MyFloodBezier(hdc, LOWORD(lParam), HIWORD(lParam), hermitePoints[0], hermitePoints[1], hermitePoints[2], hermitePoints[3], AllColors[0], workingColor);
				hermiteCounter = 0;
			}
			else
			{
				hermitePoints[hermiteCounter] = LOWORD(lParam);
				hermitePoints[hermiteCounter + 1] = HIWORD(lParam);
				hermiteCounter += 2;
			}

		}
		else if (command == 13)
		{
			static int x, y;
			static vector<Vector>arr(5);
			static int counter = 0;

			if (counter < 5)
			{
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				arr[counter][0] = x;
				arr[counter][1] = y;
				counter++;
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					if (i == 4)
						DrawLine1(hdc, arr[i][0], arr[i][1], arr[0][0], arr[0][1], RGB(255, 0, 0));
					else
						DrawLine1(hdc, arr[i][0], arr[i][1], arr[i + 1][0], arr[i + 1][1], RGB(255, 0, 0));
				}
				ConvexFill(hdc, arr, 5, RGB(0, 0, 255));
				counter = 0;
			}
		}
		else if (command == 14)
		{
			static int x, y;
			static vector<point>arr(5);
			static int counter = 0;

			if (counter < 5)
			{
				x = LOWORD(lParam);
				y = HIWORD(lParam);
				arr[counter].x = x;
				arr[counter].y = y;
				counter++;
			}
			else
			{
				for (int i = 0; i < 5; i++)
				{
					if (i == 4)
						DrawLine1(hdc, arr[i].x, arr[i].y, arr[0].x, arr[0].y, RGB(255, 0, 0));
					else
						DrawLine1(hdc, arr[i].x, arr[i].y, arr[i + 1].x, arr[i + 1].y, RGB(255, 0, 0));
				}
				GeneralPolygonFill(hdc, arr, 5, RGB(0, 0, 255));
				counter = 0;
			}
		}
		
		else if (command == 15) //Recursive Flood Fill
		{
			OutputDebugStringA("Flood");

			static int x, y;

			x = LOWORD(lParam);
			y = HIWORD(lParam);
			myFloodFillNonRec(hdc, x, y, RGB(200, 50, 50), RGB(0, 0, 255));
				

		}
		else if (command == 16) //NON-Recursive Flood Fill
		{
			static int x, y;

			x = LOWORD(lParam);
			y = HIWORD(lParam);
			myFloodFillNonRec(hdc, x, y, RGB(200, 50, 50), RGB(0, 0, 255));
			
		}
		else if (command == 17) //Cardinal Spline Curve
		{
			contSpline(hwnd, hdc, lParam, AllColors[2]);
		}
		else if (command == 18) //Direct Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, workingColor, DrawEllipseDirect);
		}
		else if (command == 19) //Polar Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, workingColor, DrawEllipseIterative);
		}
		else if (command == 20) //Midpoint Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, workingColor, DrawEllipseMidPoint);

		}
		else if (command == 21) //Rectangle Clipping (point)
		{
			hdc = GetDC(hwnd);
			static int counter = 0;
			static double points[8];
			if (counter == 6)
			{
				Rectangle(hdc, points[0], points[1], points[4], points[5]);
				counter += 2;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
			else
			{
				PointClipping(hdc, LOWORD(lParam), HIWORD(lParam), points[0], points[1], points[4], points[5], RGB(0, 0, 250));
			}

		}
		else if (command == 22) //Rectangle Clipping (Line)
		{
			hdc = GetDC(hwnd);
			static int counter = 0;
			static double points[8];
			static double line[4];
			static int lineCounter = 0;
			if (counter == 6)
			{
				Rectangle(hdc, points[0], points[1], points[4], points[5]);
				counter += 2;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
			else
			{
				if (lineCounter < 4);
				{
					line[lineCounter] = LOWORD(lParam);
					line[lineCounter + 1] = HIWORD(lParam);
					lineCounter += 2;
				}
				if (lineCounter == 4)
				{
					CohenSuth(hdc, line[0], line[1], line[2], line[3], points[0], points[1], points[4], points[5]);
					lineCounter = 0;
				}
			}
		}
		else if (command == 23) //Rectangle Clipping (Polygon)
		{
			hdc = GetDC(hwnd);
			static int counter = 0;
			static double points[8];
			static double line[4];
			static int lineCounter = 0;
			static int polygCounter = 0;
			static POINT polyg[5];
			if (counter == 6)
			{
				Rectangle(hdc, points[0], points[1], points[4], points[5]);
				counter += 2;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
			else
			{
				if (polygCounter < 5);
				{
					polyg[polygCounter].x = LOWORD(lParam);
					polyg[polygCounter].y = HIWORD(lParam);
					polygCounter += 1;
				}
				if (polygCounter == 5)
				{
					PolygonClip(hdc, polyg, 5, points[0], points[1], points[4], points[5]);
					polygCounter = 0;
				}
			}

		}
		else if (command == 24)
		{
			hdc = GetDC(hwnd);
			static int counter = 0;
			static double points[8];
			if (counter == 6)
			{
				Rectangle(hdc, points[0], points[1], points[4], points[5]);
				counter += 2;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
			else
			{
				PointClipping(hdc, LOWORD(lParam), HIWORD(lParam), points[0], points[1], points[4], points[5], RGB(0, 0, 250));
			}

		}
		else if (command == 25)
		{

			hdc = GetDC(hwnd);
			static int counter = 0;
			static double points[8];
			static double line[4];
			static int lineCounter = 0;
			if (counter == 6)
			{
				Rectangle(hdc, points[0], points[1], points[4], points[5]);
				counter += 2;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
			else
			{
				if (lineCounter < 4);
				{
					line[lineCounter] = LOWORD(lParam);
					line[lineCounter + 1] = HIWORD(lParam);
					lineCounter += 2;
				}
				if (lineCounter == 4)
				{
					CohenSuth(hdc, line[0], line[1], line[2], line[3], points[0], points[1], points[4], points[5]);
					lineCounter = 0;
				}
			}
		}
		else if (command == 26) //Circle Clipping (point)
		{
			static double Xc, Yc, pX, PY;
			static double pointX, pointY;
			static int circleCounter = 0;
			static double R;
			if (circleCounter == 0)
			{
				Xc = LOWORD(lParam);
				Yc = HIWORD(lParam);
				circleCounter++;
			}
			else if (circleCounter == 1)
			{
				R = sqrt(((Xc - LOWORD(lParam)) * (Xc - LOWORD(lParam))) + ((Yc - HIWORD(lParam)) * (Yc - HIWORD(lParam))));
				DrawCircleCartesian(hdc, Xc, Yc, R, RGB(0, 0, 250));
				circleCounter++;
			}
			else
			{
				PointClipping(hdc, LOWORD(lParam), HIWORD(lParam), Xc, Yc, R);
			}

		}
		else if (command == 27) //Circle Clipping (Line)
		{
			static double Xc, Yc, pX, PY;
			static double pointX, pointY;
			static int circleCounter = 0;
			static double lx, ly;
			static int lineCounter;
			static double R;
			if (circleCounter == 0)
			{
				Xc = LOWORD(lParam);
				Yc = HIWORD(lParam);
				circleCounter++;
			}
			else if (circleCounter == 1)
			{
				R = sqrt(((Xc - LOWORD(lParam)) * (Xc - LOWORD(lParam))) + ((Yc - HIWORD(lParam)) * (Yc - HIWORD(lParam))));
				DrawCircleCartesian(hdc, Xc, Yc, R, RGB(0, 0, 250));
				circleCounter++;
			}
			else
			{
				if (lineCounter == 0)
				{
					lx = LOWORD(lParam);
					ly = HIWORD(lParam);
					lineCounter++;
				}
				else
				{
					lineClipping(hdc, lx, ly, LOWORD(lParam), HIWORD(lParam), Xc, Yc, R, RGB(0, 0, 250));
					lineCounter = 0;
				}
			}
		}
		else if (command == 28)
		{
			static int counter = 0;
			static int hermiteCounter = 0;
			static double hermitePoints[4];
			static double points[8];
			if (counter == 6)
			{
				DrawSquareRectangle(hdc, points[0], points[1], points[4], points[5], AllColors[0]);
				counter = 0;
			}
			else if (counter < 6)
			{
				points[counter] = LOWORD(lParam);
				points[counter + 1] = HIWORD(lParam);
				counter += 2;
			}
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void loadScreen(HDC hdc)
{

	ifstream infile("./out.txt");
	while (infile >> a >> b >> c >> d >> e)
	{
		SetPixel(hdc, stoi(a), stoi(b), RGB(c, d, e));
	}
}


void saveScreen(HDC hdc)
{
	OutputDebugStringA("Saved");
}

void clearScreen(HDC hdc, HWND hwnd)
{
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
	InvalidateRect(hwnd, NULL, TRUE);
	FILE* p = fopen("out.txt", "w");
	fclose(p);
}