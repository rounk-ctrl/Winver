#include "Functions.h"
#include "IatHook.h"
#include "DpiHelpers.h"
using namespace Gdiplus;
int dpi;

wchar_t* convertCharArrayToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[200];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 200);
	return wString;
	wString = NULL;
	delete[] wString;
}

BOOL RtlGetVersion(OSVERSIONINFOEX* os) {
    HMODULE hMod;
    RtlGetVersion_FUNC func;
#ifdef UNICODE
    OSVERSIONINFOEXW* osw = os;
#else
    OSVERSIONINFOEXW o;
    OSVERSIONINFOEXW* osw = &o;
#endif

    hMod = LoadLibraryExW(L"ntdll.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hMod) {
        func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
        if (func == 0) {
            FreeLibrary(hMod);
            return FALSE;
        }
        ZeroMemory(osw, sizeof(*osw));
        osw->dwOSVersionInfoSize = sizeof(*osw);
        func(osw);
#ifndef UNICODE
        os->dwBuildNumber = osw->dwBuildNumber;
        os->dwMajorVersion = osw->dwMajorVersion;
        os->dwMinorVersion = osw->dwMinorVersion;
        os->dwPlatformId = osw->dwPlatformId;
        os->dwOSVersionInfoSize = sizeof(*os);
        DWORD sz = sizeof(os->szCSDVersion);
        WCHAR* src = osw->szCSDVersion;
        unsigned char* dtc = (unsigned char*)os->szCSDVersion;
        while (*src)
            *Dtc++ = (unsigned char)*src++;
        *Dtc = '\ 0';
#endif
    }
    else
        return FALSE;
    FreeLibrary(hMod);
    return TRUE;
}

int IsExplorerDarkTheme()
{
    RegOpenKeyEx(
        HKEY_CURRENT_USER,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0, KEY_READ, &hKeyPersonalization
    );
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = RegQueryValueEx(
        hKeyPersonalization,
        L"AppsUseLightTheme",
        0,
        NULL,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize
    );
    RegCloseKey(hKeyPersonalization);
    return ERROR_SUCCESS == nError ? !nResult : FALSE;
}

LONG GetDWORDRegKey(HKEY hKey, const std::wstring& strValueName, DWORD& nValue)
{
    DWORD dwBufferSize(sizeof(DWORD));
    DWORD nResult(0);
    LONG nError = ::RegQueryValueExW(hKey,
        strValueName.c_str(),
        0,
        NULL,
        reinterpret_cast<LPBYTE>(&nResult),
        &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        nValue = nResult;
    }
    return nError;
}

void DoStuffv2()
{
	HKEY hKey;
	wchar_t version[MAX_LOADSTRING];
	wchar_t versione[MAX_LOADSTRING];
	DWORD UBR{};
	DWORD type = REG_SZ, size = MAX_LOADSTRING;
	RegOpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", &hKey);
	if (hKey)
	{
		int error = RegQueryValueEx(hKey, L"RegisteredOwner", NULL, &type, (LPBYTE)&version, &size);
		if (error == ERROR_SUCCESS)
		{
			_bstr_t verstr(version);
			std::ostringstream verstring;
			verstring << verstr;
			Owner = convertCharArrayToLPCWSTR(verstring.str().c_str());
		}
		else
		{
			Owner = L"user name";
		}
		int erro2 = RegQueryValueEx(hKey, L"RegisteredOrganization", NULL, &type, (LPBYTE)&versione, &size);
		if (erro2 == ERROR_SUCCESS)
		{
			_bstr_t verstr9(versione);
			std::ostringstream ve2r;
			ve2r << verstr9;
			Organization = convertCharArrayToLPCWSTR(ve2r.str().c_str());
		}
		else
		{
			Organization = L"org name";
		}

		RegCloseKey(hKey);
	}
}

