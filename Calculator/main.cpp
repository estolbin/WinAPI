#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<cstdio>
#include"resource.h"



CONST CHAR g_szClassName[] = "Calculator";
CONST CHAR g_szWindowTitle[] = "Calculator";

const int g_iScreenHeight = 25;
const int g_iButtonSize = 50;
const int g_iInterval = 15;
const int g_iLength = g_iButtonSize + g_iInterval;
const int g_xstart = 15;
const int g_ystart = 15;

class CalcButton
{
public:
	CalcButton(LPCSTR WindowName, int X, int Y, HWND hwnd,HMENU idButton, int xsize = g_iButtonSize, int ysize = g_iButtonSize)
	{
		CreateWindowEx(
			0,
			"Button",
			WindowName,
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			X,
			Y,
			xsize,
			ysize,
			hwnd,
			(HMENU)idButton,
			GetModuleHandle(NULL),
			NULL
		);
	}
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GiveErrorMessage(LPSTR text);
void CreateMenu(HWND hwnd);
void CreateButtons(HWND hwnd);
void AboutDlg(HWND hwnd);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = g_szClassName;
	wc.hIcon = (HICON)LoadImage(NULL,             // hInstance must be NULL when loading from a file
		"iconfile.ico",   // the icon file name
		IMAGE_ICON,       // specifies that the file is an icon
		0,                // width of the image (we'll specify default later on)
		0,                // height of the image
		LR_LOADFROMFILE |  // we want to load a file (as opposed to a resource)
		LR_DEFAULTSIZE);   // default metrics based on the type (IMAGE_ICON, 32x32)
	

	if (!RegisterClassEx(&wc))
	{
		return GiveErrorMessage((LPSTR)"Failed to register class");
	}

	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int window_width = g_xstart * 3 + g_iLength * 5 - g_iInterval;
	int window_height = g_ystart * 5 + g_iScreenHeight + g_iLength * 4 + g_iInterval * 2;
	int start_x = screen_width * .125;
	int start_y = screen_height * .125;

	hwnd = CreateWindowEx(0, g_szClassName, g_szWindowTitle, WS_OVERLAPPEDWINDOW, start_x, start_y, window_width, window_height, NULL, NULL, hInstance, NULL);
	if (!hwnd)
	{
		return GiveErrorMessage((LPSTR)"Failed to create window");
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			CreateMenu(hwnd);
			CreateButtons(hwnd);
		}
		break;
		case WM_COMMAND:
		{

			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			CONST INT SIZE = 256;
			char sz_buffer[SIZE] = {};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			CHAR sz_digit[2] = {};
			static CHAR sz_stack[SIZE] = {};
			static char operation = ' ';
			if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_9)
			{
				if (sz_buffer[0] == '+' || sz_buffer[0] == '-' || sz_buffer[0] == '*' || sz_buffer[0] == '/') memset(sz_buffer, 0, SIZE);
				sz_digit[0] = LOWORD(wParam) - 952;
				strcat(sz_buffer, sz_digit);
				SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			}
			else
			{
				switch (LOWORD(wParam))
				{
				case IDC_BUTTON_POINT:
				{
					if (strchr(sz_buffer, '.')) break;
					strcat(sz_buffer, ".");
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)sz_buffer);
				}
					break;
				case IDC_BUTTON_CLEAR:
					SendMessage(hEdit, WM_SETTEXT, 0, NULL);
					break;
					// привязать арифметические операции к классу кнопки?
				case IDC_BUTTON_PLUS:
					strcpy(sz_stack, sz_buffer);
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"+");
					operation = '+';
					break;
				case IDC_BUTTON_MINUS:
					strcpy(sz_stack, sz_buffer);
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"-");
					operation = '-';
					break;
				case IDC_BUTTON_ASTER:
					strcpy(sz_stack, sz_buffer);
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"*");
					operation = '*';
					break;
				case IDC_BUTTON_SLASH:
					strcpy(sz_stack, sz_buffer);
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"/");
					operation = '/';
					break;
				case IDC_BUTTON_EQUAL:
					{
					char tmp[SIZE] = {};
					if (operation == '+')
						sprintf(tmp, "%.2f", atof(sz_stack) + atof(sz_buffer));
					else if (operation == '-')
						sprintf(tmp, "%.2f", atof(sz_stack) - atof(sz_buffer));
					else if (operation == '*')
						sprintf(tmp, "%.2f", atof(sz_stack) * atof(sz_buffer));
					else if (operation == '/')
						sprintf(tmp, "%.2f", atof(sz_stack) / atof(sz_buffer));

					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)tmp);
					memset(sz_stack, 0, SIZE);
					}
					break;
				case IDM_EXIT:
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				case IDM_ABOUT:
					AboutDlg(hwnd);
				}
			}
		}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		
	}
}

