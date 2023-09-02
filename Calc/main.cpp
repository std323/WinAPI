#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_MY_WINDOW_CLASS[] = "My Calculator";
//g - Global
//sz - String Zero
CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;

CONST INT g_i_INTERVAL = 5;

CONST INT g_i_BTN_SIZE = 150;
CONST INT g_i_BTN_SIZE_WITHN_INTERVAL = g_i_BTN_SIZE + g_i_INTERVAL;
CONST INT g_i_BTN_SIZE_DOUBLE = g_i_BTN_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_DISPLAY_WIDTH = g_i_BTN_SIZE * 5 + g_i_INTERVAL * 4;
CONST INT g_i_DISPLAY_HEIGHT = 50;

CONST INT g_i_BTN_START_X = g_i_START_X;
CONST INT g_i_BTN_START_Y = g_i_START_Y + g_i_DISPLAY_HEIGHT + g_i_INTERVAL;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 3 + g_i_BTN_SIZE_WITHN_INTERVAL * 4 + 30;

CONST CHAR g_OPERATIONS[] = "+-*/";



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;

	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wc.hInstance = hInstance;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = g_sz_MY_WINDOW_CLASS;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}



	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		0,
		g_sz_MY_WINDOW_CLASS,
		g_sz_MY_WINDOW_CLASS,
		WS_SYSMENU | WS_MINIMIZEBOX,

		CW_USEDEFAULT, CW_USEDEFAULT,
		g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);//Преобразуем сообщения клавиш в символы
		DispatchMessageA(&msg);//Передаём сообщение соответствующей функции окна
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hDisplay = CreateWindowEx
		(
			NULL, "Static", "0",
			WS_CHILD | WS_VISIBLE | SS_RIGHT | SS_SUNKEN | SS_CENTERIMAGE,
			g_i_START_X, g_i_START_Y,
			g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd,
			(HMENU)IDC_STATIC,
			GetModuleHandle(NULL),

			NULL
		);
		SendMessage(hDisplay, WM_SETTEXT, 0, (LPARAM)"Dsplay");

		INT i_digit = 1;
		CHAR sz_digit[2] = {};
		/*for (int i = 3; i > 0; i--)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i_digit + (char)48;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * j,
					g_i_START_Y  + g_i_BTN_SIZE_WITHN_INTERVAL * i,
					g_i_BTN_SIZE, g_i_BTN_SIZE,

					hwnd,
					(HMENU)(IDC_BUTTON_0 + i_digit),
					GetModuleHandle(NULL),
					NULL

				);
				i_digit++;
			}
		}*/
		for (int i = 2; i >= 0; i--)
		{
			for (int j = 0; j < 3; j++)
			{
				sz_digit[0] = i_digit + (char)48;
				CreateWindowEx
				(
					NULL, "Button", sz_digit,
					WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
					g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * j,
					g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL * i,
					g_i_BTN_SIZE, g_i_BTN_SIZE,

					hwnd,
					(HMENU)(IDC_BUTTON_0 + i_digit),
					GetModuleHandle(NULL),
					NULL

				);
				i_digit++;
			}
		}
		//////////////////////////////////////////////////
		CreateWindowEx
		(
			NULL, "Button", "0",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BTN_START_X, g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL * 3,
			g_i_BTN_SIZE_DOUBLE, g_i_BTN_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_0,
			GetModuleHandle(NULL),
			NULL

		);
		CreateWindowEx
		(
			NULL, "Button", ".",
			WS_CHILDWINDOW | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * 2, g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL * 3,
			g_i_BTN_SIZE, g_i_BTN_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT,
			GetModuleHandle(NULL),
			NULL
		);

		//////////////////////////////////////////////////

		INT i_operation = 0;
		CHAR sz_operation[2] = {};
		for (int i = 3; i >= 0; i--)
		{
			sz_operation[0] = g_OPERATIONS[i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation,
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * 3, g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL * (3 - i),
				g_i_BTN_SIZE, g_i_BTN_SIZE,
				hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i),
				GetModuleHandle(NULL),
				NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "<-",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * 4, g_i_BTN_START_Y,
			g_i_BTN_SIZE, g_i_BTN_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_BSP,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "C",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * 4, g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL,
			g_i_BTN_SIZE, g_i_BTN_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_CLEAR,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			g_i_BTN_START_X + g_i_BTN_SIZE_WITHN_INTERVAL * 4, g_i_BTN_START_Y + g_i_BTN_SIZE_WITHN_INTERVAL * 2,
			g_i_BTN_SIZE, g_i_BTN_SIZE_DOUBLE,
			hwnd,
			(HMENU)IDC_BUTTON_EQUAL,
			GetModuleHandle(NULL),
			NULL
		);
	}
	break;
	case WM_COMMAND:
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:	 DestroyWindow(hwnd); break;
	default:		 return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}
