#include <windows.h> 
#include <iostream>
#include "RunnerFunctions.h"
#include "Header.h"
#include "LineAlgos.h"
#include <stdio.h>
#define SAVE_ID 0x1
#define LOAD_ID 0x2

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
int command = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HIGHT = 720;
const COLORREF AllColors[3] = { RGB(200,0,0), RGB(0,100,0), RGB(0,0,200) };

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
	};
	switch (msg)
	{
	case WM_CREATE:
		//create combo box
		hwndCombo = CreateWindow(TEXT("combobox"), NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 10, 10, 350, 110, hwnd, NULL, g_hinst, NULL);
		//populate combo box
		for (int i = 0; i < 28; i++)
			SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)items[i]);


		//Create Save button
		hwndButtonSave = CreateWindow(
			TEXT("BUTTON"),		// Predefined class; Unicode assumed 
			TEXT("Save"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			SCREEN_WIDTH*0.75,	// x position 
			10,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU) SAVE_ID,	// No menu.
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);				// Pointer not needed.



		hwndButtonLoad = CreateWindow(
			TEXT("BUTTON"),		// Predefined class; Unicode assumed 
			TEXT("Load"),		// Button text 
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
			SCREEN_WIDTH*0.9,	// x position 
			10,					// y position 
			100,				// Button width
			20,					// Button height
			hwnd,				// Parent window
			(HMENU) LOAD_ID,	// No menu.
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			NULL);				// Pointer not needed.
		
		
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
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (command == 1) //DDA Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine3, AllColors[0]);
		}
		else if (command == 2)	//Midpoint Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine5, AllColors[0]);
		}
		else if (command == 3) //Parametric Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine1, AllColors[0]);
		}
		else if (command == 4) //Direct Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleCartesian, AllColors[1]);
		}
		else if (command == 5) //Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleDPolar, AllColors[1]);
		}
		else if (command == 6) //iterative Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleIPolar, AllColors[1]);
		}
		else if (command == 7) //Midpoint Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMidpoint, AllColors[1]);
		}
		else if (command == 8) //Midpoint Circle Algorithm (Modified)
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMMidpoint, AllColors[1]);
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
			//TODO
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
			contEllipseDraw(hwnd, hdc, lParam, AllColors[1], DrawEllipseDirect);
		}
		else if (command == 19) //Polar Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, AllColors[2], DrawEllipseIterative);
		}
		else if (command == 20) //Midpoint Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, AllColors[0], DrawEllipseMidPoint);

		}
		else if (command == 21) //Rectangle Clipping (point)
		{

		}
		else if (command == 22) //Rectangle Clipping (Line)
		{

		}
		else if (command == 23) //Rectangle Clipping (Polygon)
		{

		}
		else if (command == 24) //Square Clipping (point)
		{

		}
		else if (command == 25) //Square Clipping (Line)
		{

		}
		else if (command == 26) //Circle Clipping (point)
		{

		}
		else if (command == 27) //Circle Clipping (Line)
		{

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