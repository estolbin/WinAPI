#include<WinAPIfamily.h>
#include<Windows.h>
#include"resource.h"

CONST CHAR g_szLoginDefaultMessage[] = "Введите логин.";
CONST CHAR g_szPasswordDefaultMessage[] = "Введите пароль.";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(FindWindow(NULL, "WinApi - Microsoft Visual Studio (Administrator)"), "Hello World", "Info", MB_YESNO | MB_ICONWARNING | MB_HELP | MB_SYSTEMMODAL);

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc, 0);

	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)g_szLoginDefaultMessage);
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_PASSWORD), WM_SETTEXT, 0, (LPARAM)g_szPasswordDefaultMessage);
	}
	break;
	case WM_COMMAND:
	{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_COPY:
		{

			SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
			break;
		case IDOK: MessageBox(hwnd, "Была нажата кнопка ОК", "info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0); break;
		case IDC_EDIT_LOGIN:
			if (HIWORD(wParam) == EN_SETFOCUS)
			{
				SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
				if(strcmp(sz_buffer, g_szLoginDefaultMessage) ==0)
					SendMessage(hLogin, WM_SETTEXT, 0, (LPARAM)"");
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				SendMessage(hLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
				if (strlen(sz_buffer) == 0)
					SendMessage(hLogin, WM_SETTEXT, SIZE, (LPARAM)g_szLoginDefaultMessage);
			}
		}
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}