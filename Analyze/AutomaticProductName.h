#ifdef _WIN64
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"Analyze (x64 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"Analyze - Version 4.1.07.005\r\n(Build 580) - (x64 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"Analyze (x64 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"Analyze - Version 4.1.07.005\r\n(Build 580) - (x64 MBCS) (VC11.0/u5)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define	ORIGINAL_FILENAME	L"Analyze (x86 Unicode) (VC11.0/u5)\0"
#define	PRODUCT_NAME		L"Analyze - Version 4.1.07.005\r\n(Build 580) - (x86 Unicode) (VC11.0/u5)\0"
#else
#define	ORIGINAL_FILENAME	"Analyze (x86 MBCS) (VC11.0/u5)\0"
#define	PRODUCT_NAME		"Analyze - Version 4.1.07.005\r\n(Build 580) - (x86 MBCS) (VC11.0/u5)\0"
#endif
#else
#define	ORIGINAL_FILENAME	"Analyze (VC11.0/u5)\0"
#define	PRODUCT_NAME		"Analyze - Version 4.1.07.005\r\n(Build 580) - (VC11.0/u5)\0"
#endif
