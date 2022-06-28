#pragma once
#include "framework.h"

void UpdateWindowSize(HWND hWnd, LPARAM lParam);
BOOLEAN FixFont(HWND hWnd, HWND yes);
void ScaleDialog(HWND hDlg);
void UpdateLayoutForDpi(HWND hWnd, int x, int y, int width, int height);
void ScaleWindow(HWND hWnd, int Window_Width, int Window_Height);
Gdiplus::PointF ScaledPointF(Gdiplus::PointF o, int currentMonitorDpi);
Gdiplus::RectF ScaledRectF(Gdiplus::RectF o, int currentMonitorDpi);
