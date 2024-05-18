#ifdef _WIN64
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"TRCheatWin (x64 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"TRCheatWin - Version 4.1.07.009\r\n(Build 719) - (x64 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"TRCheatWin (x64 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"TRCheatWin - Version 4.1.07.009\r\n(Build 719) - (x64 MBCS) (VC11.0/u5)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"TRCheatWin (x86 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"TRCheatWin - Version 4.1.07.009\r\n(Build 719) - (x86 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"TRCheatWin (x86 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"TRCheatWin - Version 4.1.07.009\r\n(Build 719) - (x86 MBCS) (VC11.0/u5)\0"
#endif
#else
#define	ORIGINAL_FILENAME	"TRCheatWin (VC11.0/u5)\0"
#define	PRODUCT_NAME		"TRCheatWin - Version 4.1.07.009\r\n(Build 719) - (VC11.0/u5)\0"
#endif
