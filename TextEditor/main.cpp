﻿#define _CRT_SECURE_NO_WARNINGS
#include<Windows.h>
#include<Richedit.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Text Editor";


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName, LPSTR lpszFileText);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName, LPSTR lpszText, LPSTR lpszFileText);
BOOL FileWasChanged(HWND hEdit, LPSTR lpszText, LPSTR lpszFileText);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(wc);
	wc.style = 0;

	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	
		
	wc.hInstance = hInstance;
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
	wc.lpfnWndProc = WndProc;
	
	if (!RegisterClassEx(&wc))  //Эта функция регистрирует класс окна
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	//2) Создание окна:
	HWND hwnd = CreateWindowEx
	(
		NULL,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
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
		DispatchMessage(&msg);//Передаём сообщение соответствующей функции окна
	}
	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CHAR szFileName[MAX_PATH]{};
	static LPSTR lpszText = NULL;
	static LPSTR lpszFileText = NULL;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		LoadLibrary("riched20.dll");
		RECT rect;
		GetClientRect(hwnd, &rect);//Получаем прямоугольник клиентской области окна
		//Клиентская область окна - это все окно, кроме строки меню, строки заголовка и строки состояния
		//Клиентская область - это рабочая область приложения
		HWND hEdit = CreateWindowEx
		(
			NULL, RICHEDIT_CLASS, "",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE,
			0, 0,
			rect.right, rect.bottom,
		    hwnd,
			(HMENU)IDC_EDIT,
			GetModuleHandle(NULL),
			NULL
			);
	}
		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			//CHAR szFileName[MAX_PATH]{};
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;    //Owner - владелец
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0ALL files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "txt";    //Deafault Extension

			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				LoadTextFileToEdit(hEdit, szFileName, lpszFileText);
			}

		}
		break;
		case ID_FILE_SAVE:
		{
			if (szFileName[0])
			{
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName, lpszText, lpszFileText);
				break;
			}
		}
			
		case ID_FILE_SAVEAS:
		{
			//CHAR szFileName[MAX_PATH] = {};

			OPENFILENAME ofn;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files (*.txt)\0 * .txt\0ALL files(*.*)\0 * .*\0";//Double NULL-Terminated line
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "txt";

			if (GetSaveFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				SaveTextFileFromEdit(hEdit, szFileName, lpszText, lpszFileText);
			}
		}
		break;
		}
	}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:	 
	{
		if (FileWasChanged(GetDlgItem(hwnd, IDC_EDIT), lpszText, lpszFileText))
		{
			switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Question", MB_YESNOCANCEL | MB_ICONQUESTION))
			{
			case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
			case IDNO: DestroyWindow(hwnd);
			case IDCANCEL: break;
			}
			
		}
		else
		{
			DestroyWindow(hwnd);
		}
	}
		break;
	default:		 return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName, LPSTR lpszFileText)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFileText)
			{
				DWORD dwRead;
				if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL))
				{
					lpszFileText[dwFileSize] = 0;    //NULL Terminator
					if (SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileText))bSuccess = TRUE;
				}
				GlobalFree(lpszFileText);
			}
			CloseHandle(hFile);
		}
	}

	return bSuccess;
}

BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName, LPSTR lpszText, LPSTR lpszFileText)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = GetWindowTextLength(hEdit);
		if (dwTextLength > 0)
		{
			lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (lpszText != NULL)
			{
				lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
				if (GetWindowText(hEdit, lpszText, dwTextLength + 1))
				{
					DWORD dwWritten;
					if (WriteFile(hFile, lpszText, dwTextLength, &dwWritten, NULL))bSuccess = TRUE;
				}
				GlobalFree(lpszText);
			}
		}
		CloseHandle(hFile);
		strcpy(lpszFileText, lpszText);
	}
	return bSuccess;
}

BOOL FileWasChanged(HWND hEdit, LPSTR lpszText, LPSTR lpszFileText)
{
	BOOL fileChanged = FALSE;
	DWORD dwTextLength = GetWindowTextLength(hEdit);
	if (dwTextLength == 0 && lpszText == NULL && lpszFileText == NULL)return FALSE;
	if (dwTextLength > 0)
	{
		lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
		if (lpszFileText == NULL)lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
		SendMessage(hEdit, WM_GETTEXT, dwTextLength, (LPARAM)lpszText);
		//if (lpszText && lpszFileText)
		{
			//if()
			if(lpszFileText && strcmp(lpszText, lpszFileText))fileChanged = TRUE;
		}
	}
	return fileChanged;
}


