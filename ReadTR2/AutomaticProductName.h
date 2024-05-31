#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"ReadTR2 (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"ReadTR2 - Version 4.1.11.003\r\n(Build 378) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "ReadTR2 (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.1.11.003\r\n(Build 378) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"ReadTR2 (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"ReadTR2 - Version 4.1.11.003\r\n(Build 378) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "ReadTR2 (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.1.11.003\r\n(Build 378) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "ReadTR2 (MSVC)\0"
#define PRODUCT_NAME        "ReadTR2 - Version 4.1.11.003\r\n(Build 378) - (MSVC)\0"
#endif

#endif
