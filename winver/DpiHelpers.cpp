#include "DpiHelpers.h"

void UpdateEulaRichEdtLayoutForDpi(HWND hwnd)
{
	int iDpi = ::GetDpiForWindow(hwnd);
	int dpiScaledX = MulDiv(10, iDpi, 96);
	int dpiScaledY = MulDiv(15, iDpi, 96);
	int dpiScaledWidth = MulDiv(585, iDpi, 96);
	int dpiScaledHeight = MulDiv(295, iDpi, 96);
	SetWindowPos(hwnd, hwnd, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
}
void UpdateEulaButtonLayoutForDpi(HWND hWnd)
{
	int iDpi = ::GetDpiForWindow(hWnd);
	int dpiScaledX = MulDiv(525, iDpi, 96);
	int dpiScaledY = MulDiv(320, iDpi, 96);
	int dpiScaledWidth = MulDiv(70, iDpi, 96);
	int dpiScaledHeight = MulDiv(23, iDpi, 96);
	SetWindowPos(hWnd, hWnd, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
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

void UpdateButtonLayoutForDpi(HWND hWnd, int ButtonX, int ButtonY)
{
	int iDpi = ::GetDpiForWindow(hWnd);
	int dpiScaledX = MulDiv(ButtonX, iDpi, 96);
	int dpiScaledY = MulDiv(ButtonY, iDpi, 96);
	int dpiScaledWidth = MulDiv(70, iDpi, 96);
	int dpiScaledHeight = MulDiv(23, iDpi, 96);
	SetWindowPos(hWnd, hWnd, dpiScaledX, dpiScaledY, dpiScaledWidth, dpiScaledHeight, SWP_NOZORDER | SWP_NOACTIVATE);
}
void UpdateEulaLayoutForDpi(HWND hWnd, int EulaY, int EulaWidth)
{
	int iDpi = ::GetDpiForWindow(hWnd);
	int dpiScaledX = MulDiv(47, iDpi, 96);
	int dpiScaledY = MulDiv(EulaY, iDpi, 96);
	int dpiScaledWidth = MulDiv(EulaWidth, iDpi, 96);
	int dpiScaledHeight = MulDiv(40, iDpi, 96);
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