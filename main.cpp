#include <windows.h> 
#include <iostream>
#include "RunnerFunctions.h"
#include "Header.h"
#include "LineAlgos.h"

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hinst;
int command = 0;
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
	hwnd = CreateWindow(wc.lpszClassName, TEXT("Choose option"), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1280, 720, 0, 0, hInstance, 0);
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
			
			if (sel == 0 || sel != command) //Change the background of window to be white
			{
				HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
				SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
				InvalidateRect(hwnd, NULL, TRUE); 
			}
			command = sel;


			SetFocus(hwnd);
		}
		break;
	case WM_LBUTTONDOWN:
		if (command == 1) //DDA Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine3); 
		}
		else if (command == 2)	//Midpoint Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine5);
		}
		else if (command == 3) //Parametric Line Algorithm
		{
			contLineDraw(hwnd, hdc, lParam, DrawLine1);
		}
		else if (command == 4) //Direct Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleCartesian);
		}
		else if (command == 5) //Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleDPolar);
		}
		else if (command == 6) //iterative Polar Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleIPolar);
		}
		else if (command == 7) //Midpoint Circle Algorithm
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMidpoint);
		}
		else if (command == 8) //Midpoint Circle Algorithm (Modified)
		{
			contCircleDraw(hwnd, hdc, lParam, DrawCircleMMidpoint);
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

		}
		else if (command == 18) //Direct Ellipse Drawing
		{

		}
		else if (command == 19) //Polar Ellipse Drawing
		{

		}
		else if (command == 20) //Midpoint Ellipse Drawing
		{

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

	/*case WM_ERASEBKGND:
		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
		break;*/
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}