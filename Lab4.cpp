// Lab4.cpp : Определяет точку входа для приложения.
//

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <tuple>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
const INT WindowX = 540;
const INT WindowY = 540;

HWND hWnd;
INT GetWind(HWND);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = TEXT("Мой класс");
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
	if (!RegisterClassEx(&wc)) {

		MessageBox(NULL, TEXT("Не получилось зарегистрировать класс!"), TEXT("Ошибка"), MB_OK);
		return NULL;
	}
	hWnd = CreateWindow(
		szClassName,
		TEXT("Лабораторная работа №5"),
		WS_OVERLAPPED | WS_SYSMENU | WS_VSCROLL,
		CW_USEDEFAULT,
		NULL,
		WindowX,
		WindowY,
		static_cast<HWND>(NULL),
		NULL,
		static_cast<HINSTANCE>(hInst),
		NULL);
	if (!hWnd)
	{
		MessageBox(NULL, TEXT("Не получилось создать окно!"), TEXT("Ошибка"), MB_OK);
		return NULL;
	}

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
	
    while (GetMessage(&msg, NULL, NULL, NULL))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
   
}

constexpr INT X =1;
constexpr INT O =2;

INT m_nNextChar = X;
INT m_nGameGrid[9];


void Draw(HWND hWnd, HDC hDC, INT nPos, RECT* m_rcSquares, LPARAM lParam);


int GetRect(int x, int y,RECT* rect)
{
	for (int i = 0; i < 9; i++)
	{
		if (x > rect[i].left && x<rect[i].right &&
			y>rect[i].top && y < rect[i].bottom)
			return i;
	}
	return -1;
}

void DrawX(HDC hDC, int nPos,RECT* rect)
{
	HPEN pen = CreatePen(PS_SOLID, 8, RGB(0, 255, 100));
	SelectObject(hDC, pen);
	RECT rect1 = rect[nPos];

	rect1.top += 30;
	rect1.bottom -= 10;
	rect1.left += 30;
	rect1.right-=10;

	MoveToEx(hDC, rect1.left, rect1.top, NULL);
	LineTo(hDC, rect1.right, rect1.bottom);

	MoveToEx(hDC, rect1.left, rect1.bottom, NULL);
	LineTo(hDC, rect1.right, rect1.top);

	DeleteObject(pen);
}

void DrawO(HDC hDC, int nPos,RECT* rect1)
{
	HPEN pen = CreatePen(PS_SOLID, 8, RGB(0, 255, 100));
	SelectObject(hDC, pen);
	RECT rect = rect1[nPos];

	rect.top += 20;
	rect.bottom -= 20;
	rect.left += 20;
	rect.right -= 20;

	Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
	DeleteObject(pen);
}


void DrawBoard(HWND hWnd,HDC hDC,RECT* rect1)
{
	INT Border = 50;
	INT GridY = WindowY / 3;
	INT GridX = WindowX / 3;
	RECT rect;
	GetClientRect(hWnd, &rect);
	HPEN pen = CreatePen(PS_SOLID, 20, RGB(255, 100, 0));
	SelectObject(hDC, pen);

	MoveToEx(hDC, rect.left+Border, rect.top+GridY, NULL);
	LineTo(hDC,rect.right-Border, rect.top+GridY);

	MoveToEx(hDC, rect.left+Border, rect.top+2*GridY, NULL);
	LineTo(hDC, rect.right-Border, rect.top + 2 * GridY);

	MoveToEx(hDC, rect.left+GridX, rect.top+Border, NULL);
	LineTo(hDC, rect.left + GridX, rect.bottom-Border);

	MoveToEx(hDC, rect.left+2*GridX , rect.top+Border, NULL);
	LineTo(hDC, rect.left + 2 * GridX, rect.bottom-Border);
	
	

	for (int i = 0; i < 9; i++)
	{
		if (m_nGameGrid[i] == X)
		{
			DrawX(hDC, i, rect1);
		}
		else if (m_nGameGrid[i] == O)
		{
			DrawO(hDC, i,rect1);
		}
	}
	DeleteObject(pen);
}


