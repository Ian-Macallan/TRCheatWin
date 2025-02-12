#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.5.05.002\r\n(Build 1108) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.5.05.002\r\n(Build 1108) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.5.05.002\r\n(Build 1108) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.5.05.002\r\n(Build 1108) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "TRCheatWin (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.5.05.002\r\n(Build 1108) - (MSVC)\0"
#endif

#endif
