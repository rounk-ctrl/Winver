#pragma once
#include "framework.h"
#include "winver.h"

#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#define DWMWA_MICA_EFFECT 1029
#define DWMWA_SYSTEMBACKDROP_TYPE 38

enum class SystemBackdropType
{
	Auto,
	None,
	Mica,
	Acrylic,
	Tabbed
};
typedef void (WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);

int IsExplorerDarkTheme();
void DoStuff(HINSTANCE hInst);
BOOL ApplyMica(HWND hwnd);
Gdiplus::Bitmap* LoadImageFromResource(HMODULE hMod, const wchar_t* resid, const wchar_t* restype);
void DoStuffv2();
BOOL DarkTitleBar(HWND hWnd);
BOOLEAN DrawStrings(HWND hWnd, Gdiplus::Graphics& graphics);
BOOLEAN DrawLogo(HWND hwnd, Gdiplus::Graphics& graphics, HINSTANCE hInst);
BOOLEAN CreateHwnds(HWND hWnd, HINSTANCE hInst);
void SetupRichEdit(HWND hwndEdit, HWND hDlg, HINSTANCE hInst);
BOOL GetwinBrandName();
BOOL CustomDrawButton(LPARAM lParam, HWND hWnd);
void FixDarkScrollBar();
BOOLEAN DrawAbout(HWND hWnd, Gdiplus::Graphics& graphics);
