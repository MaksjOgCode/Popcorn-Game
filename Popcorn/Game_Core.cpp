#include "framework.h"
#include "Game_Core.hpp"

#define MAX_LOADSTRING 100

#define GRAPHIC_GLOBAL_SCALE     3
#define GRAPHIC_LEVEL_X_OFFSET   8
#define GRAPHIC_LEVEL_Y_OFFSET   6
#define GRAPHIC_BRICK_WIDTH      15
#define GRAPHIC_BRICK_HEIGHT     7

HINSTANCE hInst;      
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//---------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(  _In_     HINSTANCE hInstance,
                        _In_opt_ HINSTANCE hPrevInstance,
                        _In_     LPWSTR    lpCmdLine,
                        _In_     int       nCmdShow   )
{
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);

   LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
   LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
   MyRegisterClass(hInstance);

   if (!InitInstance (hInstance, nCmdShow))
      return FALSE;

   HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN));

   MSG msg;

   while ( GetMessage(&msg, nullptr, 0, 0) ) {
      if ( !TranslateAccelerator(msg.hwnd, hAccelTable, &msg) ) {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return (int) msg.wParam;
}
//---------------------------------------------------------------------------------------------------
ATOM MyRegisterClass(HINSTANCE hInstance)
{
   WNDCLASSEXW wcex;

   wcex.cbSize = sizeof(WNDCLASSEX);

   wcex.style          = CS_HREDRAW | CS_VREDRAW;
   wcex.lpfnWndProc    = WndProc;
   wcex.cbClsExtra     = 0;
   wcex.cbWndExtra     = 0;
   wcex.hInstance      = hInstance;
   wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
   wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
   wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
   wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_POPCORN);
   wcex.lpszClassName  = szWindowClass;
   wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

   return RegisterClassExW(&wcex);
}
//---------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   RECT window_rect;
   window_rect.left    = 0;
   window_rect.top     = 0;
   window_rect.right   = 320 * 3;
   window_rect.bottom  = 200 * 3;

   AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

   HWND hWnd = CreateWindowW( szWindowClass, szTitle,
                              WS_OVERLAPPEDWINDOW, 0, 0,
                              window_rect.right - window_rect.left,
                              window_rect.bottom - window_rect.top,
                              nullptr, nullptr, hInstance, nullptr   );

   if (hWnd == 0)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}
//---------------------------------------------------------------------------------------------------
void DrawBrick(HDC hdc, COLORREF RGB, int x_offset, int y_offset) {
   HBRUSH brush = CreateSolidBrush(RGB);
   SelectObject(hdc, brush);
   Rectangle(hdc, x_offset * GRAPHIC_GLOBAL_SCALE,
                  y_offset * GRAPHIC_GLOBAL_SCALE,
                  (x_offset + GRAPHIC_BRICK_WIDTH) * GRAPHIC_GLOBAL_SCALE, 
                  (y_offset + GRAPHIC_BRICK_HEIGHT) * GRAPHIC_GLOBAL_SCALE);
}
//---------------------------------------------------------------------------------------------------
void DrawFrame(HDC hdc) {
      for (int i = 0; i < 15; ++i)
         DrawBrick(hdc, RGB(255, 255, 255), GRAPHIC_LEVEL_X_OFFSET + i * (GRAPHIC_BRICK_WIDTH + 1), GRAPHIC_LEVEL_Y_OFFSET);
}
//---------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      switch (wmId)
      {
      case IDM_ABOUT: {
         DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      } break;

      case IDM_EXIT: {
         DestroyWindow(hWnd);
      } break;

      default: {
         return DefWindowProc(hWnd, message, wParam, lParam);
      }
      }
   } break;

   case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);

      DrawFrame(hdc);

      EndPaint(hWnd, &ps);
   } break;

   case WM_DESTROY: {
      PostQuitMessage(0);
   } break;

   default: {
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
   }
   return 0;
}
//---------------------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
   UNREFERENCED_PARAMETER(lParam);

   switch (message)
   {
   case WM_INITDIALOG: {
      return (INT_PTR)TRUE;
   }

   case WM_COMMAND: {
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
      {
         EndDialog(hDlg, LOWORD(wParam));
         return (INT_PTR)TRUE;
      }
   } break;
   }

   return (INT_PTR)FALSE;
}
//---------------------------------------------------------------------------------------------------