void DoStuff(HINSTANCE hInst)
{
    HKEY hKey;
	wchar_t version[MAX_LOADSTRING];
    DWORD UBR{};
	DWORD type = REG_SZ, size = MAX_LOADSTRING;
	RegOpenKey(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion", &hKey);
	if (hKey)
	{
		int error = RegQueryValueEx(hKey, L"DisplayVersion", NULL, &type, (LPBYTE)&version, &size);
		if (error != ERROR_SUCCESS)
		{
			error = RegQueryValueExW(hKey, L"ReleaseId", NULL, &type, (LPBYTE)&version, &size);
		}
		GetDWORDRegKey(hKey, L"UBR", UBR);
		RegCloseKey(hKey);
	}
    RtlGetVersion(&os);
    std::wostringstream ver;
	CString versionstr(MAKEINTRESOURCE(IDS_TEXT_VERSION));
	CString buildstr(MAKEINTRESOURCE(IDS_TEXT_BUILD));
    ver << (LPCWSTR)versionstr;
	_bstr_t verstr(version);
    ver << version;
    ver << (LPCWSTR)buildstr;
    ver << os.dwBuildNumber;
    ver << L".";
    ver << UBR;
    ver << L")";
    Version = ver.str();
}

Gdiplus::Bitmap* LoadImageFromResource(HMODULE hMod, const wchar_t* resid, const wchar_t* restype)
{
    IStream* pStream = nullptr;
    Gdiplus::Bitmap* pBmp = nullptr;
    HGLOBAL hGlobal = nullptr;

    HRSRC hrsrc = FindResourceW(hMod, resid, restype);     // get the handle to the resource
    if (hrsrc)
    {
        DWORD dwResourceSize = SizeofResource(hMod, hrsrc);
        if (dwResourceSize > 0)
        {
            HGLOBAL hGlobalResource = LoadResource(hMod, hrsrc); // load it
            if (hGlobalResource)
            {
                void* imagebytes = LockResource(hGlobalResource); // get a pointer to the file bytes

                // copy image bytes into a real hglobal memory handle
                hGlobal = ::GlobalAlloc(GHND, dwResourceSize);
                if (hGlobal)
                {
                    void* pBuffer = ::GlobalLock(hGlobal);
                    if (pBuffer)
                    {
                        memcpy(pBuffer, imagebytes, dwResourceSize);
                        HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
                        if (SUCCEEDED(hr))
                        {
                            // pStream now owns the global handle and will invoke GlobalFree on release
                            hGlobal = nullptr;
                            pBmp = new Gdiplus::Bitmap(pStream);
                        }
                    }
                }
            }
        }
    }

    if (pStream)
    {
        pStream->Release();
        pStream = nullptr;
    }

    if (hGlobal)
    {
        GlobalFree(hGlobal);
        hGlobal = nullptr;
    }

    return pBmp;
}

void SetSystemBackdropType(HWND hwnd, SystemBackdropType value)
{
    DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &value, sizeof value);
}

BOOL CobaltMicaEffect(HWND hwnd, BOOL allow)
{
    DwmSetWindowAttribute(hwnd, DWMWA_MICA_EFFECT, &allow, sizeof allow);
    return TRUE;
}

BOOL ApplyMica(HWND hwnd)
{
    if (os.dwBuildNumber == 22000)
    {
        CobaltMicaEffect(hwnd, TRUE);
    }
    else if (os.dwBuildNumber >= 22523)
    {
        SetSystemBackdropType(hwnd, SystemBackdropType::Mica);
    }
    return TRUE;
}

