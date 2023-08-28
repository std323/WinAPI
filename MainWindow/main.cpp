#include<Windows.h>
#include"resource.h"

CONST CHAR g_sz_WINDOW_CLASS[] = "My Window Class";

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) ����������� ������ ����:
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.style = 0;

	//wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SAMSUNG_BLACK));	//������, ������������ � ������ �����
	//wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_SAMSUNG_BLUE));	//������, ������������ � ������ ���������
	wc.hIcon = (HICON)LoadImage(hInstance, "samsung_blue.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "samsung_black.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));		//��������� ���� ��� ��������� �� ����
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Link.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);		//���� ���� ����

	wc.hInstance = hInstance;		//��������� ���������� ���������
	wc.lpfnWndProc = WndProc;		//��������� �� ��������� ����
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_sz_WINDOW_CLASS;	//��� ������ ����. ��������� ����� ���� ����������� ������-�� ������,
	//�.�., ��������� ����� ���� ������-�� ���� (������, ��������� ����....)

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

	//2) �������� ����:

	HWND hwnd = CreateWindowEx
	(
		0,
		g_sz_WINDOW_CLASS,		//��� ������ ����
		g_sz_WINDOW_CLASS,		//��������� ����
		WS_OVERLAPPEDWINDOW,	//������� ���� ���������, ��� ���������� TopLevelWindow


		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,	//Parent
		NULL,	//��� �������� ���� - ��� ID_ ����.
		//��� �������� ���� - ��� ID_ ������� ����� �������� (IDC_EDIT, IDC_BUTTON...)
		hInstance,	//���� ��� ������� ������� � hInstance, �������� ������, ��� �� �� �������� ��� ��� ��������,
		//�� ��� ������ ����� �������� ��� ������ ������� GetModuleHandle(NULL);
		NULL
	);
	DWORD dwErrorMessageID = GetLastError();

	if (hwnd == NULL)
	{
		//MessageBox(NULL, "���� �� �����������, ������� ������� ���� �� ��������", "�������� ����������", MB_OK | MB_ICONINFORMATION);
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
	ShowWindow(hwnd, nCmdShow);	//������ ����� ����������� ����
	UpdateWindow(hwnd);			//��������� ���������� ����

	//3) ������ ����� ���������:
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_CLOSE:	 DestroyWindow(hwnd); break;
	default:		 return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return NULL;
}