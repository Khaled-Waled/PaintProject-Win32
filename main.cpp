#include <windows.h> 
#include <iostream>
#include "RunnerFunctions.h"
#include "Header.h"
#include "LineAlgos.h"
#include <stdio.h>
#define SAVE_ID 0x1
#define LOAD_ID 0x2
#define Red_B_ID 0x3
#define Grn_B_ID 0x4
#define Blu_B_ID 0x5
#include "Clipping.h"
#include "Filling.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
int command = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HIGHT = 720;
const COLORREF AllColors[3] = { RGB(200,0,0), RGB(0,100,0), RGB(0,0,200) };
COLORREF workingColor = AllColors[0];

//Prototypes
void saveScreen(HDC);
void loadScreen(HDC);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wc = { 0 };
	wc.lpszClassName = TEXT("Application");
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
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
	static HWND hwndButtonSave;
	static HWND hwndButtonLoad;
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
			TEXT("Save and load"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			SCREEN_WIDTH * 0.9,	// x position
			10,					// y position
			120,				// Width
			20,					// Height
			hwnd, NULL, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), NULL);

		//Create Save button
		hwndButtonSave = CreateWindow(
			TEXT("BUTTON"),		
			TEXT("Save"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH*0.9,	// x position 
			30,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU) SAVE_ID,	// ID
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);				


		//Create Load button
		hwndButtonLoad = CreateWindow(
			TEXT("BUTTON"),		
			TEXT("Load"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			SCREEN_WIDTH*0.9,	// x position 
			60,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU) LOAD_ID,	// ID
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
				case SAVE_ID:
					saveScreen(hdc);
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
			contLineDraw(hwnd, hdc, lParam, DrawLine5, workingColor);
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
			//TODO
		}
		else if (command == 10) //Fill Circle (circles)
		{
			//TODO
		}
		else if (command == 11) //Fill Square (Vertical)
		{
			static double hermitePoints[4];
			static int hermiteCounter = 0;
			if (hermiteCounter == 4)
			{
				hermiteCounter++;
				MyFloodHermite(hdc, LOWORD(lParam), HIWORD(lParam), hermitePoints[0], hermitePoints[1], hermitePoints[2], hermitePoints[3], RGB(250, 0, 0), RGB(250, 250, 250));
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

		}
		else if (command == 13) //Convex Polygon Fill
		{

		}
		else if (command == 14) //NON-Convex Polygon Fill
		{

		}
		else if (command == 15) //Recursive Flood Fill
		{

		}
		else if (command == 16) //NON-Recursive Flood Fill
		{

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
				PointClipping(hdc, LOWORD(lParam), HIWORD(lParam), points[0], points[1], points[4], points[5], RGB(0,0,250));
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
					line[lineCounter+1] = HIWORD(lParam);
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
				DrawCircleCartesian(hdc, Xc, Yc, R, RGB(0,0,250));
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
					lineClipping(hdc, lx, ly, LOWORD(lParam), HIWORD(lParam), Xc, Yc, R, RGB(0,0,250));
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
				DrawSquareRectangle(hdc, points[0], points[1], points[4], points[5], RGB(250, 0, 0));
				counter += 2;
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
	if (!(freopen("./output.txt", "r", stdin) == NULL))
	{
		int lines;
		cin >> lines;

		while (lines--)
		{
			//Read Data
			int w ,h ,r, g,b;
			cin >> w >> h >> r >> g >> b;

			//Apply data
			SetPixel(hdc, w, h, RGB(r, g, b));
		}
	}

}
void loadScreenold(HDC hdc)
{
	FILE* f = freopen("./output.txt", "r", stdin);
	char line[100];

	//Read while there exits new lines
	while(fgets(line, sizeof(line), f) != NULL)
	{
		//generate 5 tokens
		const char* tok1 = strtok(line, " ");
		const char* tok2 = strtok(line, " ");
		const char* tok3 = strtok(line, " ");
		const char* tok4 = strtok(line, " ");
		const char* tok5 = strtok(NULL, " ");

		//Parse tokens
		int w = atoi(tok1);
		int h = atoi(tok2);
		int r = atoi(tok3);
		int g = atoi(tok4);
		int b = atoi(tok5);

		//Apply data
		SetPixel(hdc, w, h, RGB(r,g,b));
	}


}


void saveScreen(HDC hdc)
{
	if (!(freopen("./output.txt", "w", stdout) == NULL))
	{
		int lines = 0;
		for (int w = 0; w < SCREEN_WIDTH; w++)
		{
			for (int h = 0; h < SCREEN_HIGHT; h++)
			{
				COLORREF color = GetPixel(hdc, w, h);
				if (color == AllColors[0] || color == AllColors[1]  || color == AllColors[2])
				{
					lines++;
				}
			}
		}
		cout << lines << endl;
		for (int w = 0; w < SCREEN_WIDTH; w++)
		{
			for (int h = 0; h < SCREEN_HIGHT; h++)
			{
				COLORREF color = GetPixel(hdc, w, h);
				if (color == AllColors[0])
				{
					cout << w << " " << h << " " << 200 << " "<< 0 <<" "<< 0 <<endl;
				}
				else if (color == AllColors[1])
				{
					cout << w << " " << h << " " << 0 << " " << 100 << " " << 0 << endl;
				}
				else if (color == AllColors[2])
				{
					cout << w << " " << h << " " << 0 << " " << 0 << " " << 200 << endl; 
				}
			}
		}
	}
}