#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_INVITE[] = "Введите имя пользователя";

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//hwnd - Окно (Handler to Windows - Обработчик окна)
//uMsg - Сообщение, которое передается окну
//wParam, lParam - параметры сообщения
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	/*MessageBox
	(
		NULL, //Родительское окно. Если его нет, параметр = NULL
		"Hello World!",
		"Info",
		MB_YESNOCANCEL | MB_ICONERROR | MB_HELP | MB_DEFBUTTON4 | MB_SYSTEMMODAL
	);*/

	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc, 0);

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
		//HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		//SetFocus(hEditLogin);

		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		SendMessage(hEditLogin, WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);
		//SetFocus(hEditLogin);


	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LOGIN:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS)
				if (strcmp(sz_buffer, g_sz_INVITE) == 0)
					SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"");

			if (HIWORD(wParam) == EN_KILLFOCUS)
			{

				if (strcmp(sz_buffer, "") == 0)
					SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)g_sz_INVITE);
			}
		}
		break;
		case IDC_BUTTON_COPY:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE] = {};
			HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			SendMessage(hEditLogin, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
		break;

		case IDOK: MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION); break;
		case IDCANCEL: EndDialog(hwnd, 0);

			break;
		case WM_CLOSE: EndDialog(hwnd, 0);
		}
	}
	return FALSE;
}