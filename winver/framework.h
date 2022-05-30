// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <sstream>
#include <string>
#include <comdef.h>
#include <dwmapi.h>
#include <Uxtheme.h>
#include <assert.h>
#include <gdiplus.h>
#include <atlstr.h>
#pragma comment (lib, "gdiplus.lib")
# pragma comment(lib, "uxtheme.lib")
# pragma comment(lib, "dwmapi.lib")

