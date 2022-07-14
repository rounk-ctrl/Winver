#include "DpiHelpers.h"
using namespace Gdiplus;

BOOLEAN FixFont(HWND hWnd, HWND yes)
{
	int pointSize = 16;
	int height = MulDiv(pointSize, GetDpiForWindow(hWnd), 96);
	HFONT hFont = CreateFont(height, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI Variable Small");
	SendMessage(yes, WM_SETFONT, (LPARAM)hFont, true);
	return TRUE;
}

void UpdateWindowSize(HWND hWnd, LPARAM lParam)
{
	auto rect = *reinterpret_cast<RECT *>(lParam);
	SetWindowPos(hWnd,
		0, // No relative window
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void ScaleDialog(HWND hDlg)
{
	UINT dpi = GetDpiForWindow(hDlg);
	float scaling_factor = static_cast<float>(dpi) / 96;
	RECT scaled_size;
	scaled_size.left = 0;
	scaled_size.top = 0;
	scaled_size.right = static_cast<LONG>(605 * scaling_factor);
	scaled_size.bottom = static_cast<LONG>(355 * scaling_factor);
	AdjustWindowRectExForDpi(&scaled_size, WS_OVERLAPPEDWINDOW, false, 0, dpi);
	SetWindowPos(hDlg, nullptr, CW_USEDEFAULT, CW_USEDEFAULT, scaled_size.right - scaled_size.left, scaled_size.bottom - scaled_size.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
}

void UpdateLayoutForDpi(HWND hWnd, int x, int y, int width, int height)
{
	int iDpi = GetDpiForWindow(hWnd);
	int dpiScaledX = MulDiv(x, iDpi, 96);
	int dpiScaledY = MulDiv(y, iDpi, 96);
	int dpiScaledWidth = MulDiv(width, iDpi, 96);
	int dpiScaledHeight = MulDiv(height, iDpi, 96);
	SetWindowPos(hWnd, hWnd, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
}

void ScaleWindow(HWND hWnd, int Window_Width, int Window_Height)
{
	UINT dpi = GetDpiForWindow(hWnd);
	float scaling_factor = static_cast<float>(dpi) / 96;
	RECT scaled_size;
	scaled_size.left = 0;
	scaled_size.top = 0;
	scaled_size.right = static_cast<LONG>(Window_Width * scaling_factor);
	scaled_size.bottom = static_cast<LONG>(Window_Height * scaling_factor);
	AdjustWindowRectExForDpi(&scaled_size, WS_OVERLAPPEDWINDOW, false, 0, dpi);
	SetWindowPos(hWnd, nullptr, CW_USEDEFAULT, CW_USEDEFAULT, scaled_size.right - scaled_size.left, scaled_size.bottom - scaled_size.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
}

PointF ScaledPointF(PointF o, int currentMonitorDpi)
{
	int iDpi = currentMonitorDpi;
	REAL X = MulDiv(o.X, iDpi, 96);
	REAL Y = MulDiv(o.Y, iDpi, 96);
	return PointF(X, Y);
}

RectF ScaledRectF(RectF o, int currentMonitorDpi)
{
	int iDpi = currentMonitorDpi;
	REAL X = MulDiv(o.X, iDpi, 96);
	REAL Y = MulDiv(o.Y, iDpi, 96);
	REAL width = MulDiv(o.Width, iDpi, 96);
	REAL height = MulDiv(o.Height, iDpi, 96);
	return RectF(X, Y, width, height);
}