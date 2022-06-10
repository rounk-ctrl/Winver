// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <sstream>
#include <string>
#include <comdef.h>
#include <dwmapi.h>
#include <Uxtheme.h>
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <shellscalingapi.h>
#include <gdiplus.h>
#include <richedit.h>
#include <shellapi.h>
#include <fstream>
#include <atlstr.h>
#pragma comment (lib, "gdiplus.lib")
# pragma comment(lib, "uxtheme.lib")
# pragma comment(lib, "shcore.lib")
#pragma comment(lib, "comctl32.lib")
# pragma comment(lib, "dwmapi.lib")

