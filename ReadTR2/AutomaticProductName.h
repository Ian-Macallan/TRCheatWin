#ifdef _WIN64
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"ReadTR2 (x64 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"ReadTR2 - Version 4.1.08.016\r\n(Build 334) - (x64 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"ReadTR2 (x64 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"ReadTR2 - Version 4.1.08.016\r\n(Build 334) - (x64 MBCS) (VC11.0/u5)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"ReadTR2 (x86 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"ReadTR2 - Version 4.1.08.016\r\n(Build 334) - (x86 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"ReadTR2 (x86 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"ReadTR2 - Version 4.1.08.016\r\n(Build 334) - (x86 MBCS) (VC11.0/u5)\0"
#endif
#else
#define	ORIGINAL_FILENAME	"ReadTR2 (VC11.0/u5)\0"
#define	PRODUCT_NAME		"ReadTR2 - Version 4.1.08.016\r\n(Build 334) - (VC11.0/u5)\0"
#endif
