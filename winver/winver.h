#pragma once

#include "resource.h"

// change the BUILD_R11 value in resource.h if you want to build for Rectify11 or not.

#define MAX_LOADSTRING 100

extern LPCTSTR okef;
extern HKEY hKeyPersonalization;
extern LPCWSTR MsWin;
extern LPCWSTR AboutWin;
extern std::wstring Version;
extern LPCWSTR CopyRight;
extern LPCWSTR Owner;
extern LPCWSTR OkButton;
extern OSVERSIONINFOEX os;
extern LPCWSTR Organization;
extern int DarkThemeEnabled;
extern HWND yes;
extern HWND button;
extern int CopyWidth;
extern int OwnerY;
extern int OrganizationY;
extern int BitmapX;
extern int CopyY;
extern BOOL dpiChanged;

enum IMMERSIVE_HC_CACHE_MODE
{
	IHCM_USE_CACHED_VALUE,
	IHCM_REFRESH
};
enum class PreferredAppMode
{
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};

// 1809 17763
using fnShouldAppsUseDarkMode = bool (WINAPI*)(); // ordinal 132
using fnAllowDarkModeForWindow = bool (WINAPI*)(HWND hWnd, bool allow); // ordinal 133
using fnFlushMenuThemes = void (WINAPI*)(); // ordinal 136
using fnRefreshImmersiveColorPolicyState = void (WINAPI*)(); // ordinal 104
using fnIsDarkModeAllowedForWindow = bool (WINAPI*)(HWND hWnd); // ordinal 137
using fnGetIsImmersiveColorUsingHighContrast = bool (WINAPI*)(IMMERSIVE_HC_CACHE_MODE mode); // ordinal 106
using fnOpenNcThemeData = HTHEME(WINAPI*)(HWND hWnd, LPCWSTR pszClassList); // ordinal 49
// 1903 18362
using fnShouldSystemUseDarkMode = bool (WINAPI*)(); // ordinal 138
using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode); // ordinal 135, in 1903
using fnIsDarkModeAllowedForApp = bool (WINAPI*)(); // ordinal 

extern fnAllowDarkModeForWindow AllowDarkModeForWindow;
extern fnOpenNcThemeData OpenNcThemeData;

// Dark mode text and background color
constexpr COLORREF darkBkColor = 0x010000;
constexpr COLORREF darkTextColor = 0xFFFFFF;

constexpr COLORREF lightBkColor = 0xFFFFFF;
constexpr COLORREF lightTextColor = 0x000000;
static HBRUSH hbrBkgnd = nullptr;
