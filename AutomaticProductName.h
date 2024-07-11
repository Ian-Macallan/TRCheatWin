#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.2.06.012\r\n(Build 900) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.2.06.012\r\n(Build 900) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.2.06.012\r\n(Build 900) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.2.06.012\r\n(Build 900) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "TRCheatWin (MSVC)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.2.06.012\r\n(Build 900) - (MSVC)\0"
#endif

#endif
