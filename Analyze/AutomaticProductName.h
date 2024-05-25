#pragma once

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"Analyze (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"Analyze - Version 4.1.10.001\r\n(Build 633) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "Analyze (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.1.10.001\r\n(Build 633) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"Analyze (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"Analyze - Version 4.1.10.001\r\n(Build 633) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "Analyze (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.1.10.001\r\n(Build 633) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "Analyze (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.1.10.001\r\n(Build 633) - (MSVC)\0"
#endif