int GiveErrorMessage(LPSTR text)
{
	MessageBox(NULL, text, "Error", MB_OK);
	return 0;
}

void CreateMenu(HWND hwnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreateMenu();

	AppendMenu(SubMenu, MF_STRING, IDM_EXIT, "E&xit");
	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, "File");

	AppendMenu(RootMenu, MF_STRING, IDM_ABOUT, "A&bout");

	SetMenu(hwnd, RootMenu);
}

void CreateButtons(HWND hwnd)
{
	CreateWindowEx(0, "Edit", "", WS_BORDER | WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY, g_xstart, g_ystart, g_iLength * 5 - g_iInterval, g_iScreenHeight, hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL);


	int digit = 1;
	char sz_digit[] = "0";

	for (int i = 0;i < 3;i++)
	{
		for (int j = 0; j < 3;j++)
		{
			sz_digit[0] = '0' + digit;
			new CalcButton(sz_digit, g_xstart + g_iLength * j, g_ystart + g_iScreenHeight + g_iInterval + g_iLength * (2 - i), hwnd, (HMENU)(IDC_BUTTON_0 + digit));
			digit++;
		}
	}
	new CalcButton("0", g_xstart, g_ystart + g_iScreenHeight + g_iInterval + (g_iLength * 3), hwnd, (HMENU)IDC_BUTTON_0, g_iLength + g_iButtonSize);
	new CalcButton(".", g_xstart + g_iButtonSize * 2 + g_iInterval * 2, g_ystart + g_iInterval + g_iScreenHeight + g_iLength * 3, hwnd, (HMENU)IDC_BUTTON_POINT);

	new CalcButton("+", g_xstart + g_iLength * 3, g_ystart + g_iScreenHeight + g_iInterval, hwnd, (HMENU)IDC_BUTTON_PLUS);
	new CalcButton("-", g_xstart + g_iLength * 3, g_ystart + g_iScreenHeight + g_iInterval + g_iLength, hwnd, (HMENU)IDC_BUTTON_MINUS);
	new CalcButton("*", g_xstart + g_iLength * 3, g_ystart + g_iScreenHeight + g_iInterval + g_iLength * 2, hwnd, (HMENU)IDC_BUTTON_ASTER);
	new CalcButton("/", g_xstart + g_iLength * 3, g_ystart + g_iScreenHeight + g_iInterval + g_iLength * 3, hwnd, (HMENU)IDC_BUTTON_SLASH);

	new CalcButton("C", g_xstart + g_iLength * 4, g_ystart + g_iScreenHeight + g_iInterval, hwnd, (HMENU)IDC_BUTTON_CLEAR, g_iButtonSize, g_iLength + g_iButtonSize);
	new CalcButton("=", g_xstart + g_iLength * 4, g_ystart + g_iScreenHeight + g_iLength*2 + g_iInterval, hwnd, (HMENU)IDC_BUTTON_EQUAL, g_iButtonSize, g_iLength + g_iButtonSize);
}

void AboutDlg(HWND hwnd)
{
	MessageBox(hwnd, "Это простой калькулятор, с использованием WINAPI\nверсия: СБД-121", "О программе", MB_OK);
}