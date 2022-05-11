#if defined(UNICODE) && !defined(_UNICODE)
#endif
#include <tchar.h>
#include <windows.h>
#include<math.h>
using namespace std;


void draw8Points (HDC hdc,int x,int y,int xc,int yc)
{
    SetPixel(hdc,xc+x,yc+y,RGB(0,170,0));
    SetPixel(hdc,xc-x,yc+y,RGB(0,170,0));
    SetPixel(hdc,xc+x,yc-y,RGB(0,170,0));
    SetPixel(hdc,xc-x,yc-y,RGB(0,170,0));
    SetPixel(hdc,xc-y,yc+x,RGB(0,170,0));
    SetPixel(hdc,xc+y,yc-x,RGB(0,170,0));
    SetPixel(hdc,xc+y,yc+x,RGB(0,170,0));
    SetPixel(hdc,xc-y,yc-x,RGB(0,170,0));
}

void midpoint(HDC hdc,int xc,int yc,int r)
{
    int x=0;
    int y=r;
    double d=1-r;
    while(x<y)
    {
        if(d<=0)
        {
            d=d+2*x+3;
            x++;
        }
        else
        {
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        draw8Points(hdc,x,y,xc,yc);
    }
}

int Round(double x)
{
	return (int)(x + 0.5);
}


struct Vector{
	double v[2];
	Vector(double x = 0, double y = 0)
	 { v[0] = x; v[1] = y; }
	double& operator[](int i){ return v[i];
	}
};

 boolean insideCircle(int x, int y, int xc, int yc, int r) {
        int d = sqrt(pow((x-xc),2)+pow((y-yc),2));
        if (d<=r) {
            return true;
        }
        return false;
    };

void DrawHermiteCurve(HDC hdc,Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c, int xc, int yc, int r)
{
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];
	for (double t = 0; t <= 1; t += 0.001)
	{
		double t2 = t*t, t3 = t2*t;
		double x = a0 + a1*t + a2*t2 + a3*t3;
		double y = b0 + b1*t + b2*t2 + b3*t3;
		if (insideCircle(x,y,xc,yc,r)) {SetPixel(hdc,x,y,c);}
            else {SetPixel(hdc,x,y,RGB(0,0,255));}
	}

}


/*  This function is called by the Windows function DispatchMessage()  */
int xc,yc,r,x2,y2,r2;
int leftClickCount =0;
LRESULT WINAPI MyWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 	static Vector p[4];
	static int index = 0;
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONUP:
        switch (leftClickCount)
        {
        case 0 :
            xc=LOWORD(lParam);
            yc=HIWORD(lParam);
            leftClickCount++;
            break;
        case 1:
            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            r = sqrt(pow((x2-xc),2)+pow((y2-yc),2));
            midpoint(hdc,xc,yc,r);
            leftClickCount++;
            break;
        case 2:
            {
            p[index] = Vector(LOWORD(lParam), HIWORD(lParam));
            if (index == 3){
            Vector T1(3 * (p[1][0] - p[0][0]), 3 * (p[1][1] - p[0][1]));
			Vector T2(3 * (p[3][0] - p[2][0]), 3 * (p[3][1] - p[2][1]));
			hdc = GetDC(hwnd);
			DrawHermiteCurve(hdc, p[0], T1, p[3], T2,RGB(255,0,0), xc,yc,r);
			ReleaseDC(hwnd, hdc);
			leftClickCount++;
            }
            else index++;
		}
            break;
        case 3:
            xc=LOWORD(lParam);
            yc=HIWORD(lParam);
            leftClickCount++;
            break;
        case 4:
            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            r = sqrt(pow((x2-xc),2)+pow((y2-yc),2));
            midpoint(hdc,xc,yc,r);
            leftClickCount++;
            break;
        case 5:
            xc=LOWORD(lParam);
            yc=HIWORD(lParam);
            leftClickCount++;
            break;
        case 6:
            x2=LOWORD(lParam);
            y2=HIWORD(lParam);
            r = sqrt(pow((x2-xc),2)+pow((y2-yc),2));
            midpoint(hdc,xc,yc,r);
            leftClickCount++;
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}



int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = "MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow("MyClass", "My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
