// winver.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winver.h"
#include "Functions.h"
#include "DpiHelpers.h"

HWND hWnd;
using namespace Gdiplus;

// Global Variables:
HINSTANCE hInst;                                // current instance
LPCWSTR title;									// The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// dark mode stuff
fnSetPreferredAppMode SetPreferredAppMode;
fnOpenNcThemeData OpenNcThemeData;
fnAllowDarkModeForWindow AllowDarkModeForWindow;

// variables
OSVERSIONINFOEX os;
LPCTSTR okef;
HKEY hKeyPersonalization;
LPCWSTR MsWin;
LPCWSTR AboutWin;
std::wstring Version;
LPCWSTR CopyRight;
LPCWSTR Owner;
LPCWSTR Organization;
LPCWSTR OkButton;
int DarkThemeEnabled;
HWND yes;
HWND button;
HWND hwndEdit{};

// window size
int Window_Width;
int Window_Height;
int EulaY;
int CopyWidth;
int CopyY;
int OwnerY;
int OrganizationY;
int ButtonX;
int ButtonY;
int EulaWidth;
int BitmapX;

BOOL Eaow = FALSE;
BOOL dpiChanged = FALSE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EulaProc(HWND, UINT, WPARAM, LPARAM);

void SetProperWindowDimensions(LANGID yes)
{
	switch (yes)
	{
		case LANG_POLISH:
		{
			Window_Width = 495;
			Window_Height = 410;
			EulaY = 256;
			CopyWidth = 420;
			OwnerY = 293;
			OrganizationY = 310;
			ButtonX = 413;
			ButtonY = 375;
			EulaWidth = 360;
			BitmapX = 65;
			CopyY = 165;
			break;
		}
		case LANG_GERMAN:
		case LANG_GREEK:
		{
			Window_Width = 465;
			Window_Height = 400;
			EulaY = 256;
			CopyWidth = 400;
			CopyY = 165;
			OwnerY = 293;
			OrganizationY = 310;
			ButtonX = 382;
			ButtonY = 363;
			EulaWidth = 345;
			BitmapX = 60;
			break;
		}
		case LANG_VIETNAMESE:
		{
			Window_Width = 475;
			Window_Height = 385;
			EulaY = 245;
			CopyWidth = 405;
			CopyY = 165;
			OwnerY = 283;
			OrganizationY = 300;
			ButtonX = 393;
			ButtonY = 350;
			EulaWidth = 345;
			BitmapX = 60;
			break;
		}
		case LANG_CHINESE_SIMPLIFIED:
		{
			Window_Width = 467;
			Window_Height = 385;
			EulaY = 243;
			CopyWidth = 385;
			CopyY = 165;
			OwnerY = 279;
			OrganizationY = 297;
			ButtonX = 386;
			ButtonY = 349;
			EulaWidth = 345;
			BitmapX = 55;
			break;
		}
		case LANG_RUSSIAN:
		{
			Window_Width = 475;
			Window_Height = 420;
			EulaY = 280;
			CopyWidth = 385;
			CopyY = 180;
			OwnerY = 320;
			OrganizationY = 338;
			ButtonX = 393;
			ButtonY = 383;
			EulaWidth = 345;
			BitmapX = 63;
			break;
		}
		default:
		{
			Window_Width = 455;
			Window_Height = 380;
			EulaY = 243;
			CopyWidth = 385;
			CopyY = 165;
			OwnerY = 279;
			OrganizationY = 297;
			ButtonX = 373;
			ButtonY = 345;
			EulaWidth = 345;
			BitmapX = 55;
			break;
		}
	}
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    assert(st == Ok);
    if (st != Ok) return FALSE;

	// load strings
	LANGID lang = GetUserDefaultUILanguage();
	SetThreadUILanguage(lang);
	// set window positions
	SetProperWindowDimensions(lang);

	// title
	CString wintitle(MAKEINTRESOURCE(IDS_APP_TITLE));
	title = wintitle;
    LoadStringW(hInstance, IDC_WINVER, szWindowClass, MAX_LOADSTRING);

	// button text
	CString buttontxt(MAKEINTRESOURCE(IDS_BUTTON));
	OkButton = buttontxt;

	// registers class
    MyRegisterClass(hInstance);

    if (!GetwinBrandName())
    {
        return FALSE;
    }

    DoStuff(hInstance);
	DoStuffv2();

#if BUILD_R11
	LPWSTR *lpszArgv;
	int nArgc{};
	lpszArgv = CommandLineToArgvW(GetCommandLineW(), &nArgc);
	for (int i = 0; i < nArgc; i++)
	{
		if (!wcscmp(lpszArgv[i], L"/t"))
		{
			int ok = i + 1;
			title = lpszArgv[ok];
		}
		else if (!wcscmp(lpszArgv[i], L"/a"))
		{
			int ok = i + 1;
			MsWin = lpszArgv[ok];
		}
	}
#endif

    // Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	LoadLibraryExW(L"Msftedit.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCL));
	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_R11));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_R11));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;

    return RegisterClassExW(&wcex);
}

HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

	DarkThemeEnabled = IsExplorerDarkTheme();
    SetPreferredAppMode = (fnSetPreferredAppMode)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
	AllowDarkModeForWindow = (fnAllowDarkModeForWindow)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133));
	OpenNcThemeData = (fnOpenNcThemeData)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(49));
    FreeLibrary(hUxtheme);

	SetPreferredAppMode(PreferredAppMode::AllowDark);
	FixDarkScrollBar();

    hWnd = CreateWindowW(szWindowClass, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return FALSE;

	ScaleWindow(hWnd, Window_Width, Window_Height);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX &  ~WS_SIZEBOX);
	DarkTitleBar(hWnd);
	if (DarkThemeEnabled)
		ApplyMica(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH brush = CreateSolidBrush(DarkThemeEnabled ? darkBkColor : lightBkColor);
    switch (message)
    {
		case WM_CREATE:
		{
			CreateHwnds(hWnd, hInst);
			UpdateLayoutForDpi(button, ButtonX, ButtonY, 70, 23);
			UpdateLayoutForDpi(yes, 47, EulaY, EulaWidth, 40);
			break;
		}
		case WM_DPICHANGED:
		{
			dpiChanged = TRUE;
			UpdateWindowSize(hWnd, lParam);
			UpdateLayoutForDpi(button, ButtonX, ButtonY, 70, 23);
			UpdateLayoutForDpi(yes, 47, EulaY, EulaWidth, 40);
			FixFont(hWnd, yes);
			InvalidateRect(hWnd, NULL, FALSE);
			UpdateWindow(hWnd);
			break;
		}
		case WM_COMMAND:
		{
	        if ((HIWORD(wParam) == BN_CLICKED) && (lParam != 0))
			{
	            HWND hwndBtn = (HWND)lParam;
				if (hwndBtn == button)
					SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
			else if ((LOWORD(wParam) == ID_RETURN) || (LOWORD(wParam) == ID_ESC))
			{
				SendMessage(hWnd, WM_DESTROY, 0, 0);
			}
			else if ((LOWORD(wParam) == ID_E) && !Eaow)
			{
				Eaow = TRUE;
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
				DestroyWindow(yes);
			}
			break;
		}
		case WM_SETTINGCHANGE:
		{
			if (DarkThemeEnabled != IsExplorerDarkTheme())
			{
				DarkThemeEnabled = IsExplorerDarkTheme();
				DarkTitleBar(hWnd);
				if (DarkThemeEnabled)
					ApplyMica(hWnd);
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);
			}
		}
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
			case NM_CLICK:
			case NM_RETURN:
			{
				PNMLINK pNMLink = (PNMLINK)lParam;
				LITEM   item = pNMLink->item;
				if (wcscmp(item.szID, L"idInfo") == 0)
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_EULA), hWnd, EulaProc);					
				}
				break;
			}
			case NM_CUSTOMDRAW:
			{
				CustomDrawButton(lParam, hWnd);
			}
			return CDRF_DODEFAULT;
			break;
			}
		}
		case WM_CTLCOLORSTATIC:
		{
			if (DarkThemeEnabled)
			{
				DarkModeHandler(wParam);
				return reinterpret_cast<INT_PTR>(CreateSolidBrush(darkBkColor));
			}
		}
		case WM_ERASEBKGND:
	        return 0;
		case WM_PAINT:
		{
			RECT rc;
			hdc = BeginPaint(hWnd, &ps);
			
			// set proper bg color
			GetClientRect(hWnd, &rc);
			SetDCBrushColor(hdc, DarkThemeEnabled ? darkBkColor : lightBkColor);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));
			
			// draw logo and strings
			Gdiplus::Graphics graphics(hdc);

			if (!Eaow)
				DrawStrings(hWnd, graphics);
			else
				DrawE(hWnd, graphics);

			DrawLogo(hWnd, graphics, hInst);

			// clean up
			EndPaint(hWnd, &ps);
			DeleteObject(&rc);
			DeleteObject(&graphics);
			DeleteObject(&hdc);
			DeleteObject(&ps);
			DeleteObject(&brush);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
    return 0;
}

INT_PTR CALLBACK EulaProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			ScaleDialog(hDlg);
			SetupRichEdit(hwndEdit, hDlg, hInst);
			UpdateLayoutForDpi(hwndEdit, 10, 15, 580, 280);
			UpdateLayoutForDpi(GetDlgItem(hDlg, IDOK), 525, 320, 70, 23);
			SetWindowTheme(GetDlgItem(hDlg, IDOK), L"Explorer", nullptr);
			CString title(MAKEINTRESOURCE(IDG_TITLE));
			SetWindowText(hDlg, title);
			SetWindowText(GetDlgItem(hDlg, IDOK), OkButton);
			DarkTitleBar(hDlg);
			if (DarkThemeEnabled)
			{
				ApplyMica(hDlg);
				AllowDarkModeForWindow(GetDlgItem(hDlg, IDOK), true);
				SendMessageW(GetDlgItem(hDlg, IDOK), WM_THEMECHANGED, 0, 0);
			}

			UpdateWindow(hDlg);
			break;
		}
		case WM_CTLCOLORSTATIC:
		{
			if (DarkThemeEnabled)
			{
				DarkModeHandler(wParam);
				return reinterpret_cast<INT_PTR>(hbrBkgnd);
			}
			break;
		}
		case WM_CTLCOLORDLG:
		{
			return (INT_PTR)CreateSolidBrush(DarkThemeEnabled ? darkBkColor : lightBkColor);
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
	}
	return (INT_PTR)FALSE;
}
