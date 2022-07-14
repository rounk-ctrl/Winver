#pragma once
#include "framework.h"

// Updates window size when DPI changes.
void UpdateWindowSize(HWND hWnd, LPARAM lParam);

// Scales the main window depending on DPI.
void ScaleWindow(HWND hWnd, int Window_Width, int Window_Height);

// Sets Segoe UI Variable font.
BOOLEAN FixFont(HWND hWnd, HWND yes);

// Scales the dialog depending on DPI
void ScaleDialog(HWND hDlg);

// Updates the layout of a control depending on DPI.
void UpdateLayoutForDpi(HWND hWnd, int x, int y, int width, int height);

// Scales a PointF depending on DPI.
Gdiplus::PointF ScaledPointF(Gdiplus::PointF o, int currentMonitorDpi);

// Scales a RectF depending on DPI.
Gdiplus::RectF ScaledRectF(Gdiplus::RectF o, int currentMonitorDpi);