BOOL DarkTitleBar(HWND hWnd)
{
	if (os.dwBuildNumber < 17763)
	{
		return TRUE;
	}
	if (DarkThemeEnabled)
	{
		MARGINS margins = { -1 };
		DwmExtendFrameIntoClientArea(hWnd, &margins);
		if ((os.dwBuildNumber < 18362) && (os.dwBuildNumber >= 17763))
		{
			SetPropW(hWnd, L"UseImmersiveDarkModeColors", reinterpret_cast<HANDLE>(static_cast<INT_PTR>(DarkThemeEnabled)));
			return TRUE;
		}
		else if ((os.dwBuildNumber >= 18362) && (os.dwBuildNumber < 19041))
		{
			DwmSetWindowAttribute(hWnd, 19, &DarkThemeEnabled, sizeof DarkThemeEnabled);
			return TRUE;
		}
		else if (os.dwBuildNumber >= 19041)
		{
			DwmSetWindowAttribute(hWnd, 20, &DarkThemeEnabled, sizeof DarkThemeEnabled);
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN DrawStrings(HWND hWnd, Graphics& graphics)
{
	Gdiplus::SolidBrush      lightmodetext(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::SolidBrush      darkmodetext(Gdiplus::Color(255, 255, 255, 255));
	dpi = GetDpiForWindow(hWnd);
	Gdiplus::REAL emSize = 9.0;
	FontFamily      fontFamily(L"Segoe UI Variable Small");
	Gdiplus::Font   font(&fontFamily, emSize);
	
	graphics.DrawString(MsWin, -1, &font, ScaledPointF(PointF(45, 100), dpi), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
	graphics.DrawString(Version.c_str(), -1, &font, ScaledPointF(PointF(45, 118), dpi), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
	graphics.DrawString(CopyRight, -1, &font, ScaledRectF(RectF(45, 136, 385, 40), dpi), NULL, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
	graphics.DrawString(AboutWin, -1, &font, ScaledRectF(RectF(45, CopyY, CopyWidth, 80), dpi), NULL, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
	graphics.DrawString(Owner, -1, &font, ScaledPointF(PointF(60, OwnerY), dpi), DarkThemeEnabled ? &darkmodetext : &lightmodetext);
	graphics.DrawString(Organization, -1, &font, ScaledPointF(PointF(60, OrganizationY), dpi), DarkThemeEnabled ? &darkmodetext : &lightmodetext);

	//clean up
	DeleteObject(&font);
	DeleteObject(&fontFamily);
	DeleteObject(&emSize);
	DeleteObject(&lightmodetext);
	DeleteObject(&darkmodetext);
	return TRUE;
}

BOOLEAN DrawAbout(HWND hWnd, Graphics& graphics)
{
	Gdiplus::SolidBrush      lightmodetext(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::SolidBrush      darkmodetext(Gdiplus::Color(255, 255, 255, 255));
	dpi = GetDpiForWindow(hWnd);
	Gdiplus::REAL emSize = 10.0;
	FontFamily      fontFamily(L"Segoe UI Variable Small");
	Gdiplus::Font   font(&fontFamily, emSize);

	graphics.DrawString(L"Hi, this page is a placeholder for now :)", -1, &font, ScaledPointF(PointF(60, 150), dpi), DarkThemeEnabled ? &darkmodetext : &lightmodetext);

	//clean up
	DeleteObject(&font);
	DeleteObject(&fontFamily);
	DeleteObject(&emSize);
	DeleteObject(&lightmodetext);
	DeleteObject(&darkmodetext);
	return TRUE;
}

BOOLEAN DrawLogo(HWND hwnd, Graphics& graphics, HINSTANCE hInst)
{
	Gdiplus::Bitmap* pBmp = LoadImageFromResource(hInst, MAKEINTRESOURCE(IDB_R11), L"PNG");
	RectF        imgrectF(BitmapX, 15, 350, 67);
	graphics.DrawImage(pBmp, ScaledRectF(imgrectF, dpi));

	DeleteObject(&imgrectF);
	DeleteObject(&pBmp);
	return TRUE;
}

BOOLEAN CreateHwnds(HWND hWnd, HINSTANCE hInst)
{
	button = CreateWindow(L"button", L"", WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)199, hInst, NULL);
	SetWindowTheme(button, L"Explorer", nullptr);
	AllowDarkModeForWindow(button, true);
	SendMessageW(button, WM_THEMECHANGED, 0, 0);
	CString eulatxt(MAKEINTRESOURCE(IDS_TEXT_EULA));
	yes = CreateWindowEx(WS_EX_COMPOSITED, WC_LINK, eulatxt, WS_VISIBLE | WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, hWnd, (HMENU)200, hInst, NULL);
	FixFont(hWnd, yes);
	UpdateWindow(hWnd);
	return TRUE;
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

void SetTxtColor(HWND hWindow, COLORREF clr) {
	CHARFORMAT cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = clr;
	cf.dwEffects = 0;
	SendMessage(hWindow, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);
}

void SetupRichEdit(HWND hwndEdit, HWND hDlg, HINSTANCE hInst)
{
	hwndEdit = CreateWindowEx(WS_EX_COMPOSITED, MSFTEDIT_CLASS, TEXT(""),
		ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_TABSTOP | ES_AUTOVSCROLL | WS_VSCROLL,
		0, 0, 0, 0,
		hDlg, (HMENU)230, hInst, NULL);
	UpdateLayoutForDpi(hwndEdit, 10, 15, 580, 280);
	SendMessage(hwndEdit, EM_SETREADONLY, TRUE, 0);
	FillRichEditFromFile(hwndEdit, L"C:\\Windows\\System32\\license.rtf");
	if (DarkThemeEnabled)
	{
		SendMessage(hwndEdit, EM_SETBKGNDCOLOR, 0, darkBkColor);
		SetTxtColor(hwndEdit, darkTextColor);
	}
}

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

			const wchar_t copy[50] = L"%WINDOWS_COPYRIGHT%";
			CopyRight = getwinName(copy);

			const wchar_t msw[50] = L"%MICROSOFT_COMPANYNAME% %WINDOWS_GENERIC%";
			MsWin = getwinName(msw);
			BRet = TRUE;
		}
		FreeLibrary(hModNtdll);
		hModNtdll = NULL;
	}
	else
	{
		MessageBox(0, L"winbrand.dll not found.", L"Error", MB_ICONERROR);
	}
	return BRet;
}

BOOL CustomDrawButton(LPARAM lParam, HWND hWnd)
{
	if (((LPNMHDR)lParam)->idFrom == 199)
	{
		LPNMHDR some_item = (LPNMHDR)lParam;
		LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;
		SetBkMode(item->hdc, TRANSPARENT);
		Gdiplus::Graphics graphicshdc(item->hdc);
		Gdiplus::SolidBrush      lightmodetext(Gdiplus::Color(255, 0, 0, 0));
		Gdiplus::SolidBrush      darkmodetext(Gdiplus::Color(255, 255, 255, 255));
		StringFormat stringFormat;
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
		Gdiplus::REAL emSize = 9.0;
		FontFamily      fontFamily(L"Segoe UI Variable Small");
		Gdiplus::Font   font(&fontFamily, emSize);
		if (item->uItemState & CDIS_SELECTED)
		{
			graphicshdc.DrawString(OkButton, -1, &font, RectF(item->rc.left, item->rc.top, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top), &stringFormat, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
			DeleteObject(&font);
			DeleteObject(&fontFamily);
			DeleteObject(&emSize);
			DeleteObject(&lightmodetext);
			DeleteObject(&darkmodetext);
			return CDRF_DODEFAULT;
		}
		else
		{
			if (item->uItemState & CDIS_HOT) //Our mouse is over the button
			{
				graphicshdc.DrawString(OkButton, -1, &font, RectF(item->rc.left, item->rc.top, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top), &stringFormat, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
				DeleteObject(&font);
				DeleteObject(&fontFamily);
				DeleteObject(&emSize);
				DeleteObject(&lightmodetext);
				DeleteObject(&darkmodetext);
				return CDRF_DODEFAULT;
			}
			graphicshdc.DrawString(OkButton, -1, &font, RectF(item->rc.left, item->rc.top, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top), &stringFormat, DarkThemeEnabled ? &darkmodetext : &lightmodetext);
			DeleteObject(&font);
			DeleteObject(&fontFamily);
			DeleteObject(&emSize);
			DeleteObject(&lightmodetext);
			DeleteObject(&darkmodetext);
			return CDRF_DODEFAULT;
		}
	}
	return CDRF_DODEFAULT;
}

void FixDarkScrollBar()
{
	HMODULE hComctl = LoadLibraryExW(L"comctl32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hComctl)
	{
		auto addr = FindDelayLoadThunkInModule(hComctl, "uxtheme.dll", 49); // OpenNcThemeData
		if (addr)
		{
			DWORD oldProtect;
			if (VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), PAGE_READWRITE, &oldProtect))
			{
				auto MyOpenThemeData = [](HWND hWnd, LPCWSTR classList) -> HTHEME {
					if (wcscmp(classList, L"ScrollBar") == 0)
					{
						hWnd = nullptr;
						classList = L"Explorer::ScrollBar";
					}
					return OpenNcThemeData(hWnd, classList);
				};
				addr->u1.Function = reinterpret_cast<ULONG_PTR>(static_cast<fnOpenNcThemeData>(MyOpenThemeData));
				VirtualProtect(addr, sizeof(IMAGE_THUNK_DATA), oldProtect, &oldProtect);
			}
		}
	}
}
