#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"Analyze (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"Analyze - Version 4.5.03.001\r\n(Build 962) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "Analyze (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.5.03.001\r\n(Build 962) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"Analyze (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"Analyze - Version 4.5.03.001\r\n(Build 962) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "Analyze (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.5.03.001\r\n(Build 962) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "Analyze (MSVC)\0"
#define PRODUCT_NAME        "Analyze - Version 4.5.03.001\r\n(Build 962) - (MSVC)\0"
#endif

#endif
