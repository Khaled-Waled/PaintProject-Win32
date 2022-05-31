#include <windows.h>
#include <iostream>
#include "RunnerFunctions.h"
#include "Header.h"
#include "LineAlgos.h"
#include "Clipping.h"
#include "Filling.h"
#include "Filling2.h"

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
			static double xc,yc,r,rx,ry,qx,qy;
			static int counter = 0;
			if(counter==0)
            {
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                counter++;
            }
            else if(counter==1)
            {
                rx = LOWORD(lParam);
                ry = HIWORD(lParam);
                r=sqrt((abs(rx-xc)*abs(rx-xc)) + (abs(ry-yc)*abs(ry-yc)));
                DrawCircleCartesian(hdc,rx,ry,r,RGB(255,0,0));
                counter++;
            }
            else if(counter==2)
            {
                qx = LOWORD(lParam);
                qy = HIWORD(lParam);
                if(qx>=xc)
                {
                    if(qy<yc)
                    {
                        fillingCircleWithLines(hdc,rx,ry,r,RGB(0,255,0),1);
                    }
                    else
                    {
                        fillingCircleWithLines(hdc,rx,ry,r,RGB(0,255,0),2);
                    }
                }
                else
                {
                    if(qy<yc)
                    {
                        fillingCircleWithLines(hdc,rx,ry,r,RGB(0,255,0),4);
                    }
                    else
                    {
                        fillingCircleWithLines(hdc,rx,ry,r,RGB(0,255,0),3);
                    }
                }
            }
		}
		else if (command == 10) //Fill Circle (circles)
		{
			static double xc,yc,r,rx,ry,qx,qy;
			static int counter = 0;
			if(counter==0)
            {
                xc = LOWORD(lParam);
                yc = HIWORD(lParam);
                counter++;
            }
            else if(counter==1)
            {
                rx = LOWORD(lParam);
                ry = HIWORD(lParam);
                r=sqrt((abs(rx-xc)*abs(rx-xc)) + (abs(ry-yc)*abs(ry-yc)));
                DrawCircleCartesian(hdc,rx,ry,r,RGB(255,0,0));
                counter++;
            }
            else if(counter==2)
            {
                qx = LOWORD(lParam);
                qy = HIWORD(lParam);
                if(qx>=xc)
                {
                    if(qy<yc)
                    {
                        fillingCircleWithCircles(hdc,rx,ry,r,RGB(0,255,0),1);
                    }
                    else
                    {
                        fillingCircleWithCircles(hdc,rx,ry,r,RGB(0,255,0),2);
                    }
                }
                else
                {
                    if(qy<yc)
                    {
                        fillingCircleWithCircles(hdc,rx,ry,r,RGB(0,255,0),4);
                    }
                    else
                    {
                        fillingCircleWithCircles(hdc,rx,ry,r,RGB(0,255,0),3);
                    }
                }
            }
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
		else if (command == 13 or command == 14) //Convex and NON-Convex Polygon Fill
		{
		    static int x,y;
		    static vector<point>arr(5);
		    static int counter=0;

		    if(counter<5)
            {
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                arr[counter].x=x;
                arr[counter].y=y;
                counter++;
            }
            else
            {
                for(int i=0;i<5;i++)
                {
                    if(i==4)
                        DrawLine1(hdc, arr[i].x,arr[i].y,arr[0].x,arr[0].y,RGB(255,0,0));
                    else
                        DrawLine1(hdc, arr[i].x,arr[i].y,arr[i+1].x,arr[i+1].y,RGB(255,0,0));
                }
                GeneralPolygonFill(hdc,arr,5,RGB(0,0,255));
            }
		}
		else if (command == 15) //Recursive Flood Fill
		{
		    static int x,y;
		    static vector<point>arr(5);
		    static int counter=0;

		    if(counter<5)
            {
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                arr[counter].x=x;
                arr[counter].y=y;
                counter++;
            }
            else
            {
                for(int i=0;i<5;i++)
                {
                    if(i==4)
                        DrawLine2(hdc, arr[i].x,arr[i].y,arr[0].x,arr[0].y,RGB(255,0,0));
                    else
                        DrawLine2(hdc, arr[i].x,arr[i].y,arr[i+1].x,arr[i+1].y,RGB(255,0,0));
                }
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                myFloodFillNonRec(hdc,x,y,RGB(255,0,0),RGB(0,0,255));
            }
		}
		else if (command == 16) //NON-Recursive Flood Fill
		{
		    static int x,y;
		    static vector<point>arr(5);
		    static int counter=0;

		    if(counter<5)
            {
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                arr[counter].x=x;
                arr[counter].y=y;
                counter++;
            }
            else
            {
                for(int i=0;i<5;i++)
                {
                    if(i==4)
                        DrawLine2(hdc, arr[i].x,arr[i].y,arr[0].x,arr[0].y,RGB(255,0,0));
                    else
                        DrawLine2(hdc, arr[i].x,arr[i].y,arr[i+1].x,arr[i+1].y,RGB(255,0,0));
                }
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                myFloodFillNonRec(hdc,x,y,RGB(255,0,0),RGB(0,0,255));
            }
		}
		else if (command == 17) //Cardinal Spline Curve
		{
			contSpline(hwnd, hdc, lParam, RGB(0, 0, 200));
		}
		else if (command == 18) //Direct Ellipse Drawing
		{

		}
		else if (command == 19) //Polar Ellipse Drawing
		{
			contEllipseDraw(hwnd, hdc, lParam, RGB(0, 0, 255));
		}
		else if (command == 20) //Midpoint Ellipse Drawing
		{

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

		/*case WM_ERASEBKGND:
			HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
			SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
			break;*/
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
