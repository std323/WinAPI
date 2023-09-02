﻿#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include"resource.h"
#include<cstdio>
//#include"cstdio"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Window Class";

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

	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SAMSUNG_BLACK));	//Иконка, отображаемая в панели задач
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SAMSUNG_BLUE));	//Иконка, отображаемая в строке заголовка
	wc.hIcon = (HICON)LoadImage(hInstance, "samsung_blue.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "samsung_black.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));		//Указатель мыши при наведении на окно
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Link.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//Цвет фона окна

	wc.hInstance = hInstance;		//Экземпляр запущенной программы
	wc.lpfnWndProc = WndProc;		//Указатель на процедуру окна
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;	//Имя класса окна. Абсолютно любое окно пренадлежит какому-то классу,
	//т.е., абсолютно любое окно какого-то типа (Кнопка, Текстовое поле....)

	if (!RegisterClassEx(&wc))
	{
		DWORD dwErrorMessageID = GetLastError();
		LPSTR lpszMessageBuffer = NULL;
		DWORD dwSize =
			FormatMessage
			(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwErrorMessageID,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA),
				(LPSTR)&lpszMessageBuffer,
				0,
				NULL
			);
		MessageBox(NULL, lpszMessageBuffer, "Error", MB_OK | MB_ICONERROR);
		LocalFree(lpszMessageBuffer);
		//MessageBox(NULL, "Something went wrong", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:

	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int window_width = screen_width * .75;
	int window_height = screen_height * .75;
	int start_x = screen_width * .125;
	int start_y = screen_height * .125;

	HWND hwnd = CreateWindowEx
	(
		0,
		g_sz_WINDOW_CLASS,		//Имя класса окна
		g_sz_WINDOW_CLASS,		//Заголовок окна
		WS_OVERLAPPEDWINDOW,	//Главное окно программы, еще называется TopLevelWindow

		start_x, start_y,
		window_width, window_height,

		NULL,	//Parent
		NULL,	//Для главного окна - это ID_ меню.
		//Для элемента окна - это ID_ ресурса этого элемента (IDC_EDIT, IDC_BUTTON...)
		hInstance,	//Если нет прямого доступа к hInstance, например потому, что мы не получили его как параметр,
		//то его всегда можно получить при помощи функции GetModuleHandle(NULL);
		NULL
	);
	DWORD dwErrorMessageID = GetLastError();

	if (hwnd == NULL)
	{
		//MessageBox(NULL, "Окно не открывается, поэтому откроем хотя бы форточку", "Полезная информация", MB_OK | MB_ICONINFORMATION);
		LPSTR lpszMessageBuffer = NULL;
		DWORD dwSize =
			FormatMessage
			(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwErrorMessageID,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA),
				(LPSTR)&lpszMessageBuffer,
				0,
				NULL
			);
		MessageBox(NULL, lpszMessageBuffer, "Error", MB_OK | MB_ICONERROR);
		LocalFree(lpszMessageBuffer);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);	//Задаем режим отображения окна
	UpdateWindow(hwnd);			//Выполняет прорисовку окна

	//3) Запуск цикла сообщений:
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);//Преобразуем сообщения клавиш в символы
		DispatchMessageA(&msg);//Передаём сообщение соответствующей функции окна
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
	case WM_SIZE:
	case WM_MOVE:
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		CONST INT SIZE = 256;
		CHAR sz_buffer[SIZE] = {};
		sprintf(sz_buffer, "%s, Position: %ix%i Size: %ix%i", g_sz_WINDOW_CLASS, rect.left, rect.top, width, height);
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		
	}
		CHAR sz_static_text[] = "Этот Static control создан при помощи функции CreateWindowEx()";
		CreateWindowEx
		(
			0, "Static", sz_static_text,
			WS_CHILD | WS_VISIBLE,
			100, 100,
			strlen(sz_static_text)*7.2, 20,
			hwnd,
			(HMENU)IDC_STATIC,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			0, "Edit", "",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			100, 120,
			300, 20,
			hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
		);
		CreateWindowEx
		(
			0, "Button", "OK",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			300, 150,
			100, 25,
			hwnd,
			(HMENU)IDC_BUTTON,
			GetModuleHandle(NULL),
			NULL
		);
		
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON:
		{
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(GetDlgItem(hwnd, IDC_EDIT), WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(GetDlgItem(hwnd, IDC_STATIC), WM_SETTEXT, 0, (LPARAM)sz_buffer);

		}
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:	 DestroyWindow(hwnd); break;
	default:		 return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}