int Winner()
{
	const static std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> g_Condition{

	 { 0, 1, 2 },

	 { 3, 4, 5 },

	 { 6, 7, 8 },

	 { 0, 3, 6 },

	 { 1, 4, 7 },

	 { 2, 5, 8 },

	 { 0, 4, 8 },

	 { 2, 4, 6 }

	};

	for (const auto& current : g_Condition)
	{
		if (const auto [first, second, third] = current;
			(m_nGameGrid[first] == X) &&
			(m_nGameGrid[second] == X) &&
			(m_nGameGrid[third] == X))
		{
			return X;
		}
		else if ((m_nGameGrid[first] == O) &&
			(m_nGameGrid[second] == O) &&
			(m_nGameGrid[third] == O))
		{
			return O;
		}

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
		
	}
	return TRUE;
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
	static HDC hDC;
	PAINTSTRUCT ps;
	static int nPos;
	INT GridY = WindowY / 3;
	INT GridX = WindowX / 3;
	RECT rect;
	GetClientRect(hWnd, &rect);
	RECT m_rcSquares[9] =
	{
	{ rect.left,rect.top,rect.right / 3,rect.bottom/3},
	{ rect.left + GridX,rect.top,rect.right / 3+GridX,rect.bottom/3  },
	{ rect.left + 2 * GridX,rect.top,rect.right,rect.bottom / 3 },
	{ rect.left,rect.top + GridY,rect.right / 3 ,rect.bottom / 3 +  GridY },
	{ rect.left + GridX,rect.top + GridY,rect.right / 3+GridX,rect.bottom / 3 +GridY },
	{ rect.left + 2* GridX,rect.top + GridY,rect.right ,rect.bottom / 3 + GridY },
	{ rect.left,rect.top + 2 * GridY,rect.right / 3,rect.bottom },
	{ rect.left+GridX,rect.top + 2 * GridY,rect.right / 3 +  GridX,rect.bottom },
	{ rect.left + 2 * GridX,rect.top + 2 * GridY,rect.right,rect.bottom }
	};
	switch (uMsg)
	{
	case WM_CREATE:
		ResetGame(hWnd);
	
		break;
	case WM_LBUTTONDOWN:
		if (m_nNextChar != X)
		{
			return 0;
		}
		Draw(hWnd, hDC, nPos, m_rcSquares, lParam);
		return 0;

	case WM_RBUTTONDOWN:
		if (m_nNextChar != O)
			return 0;
		Draw(hWnd, hDC, nPos, m_rcSquares, lParam);
		return 0;

	case WM_MBUTTONDOWN:
		ResetGame(hWnd);
		return 0;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		SetBkMode(hDC, TRANSPARENT);
		DrawBoard(hWnd,hDC, m_rcSquares);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Draw(HWND hWnd, HDC hDC,INT nPos, RECT* m_rcSquares,LPARAM lParam)
{

	nPos = GetRect(LOWORD(lParam), HIWORD(lParam), m_rcSquares);
	if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
	{
		return;
	}
	if (m_nNextChar == X)
	{
		m_nGameGrid[nPos] = X;
		m_nNextChar = O;
		hDC = GetDC(hWnd);
		DrawX(hDC, nPos, m_rcSquares);
		CheckForGameOver(hWnd);
	}
	else if (m_nNextChar == O)
	{
		nPos = GetRect(LOWORD(lParam), HIWORD(lParam), m_rcSquares);
		if ((nPos == -1) || (m_nGameGrid[nPos] != 0))
			return;
		m_nGameGrid[nPos] = O;
		m_nNextChar = X;
		hDC = GetDC(hWnd);
		DrawO(hDC, nPos, m_rcSquares);
		CheckForGameOver(hWnd);
	}
}