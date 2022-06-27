#pragma once
#include "framework.h"
#include "winver.h"

void UpdateEulaRichEdtLayoutForDpi(HWND hwnd);
void UpdateEulaButtonLayoutForDpi(HWND hWnd);
void ScaleDialog(HWND hDlg);
void UpdateButtonLayoutForDpi(HWND hWnd, int ButtonX, int ButtonY);
void UpdateEulaLayoutForDpi(HWND hWnd, int EulaY, int EulaWidth);
void ScaleWindow(HWND hWnd, int Window_Width, int Window_Height);
