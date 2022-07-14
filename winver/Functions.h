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

// Detects whether system is using dark mode or not.
int IsExplorerDarkTheme();

// Sets string values about system information.
void DoStuff(HINSTANCE hInst);

// Same as DoStuff but I need this otherwise registry is giving garbage values.
void DoStuffv2();

// Applies mica to a window.
BOOL ApplyMica(HWND hwnd);

// Sets dark titlebar.
BOOL DarkTitleBar(HWND hWnd);

// Loads a bitmap from a resource id.
Gdiplus::Bitmap* LoadImageFromResource(HMODULE hMod, const wchar_t* resid, const wchar_t* restype);

// Draws the text using GDI+.
BOOLEAN DrawStrings(HWND hWnd, Gdiplus::Graphics& graphics);

// Draws the bitmap using GDI+.
BOOLEAN DrawLogo(HWND hwnd, Gdiplus::Graphics& graphics, HINSTANCE hInst);

// Creates the hyperlink window and the OK button.
BOOLEAN CreateHwnds(HWND hWnd, HINSTANCE hInst);

// Creates the richedit and loads the license file.
void SetupRichEdit(HWND hwndEdit, HWND hDlg, HINSTANCE hInst);

// Uses the BrandingFormatString function from winbrand.dll to get some texts.
BOOL GetwinBrandName();

// Custom draw button to fix bug in light mode (not needed anymore ig).
BOOL CustomDrawButton(LPARAM lParam, HWND hWnd);

// Loads dark scrollbar if system is using dark mode.
void FixDarkScrollBar();

void DarkModeHandler(WPARAM wParam);
BOOLEAN DrawE(HWND hWnd, Gdiplus::Graphics& graphics);
