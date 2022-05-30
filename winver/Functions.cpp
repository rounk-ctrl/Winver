#include "Functions.h"

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

    hMod = LoadLibrary(TEXT("ntdll.dll"));
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
LONG GetStringRegKey(HKEY hKey, const std::wstring &strValueName, std::wstring &strValue, const std::wstring &strDefaultValue)
{
	strValue = strDefaultValue;
	WCHAR szBuffer[512];
	DWORD dwBufferSize = sizeof(szBuffer);
	ULONG nError;
	nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
	if (ERROR_SUCCESS == nError)
	{
		strValue = szBuffer;
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
			std::ostringstream ver;
			ver << verstr;
			Owner = convertCharArrayToLPCWSTR(ver.str().c_str());
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

void DoStuff()
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
    std::ostringstream ver;
    ver << "Version ";
    _bstr_t verstr(version);
    ver << verstr;
    ver << " (OS Build ";
    char buildno[10];
    sprintf_s(buildno, "%d", os.dwBuildNumber);
    ver <<  buildno;
    ver << ".";
    std::ostringstream os;
    os << UBR;
    ver << os.str().c_str();
    ver << ")";
    Version = convertCharArrayToLPCWSTR(ver.str().c_str());
    EulaText = L"This product is licensed under the Microsoft Software Licence Terms to: ";
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
	if (DarkThemeEnabled)
	{
		if ((os.dwBuildNumber < 18362) && (os.dwBuildNumber >= 17763))
		{
			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(hWnd, &margins);
			SetPropW(hWnd, L"UseImmersiveDarkModeColors", reinterpret_cast<HANDLE>(static_cast<INT_PTR>(DarkThemeEnabled)));
			return TRUE;
		}
		else if ((os.dwBuildNumber >= 18362) && (os.dwBuildNumber < 19041))
		{
			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(hWnd, &margins);
			DwmSetWindowAttribute(hWnd, 19, &DarkThemeEnabled, sizeof DarkThemeEnabled);
			return TRUE;
		}
		else if (os.dwBuildNumber >= 19041)
		{
			MARGINS margins = { -1 };
			DwmExtendFrameIntoClientArea(hWnd, &margins);
			DwmSetWindowAttribute(hWnd, 20, &DarkThemeEnabled, sizeof DarkThemeEnabled);
			return TRUE;
		}
	}
	return FALSE;
}
