#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"ReadTR2 (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"ReadTR2 - Version 4.3.06.004\r\n(Build 594) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "ReadTR2 (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.3.06.004\r\n(Build 594) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"ReadTR2 (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"ReadTR2 - Version 4.3.06.004\r\n(Build 594) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "ReadTR2 (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.3.06.004\r\n(Build 594) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "ReadTR2 (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.3.06.004\r\n(Build 594) - (MSVC)\0"
#endif

#endif
