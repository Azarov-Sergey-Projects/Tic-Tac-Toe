// Lab4.cpp : Определяет точку входа для приложения.
//

#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
    TCHAR szClassName[] = TEXT("Мой класс");
    HWND hMainWnd;
    MSG msg;
    WNDCLASSEX wc;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName;
    wc.cbWndExtra = NULL;
    wc.cbClsExtra = NULL;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
    wc.hInstance = hInst;
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Не получилось зарегистрировать класс!"), TEXT("Ошибка"), MB_OK);
        return NULL;
    }

    hMainWnd = CreateWindow(szClassName, TEXT("Лабораторая работа 4"), WS_OVERLAPPEDWINDOW | WS_VSCROLL, CW_USEDEFAULT, NULL, 520, 520, static_cast<HWND>(NULL), NULL, static_cast<HINSTANCE>(hInst), NULL);
    if (!hMainWnd)
    {
        MessageBox(NULL, TEXT("Не получилось создать окно!"), TEXT("Ошибка"), MB_OK);
        return NULL;
    }

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
   
}
const RECT m_rcSquares[9] = {
{ 22, 22,150,150},
{182, 22,320,150},
{342, 22,460,150},
{ 22,182,150,320},
{182,182,320,320},
{342,182,460,320},
{ 22,342,150,460},
{182,342,320,460},
{342,342,460,460}
};
#define X 1
#define O 2

INT m_nNextChar = X;
INT m_nGameGrid[9];





int GetRect(int x, int y)
{
	for (int i = 0; i < 9; i++)
	{
		if (x > m_rcSquares[i].left && x<m_rcSquares[i].right &&
			y>m_rcSquares[i].top && y < m_rcSquares[i].bottom)
			return i;
	}
	return -1;
}

void DrawX(HDC hDC, int nPos)
{
	HPEN pen = CreatePen(PS_SOLID, 8, RGB(0, 255, 100));
	SelectObject(hDC, pen);
	RECT rect = m_rcSquares[nPos];

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;

	MoveToEx(hDC, rect.left, rect.top, NULL);
	LineTo(hDC, rect.right, rect.bottom);

	MoveToEx(hDC, rect.left, rect.bottom, NULL);
	LineTo(hDC, rect.right, rect.top);

	DeleteObject(pen);
}

void DrawO(HDC hDC, int nPos)
{
	HPEN pen = CreatePen(PS_SOLID, 8, RGB(0, 255, 100));
	SelectObject(hDC, pen);
	RECT rect = m_rcSquares[nPos];

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;

	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject(pen);
}


void DrawBoard(HDC hDC)
{
	HPEN pen = CreatePen(PS_SOLID, 20, RGB(255, 100, 0));
	SelectObject(hDC, pen);

	MoveToEx(hDC, 171, 22, NULL);
	LineTo(hDC, 171, 450);

	MoveToEx(hDC, 331, 16, NULL);
	LineTo(hDC, 331, 450);
	
	MoveToEx(hDC, 22, 171, NULL);
	LineTo(hDC, 460, 171);

	MoveToEx(hDC, 22,331, NULL);
	LineTo(hDC, 460, 331);

	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == X)
		{
			DrawX(hDC, i);
		}
		else if (m_nGameGrid[i] == O)
		{
			DrawO(hDC, i);
		}
	}
	DeleteObject(pen);
}


int Winner()
{
	static int Condition[8][3] =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		0, 3, 6,
		1, 4, 7,
		2, 5, 8,
		0, 4, 8,
		2, 4, 6
	};

	for (int i = 0; i < 8; i++)
	{
		if ((m_nGameGrid[Condition[i][0]] == X) &&
			(m_nGameGrid[Condition[i][1]] == X) &&
			(m_nGameGrid[Condition[i][2]] == X))
			return X;
		if ((m_nGameGrid[Condition[i][0]] == O) &&
			(m_nGameGrid[Condition[i][1]] == O) &&
			(m_nGameGrid[Condition[i][2]] == O))
			return O;
	}
	return 0;
}

BOOL Draw()
{
	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == 0)
		{
			return FALSE;
		}
		return TRUE;
	}
}


void ResetGame(HWND hWnd)
{
	RECT rect;
	m_nNextChar = X;
	ZeroMemory(m_nGameGrid, 9 * sizeof(INT));
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, TRUE);
}

void CheckForGameOver(HWND hWnd)
{
	int nWinner;

	//if the grid contains three consecutive X's or O's, declare a winner and start a new game
	if (nWinner = Winner())
	{
		MessageBox(hWnd, ((nWinner == X) ? _T("X wins!") : _T("O wins!")),
			_T("Game Over"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame(hWnd);
	}
	else if (Draw())
	{
		MessageBox(hWnd, _T("It's a draw!"), _T("Game Over"), MB_ICONEXCLAMATION | MB_OK);
		ResetGame(hWnd);
	}
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int nPos;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		if (m_nNextChar != X)
			return 0;
		nPos = GetRect(LOWORD(lParam), HIWORD(lParam));
		if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
			return 0;
		m_nGameGrid[nPos] = X;
		m_nNextChar = O;
		hdc = GetDC(hWnd);
		DrawX(hdc, nPos);
		CheckForGameOver(hWnd);
		return 0;

	case WM_RBUTTONDOWN:
		if (m_nNextChar != O)
			return 0;
		nPos = GetRect(LOWORD(lParam), HIWORD(lParam));
		if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
			return 0;
		m_nGameGrid[nPos] = O;
		m_nNextChar = X;
		hdc = GetDC(hWnd);
		DrawO(hdc, nPos);
		CheckForGameOver(hWnd);
		return 0;

	case WM_MBUTTONDOWN:
		ResetGame(hWnd);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		//GetClientRect (hwnd, &rect);
		SetBkMode(hdc, TRANSPARENT);
		DrawBoard(hdc);

		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
