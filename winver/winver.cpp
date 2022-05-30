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
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND butt;

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
LPCWSTR EulaText;
LPCWSTR Owner;
LPCWSTR Organization;
int DarkThemeEnabled;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


BOOL GetwinBrandName()
{
    BOOL BRet = FALSE;
    HMODULE hModNtdll = NULL;
    if (hModNtdll = ::LoadLibraryW(L"winbrand.dll"))
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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!GetwinBrandName())
    {
        return FALSE;
    }
    DoStuff();
	DoStuffv2();

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINVER));

MSG msg;
// Main message loop:
while (GetMessage(&msg, nullptr, 0, 0))
{
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINVER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WINVER));

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
    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 472, 425, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);

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
        butt = CreateWindow(L"Button", L"OK", WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_FLAT | BS_DEFPUSHBUTTON, 377, 352, 70, 23, hWnd, NULL, NULL, NULL);
        SetWindowTheme(butt, L"Explorer", nullptr);
        SendMessage(butt, WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), true);
        SendMessageW(hWnd, WM_THEMECHANGED, 0, 0);
        UpdateWindow(hWnd);
    }
    break;
    case WM_COMMAND:
    {
        if ((HIWORD(wParam) == BN_CLICKED) && (lParam != 0))
        {
            HWND hwndBtn = (HWND)lParam;

            if (hwndBtn == butt)
                DestroyWindow(hWnd);
        }
        break;
        
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
    }
    break;
    case WM_THEMECHANGED:
    {
        if (DarkThemeEnabled)
        {
            AllowDarkModeForWindow(hWnd, DarkThemeEnabled);
            AllowDarkModeForWindow(butt, DarkThemeEnabled);
            SendMessageW(butt, WM_THEMECHANGED, 0, 0);
            UpdateWindow(hWnd);
        }
    }
    break;
    case WM_ERASEBKGND:
        return 0;
    case WM_PAINT:
    {
        RECT rc;
        hdc = BeginPaint(hWnd, &ps);

        GetClientRect(hWnd, &rc);
        SetDCBrushColor(hdc, DarkThemeEnabled ? darkBkColor : lightBkColor);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(DC_BRUSH));

        Gdiplus::Graphics        graphics(hdc);
        SolidBrush      lightmodetext(Gdiplus::Color(255, 0, 0, 0));
        SolidBrush      darkmodetext(Gdiplus::Color(255, 255, 255, 255));
        FontFamily      fontFamily(L"Segoe UI Variable Small");
        Gdiplus::Font   font(&fontFamily, 9);
        graphics.DrawString(MsWin, -1, &font, PointF(35, 110), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
        graphics.DrawString(Version, -1, &font, PointF(35, 128), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
        graphics.DrawString(CopyRight, -1, &font, PointF(35, 146), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
        RectF        rectF(35, 180, 385, 70);
        graphics.DrawString(AboutWin, -1, &font, rectF, NULL, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
        RectF        imgrectF(65, 10, 305, 90);
        Gdiplus::Bitmap* pBmp = LoadImageFromResource(hInst, MAKEINTRESOURCE(IDB_PNG2), L"PNG");
        graphics.DrawImage(pBmp, imgrectF);
        RectF        arectF(35, 250, 345, 90);
		graphics.DrawString(EulaText, -1, &font, arectF, NULL, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
		graphics.DrawString(Owner, -1, &font, PointF(50, 285), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
		graphics.DrawString(Organization, -1, &font, PointF(50, 303), DarkThemeEnabled ? &darkmodetext : &lightmodetext);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
