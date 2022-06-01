// winver.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "winver.h"
#include "Functions.h"

HWND hWnd;
using namespace Gdiplus;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
LPCWSTR title = L"About Windows";
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// dark mode stuff
fnSetPreferredAppMode SetPreferredAppMode;
fnRefreshImmersiveColorPolicyState RefreshImmersiveColorPolicyState;
fnAllowDarkModeForWindow AllowDarkModeForWindow;

// variables
OSVERSIONINFOEX os;
LPCTSTR okef;
HKEY hKeyPersonalization;
LPCWSTR MsWin;
LPCWSTR AboutWin;
LPCWSTR Version;
LPCWSTR CopyRight;
LPCWSTR Owner;
LPCWSTR Organization;
int DarkThemeEnabled;
HWND yes;
HWND button;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EulaProc(HWND, UINT, WPARAM, LPARAM);

BOOL GetwinBrandName()
{
    BOOL BRet = FALSE;
    HMODULE hModNtdll = NULL;
    if (hModNtdll = LoadLibraryExW(L"winbrand.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32))
    {
        typedef LPTSTR(WINAPI* BrandingFormatString) (const wchar_t*);
        BrandingFormatString getwinName;
        getwinName = (BrandingFormatString)::GetProcAddress(hModNtdll, "BrandingFormatString");
        if (getwinName)
        {
            CString lpct(MAKEINTRESOURCE(IDS_TEXT_COPY));
            AboutWin = getwinName(lpct);
            BRet = TRUE;

            const wchar_t copy[50] = L"%WINDOWS_COPYRIGHT%";
            CopyRight = getwinName(copy);

            const wchar_t msw[50] = L"%MICROSOFT_COMPANYNAME% %WINDOWS_GENERIC%";
            MsWin = getwinName(msw);
            BRet = TRUE;
        }
        ::FreeLibrary(hModNtdll);
        hModNtdll = NULL;
    }
    else
    {
        MessageBox(hWnd, L"Bruh can't find winbrand.dll", L"Error", MB_ICONERROR);
    }
    return BRet;
}
DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE lpBuff, LONG cb, PLONG pcb)
{
	HANDLE hFile = (HANDLE)dwCookie;
	return !ReadFile(hFile, lpBuff, cb, (DWORD *)pcb, NULL);
}
BOOL FillRichEditFromFile(HWND hwnd, LPCTSTR pszFile)
{
	BOOL fSuccess = FALSE;
	HANDLE hFile = CreateFile(pszFile, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		EDITSTREAM es = { (DWORD_PTR)hFile, 0, EditStreamCallback };
		if (SendMessage(hwnd, EM_STREAMIN, SF_RTF, (LPARAM)&es) &&
			es.dwError == 0) {
			fSuccess = TRUE;
		}
		CloseHandle(hFile);
	}
	return fSuccess;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Status st = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    assert(st == Ok);
    if (st != Ok) return FALSE;

    // Initialize global strings

	CString wintitle(MAKEINTRESOURCE(IDS_APP_TITLE));
	title = wintitle;
    LoadStringW(hInstance, IDC_WINVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!GetwinBrandName())
    {
        return FALSE;
    }

    DoStuff();
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

	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
#if BUILD_R11
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_R11ICO));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_R11ICO));
#else 
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STOCKICO));
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_STOCKICO));
#endif
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
    RefreshImmersiveColorPolicyState = (fnRefreshImmersiveColorPolicyState)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(104));
    AllowDarkModeForWindow = (fnAllowDarkModeForWindow)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133));
    FreeLibrary(hUxtheme);

    if (DarkThemeEnabled)
    {
        SetPreferredAppMode(PreferredAppMode::ForceDark);
    }
    hWnd = CreateWindowW(szWindowClass, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 472, 425, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX &  ~WS_SIZEBOX);
	DarkTitleBar(hWnd);
    ApplyMica(hWnd);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH brush = CreateSolidBrush(darkBkColor);
    switch (message)
    {
		case WM_CREATE:
		{
			CreateHwnds(hWnd, hInst);
			break;
		}
		case WM_COMMAND:
		{
	        if ((HIWORD(wParam) == BN_CLICKED) && (lParam != 0))
			{
	            HWND hwndBtn = (HWND)lParam;
				if (hwndBtn == button)
					PostQuitMessage(0);
			}
			break;	
		}
		case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
			case NM_CLICK:          // Fall through to the next case.

			case NM_RETURN:
			{
				PNMLINK pNMLink = (PNMLINK)lParam;
				LITEM   item = pNMLink->item;
				if (wcscmp(item.szID, L"idInfo") == 0)
				{
					// WIP
					// DialogBox(hInst, MAKEINTRESOURCE(IDD_EULA), hWnd, EulaProc);
					ShellExecute(hWnd, L"open", L"write.exe", L"C:\\Windows\\System32\\license.rtf", NULL, SW_SHOW);
				}
				break;
			}
		}
		case WM_CTLCOLORSTATIC:
		{
	        if (DarkThemeEnabled)
			{
	            HDC hdc = reinterpret_cast<HDC>(wParam);
				SetTextColor(hdc, darkTextColor);
				SetBkColor(hdc, darkBkColor);
				if (!hbrBkgnd)
					hbrBkgnd = CreateSolidBrush(darkBkColor);
	            return reinterpret_cast<INT_PTR>(hbrBkgnd);
		    }
			break;
		}
		case WM_ERASEBKGND:
	        return 0;
		case WM_PAINT:
	    {
			RECT rc;
			hdc = BeginPaint(hWnd, &ps);
					
			GetClientRect(hWnd, &rc);
			SetDCBrushColor(hdc, DarkThemeEnabled ? darkBkColor : lightBkColor);
			FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));

	        Gdiplus::Graphics graphics(hdc);
			DrawStrings(graphics, hInst);
			EndPaint(hWnd, &ps);
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
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
	}
	return (INT_PTR)FALSE;
}
