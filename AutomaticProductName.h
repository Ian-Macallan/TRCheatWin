#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x64 Unicode) (VC11.0/u5)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.1.09.011\r\n(Build 759) - (x64 Unicode) (VC11.0/u5)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x64 MBCS) (VC11.0/u5)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.1.09.011\r\n(Build 759) - (x64 MBCS) (VC11.0/u5)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"TRCheatWin (x86 Unicode) (VC11.0/u5)\0"
#define PRODUCT_NAME        L"TRCheatWin - Version 4.1.09.011\r\n(Build 759) - (x86 Unicode) (VC11.0/u5)\0"
#else
#define ORIGINAL_FILENAME   "TRCheatWin (x86 MBCS) (VC11.0/u5)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.1.09.011\r\n(Build 759) - (x86 MBCS) (VC11.0/u5)\0"
#endif
#else
#define ORIGINAL_FILENAME   "TRCheatWin (VC11.0/u5)\0"
#define PRODUCT_NAME        "TRCheatWin - Version 4.1.09.011\r\n(Build 759) - (VC11.0/u5)\0"
#endif
