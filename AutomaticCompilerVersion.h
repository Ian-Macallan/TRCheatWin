#pragma once

#ifndef AUTOMATICOMPILERVERSION_H
#define AUTOMATICOMPILERVERSION_H   1

#ifndef _MSC_VER
#define _MSC_VER        0
#endif
#ifndef _MSC_FULL_VER
#define _MSC_FULL_VER   0
#endif


#if _MSC_VER >= 1940
#define VS_VERSION_A        "Visual Studio > 2022 "
#define VS_VERSION_W        L"Visual Studio > 2022 "
#define VS_VERSION_N        _MSC_VER
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio > 2022 "
#else
#define VS_VERSION_T        "Visual Studio > 2022 "
#endif

#elif _MSC_FULL_VER >= 193933521
#define VS_VERSION_A        "Visual Studio 2022 version 17.9.2"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.9.2"
#define VS_VERSION_N        1939
#define VS_VERSION_F        193933521
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.9.2"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.9.2"
#endif

#elif _MSC_FULL_VER >= 193833133
#define VS_VERSION_A        "Visual Studio 2022 version 17.8.3"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.8.3"
#define VS_VERSION_N        1938
#define VS_VERSION_F        193833133
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.8.3"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.8.3"
#endif

#elif _MSC_FULL_VER >= 193732822
#define VS_VERSION_A        "Visual Studio 2022 version 17.7.0"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.7.0"
#define VS_VERSION_N        1937
#define VS_VERSION_F        193732822
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.7.0"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.7.0"
#endif

#elif _MSC_FULL_VER >= 193632532
#define VS_VERSION_A        "Visual Studio 2022 version 17.6.2"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.6.2"
#define VS_VERSION_N        1936
#define VS_VERSION_F        193632532
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.6.2"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.6.2"
#endif

#elif _MSC_FULL_VER >= 193532217
#define VS_VERSION_A        "Visual Studio 2022 version 17.5.4"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.5.4"
#define VS_VERSION_N        1935
#define VS_VERSION_F        193532217
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.5.4"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.5.4"
#endif

#elif _MSC_FULL_VER >= 193431944
#define VS_VERSION_A        "Visual Studio 2022 version 17.4.9"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.4.9"
#define VS_VERSION_N        1934
#define VS_VERSION_F        193431944
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.4.9"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.4.9"
#endif

#elif _MSC_FULL_VER >= 193331630
#define VS_VERSION_A        "Visual Studio 2022 version 17.3.6"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.3.6"
#define VS_VERSION_N        1933
#define VS_VERSION_F        193331630
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.3.6"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.3.6"
#endif

#elif _MSC_FULL_VER >= 193231329
#define VS_VERSION_A        "Visual Studio 2022 version 17.2.2"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.2.2"
#define VS_VERSION_N        1932
#define VS_VERSION_F        193231329
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.2.2"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.2.2"
#endif

#elif _MSC_FULL_VER >= 193030706
#define VS_VERSION_A        "Visual Studio 2022 version 17.0.2"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.0.2"
#define VS_VERSION_N        1930
#define VS_VERSION_F        193030706
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.0.2"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.0.2"
#endif

#elif _MSC_FULL_VER >= 193030705
#define VS_VERSION_A        "Visual Studio 2022 version 17.0.1"
#define VS_VERSION_W        L"Visual Studio 2022 version 17.0.1"
#define VS_VERSION_N        1930
#define VS_VERSION_F        193030705
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2022 version 17.0.1"
#else
#define VS_VERSION_T        "Visual Studio 2022 version 17.0.1"
#endif

#elif _MSC_FULL_VER >= 192930133
#define VS_VERSION_A        "Visual Studio 2019 version 16.11.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.11.2"
#define VS_VERSION_N        1929
#define VS_VERSION_F        192930133
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.11.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.11.2"
#endif

#elif _MSC_FULL_VER >= 192829913
#define VS_VERSION_A        "Visual Studio 2019 version 16.9.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.9.2"
#define VS_VERSION_N        1928
#define VS_VERSION_F        192829913
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.9.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.9.2"
#endif

#elif _MSC_FULL_VER >= 192829334
#define VS_VERSION_A        "Visual Studio 2019 version 16.8.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.8.2"
#define VS_VERSION_N        1928
#define VS_VERSION_F        192829334
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.8.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.8.2"
#endif

#elif _MSC_FULL_VER >= 192829333
#define VS_VERSION_A        "Visual Studio 2019 version 16.8.1"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.8.1"
#define VS_VERSION_N        1928
#define VS_VERSION_F        192829333
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.8.1"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.8.1"
#endif

#elif _MSC_FULL_VER >= 192729112
#define VS_VERSION_A        "Visual Studio 2019 version 16.7"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.7"
#define VS_VERSION_N        1927
#define VS_VERSION_F        192729112
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.7"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.7"
#endif

#elif _MSC_FULL_VER >= 192628806
#define VS_VERSION_A        "Visual Studio 2019 version 16.6.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.6.2"
#define VS_VERSION_N        1926
#define VS_VERSION_F        192628806
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.6.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.6.2"
#endif

#elif _MSC_FULL_VER >= 192528611
#define VS_VERSION_A        "Visual Studio 2019 version 16.5.1"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.5.1"
#define VS_VERSION_N        1925
#define VS_VERSION_F        192528611
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.5.1"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.5.1"
#endif

#elif _MSC_FULL_VER >= 192428314
#define VS_VERSION_A        "Visual Studio 2019 version 16.4.0"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.4.0"
#define VS_VERSION_N        1924
#define VS_VERSION_F        192428314
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.4.0"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.4.0"
#endif

#elif _MSC_FULL_VER >= 192328105
#define VS_VERSION_A        "Visual Studio 2019 version 16.3.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.3.2"
#define VS_VERSION_N        1923
#define VS_VERSION_F        192328105
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.3.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.3.2"
#endif

#elif _MSC_FULL_VER >= 192227905
#define VS_VERSION_A        "Visual Studio 2019 version 16.2.3"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.2.3"
#define VS_VERSION_N        1922
#define VS_VERSION_F        192227905
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.2.3"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.2.3"
#endif

#elif _MSC_FULL_VER >= 192127702
#define VS_VERSION_A        "Visual Studio 2019 version 16.1.2"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.1.2"
#define VS_VERSION_N        1921
#define VS_VERSION_F        192127702
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.1.2"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.1.2"
#endif

#elif _MSC_FULL_VER >= 192027508
#define VS_VERSION_A        "Visual Studio 2019 version 16.0.0"
#define VS_VERSION_W        L"Visual Studio 2019 version 16.0.0"
#define VS_VERSION_N        1920
#define VS_VERSION_F        192027508
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2019 version 16.0.0"
#else
#define VS_VERSION_T        "Visual Studio 2019 version 16.0.0"
#endif

#elif _MSC_FULL_VER >= 191627030
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.11"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.11"
#define VS_VERSION_N        1916
#define VS_VERSION_F        191627030
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.11"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.11"
#endif

#elif _MSC_FULL_VER >= 191627027
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.7"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.7"
#define VS_VERSION_N        1916
#define VS_VERSION_F        191627027
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.7"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.7"
#endif

#elif _MSC_FULL_VER >= 191627026
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.5"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.5"
#define VS_VERSION_N        1916
#define VS_VERSION_F        191627026
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.5"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.5"
#endif

#elif _MSC_FULL_VER >= 191627025
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.4"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.4"
#define VS_VERSION_N        1916
#define VS_VERSION_F        191627025
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.4"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.4"
#endif

#elif _MSC_FULL_VER >= 191627023
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.1"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.1"
#define VS_VERSION_N        1916
#define VS_VERSION_F        191627023
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.1"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.1"
#endif

#elif _MSC_VER >= 1916
#define VS_VERSION_A        "Visual Studio 2017 version 15.9.0"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.9.0"
#define VS_VERSION_N        1916
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.9.0"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.9.0"
#endif

#elif _MSC_VER >= 1915
#define VS_VERSION_A        "Visual Studio 2017 version 15.8.0"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.8.0"
#define VS_VERSION_N        1915
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.8.0"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.8.0"
#endif

#elif _MSC_FULL_VER >= 191426433
#define VS_VERSION_A        "Visual Studio 2017 version 15.7.5"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.7.5"
#define VS_VERSION_N        1914
#define VS_VERSION_F        191426433
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.7.5"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.7.5"
#endif

#elif _MSC_FULL_VER >= 191426430
#define VS_VERSION_A        "Visual Studio 2017 version 15.7.3"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.7.3"
#define VS_VERSION_N        1914
#define VS_VERSION_F        191426430
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.7.3"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.7.3"
#endif

#elif _MSC_FULL_VER >= 191426429
#define VS_VERSION_A        "Visual Studio 2017 version 15.7.2"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.7.2"
#define VS_VERSION_N        1914
#define VS_VERSION_F        191426429
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.7.2"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.7.2"
#endif

#elif _MSC_FULL_VER >= 191426428
#define VS_VERSION_A        "Visual Studio 2017 version 15.7.1"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.7.1"
#define VS_VERSION_N        1914
#define VS_VERSION_F        191426428
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.7.1"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.7.1"
#endif

#elif _MSC_FULL_VER >= 191326132
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.7"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.7"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326132
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.7"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.7"
#endif

#elif _MSC_FULL_VER >= 191326131
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.6"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.6"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326131
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.6"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.6"
#endif

#elif _MSC_FULL_VER >= 191326129
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.4"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.4"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326129
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.4"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.4"
#endif

#elif _MSC_FULL_VER >= 191326129
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.3"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.3"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326129
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.3"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.3"
#endif

#elif _MSC_FULL_VER >= 191326128
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.2"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.2"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326128
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.2"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.2"
#endif

#elif _MSC_FULL_VER >= 191326128
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.1"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.1"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326128
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.1"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.1"
#endif

#elif _MSC_FULL_VER >= 191326128
#define VS_VERSION_A        "Visual Studio 2017 version 15.6.0"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.6.0"
#define VS_VERSION_N        1913
#define VS_VERSION_F        191326128
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.6.0"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.6.0"
#endif

#elif _MSC_FULL_VER >= 191225835
#define VS_VERSION_A        "Visual Studio 2017 version 15.5.7"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.5.7"
#define VS_VERSION_N        1912
#define VS_VERSION_F        191225835
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.5.7"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.5.7"
#endif

#elif _MSC_FULL_VER >= 191225835
#define VS_VERSION_A        "Visual Studio 2017 version 15.5.6"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.5.6"
#define VS_VERSION_N        1912
#define VS_VERSION_F        191225835
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.5.6"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.5.6"
#endif

#elif _MSC_FULL_VER >= 191225834
#define VS_VERSION_A        "Visual Studio 2017 version 15.5.4"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.5.4"
#define VS_VERSION_N        1912
#define VS_VERSION_F        191225834
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.5.4"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.5.4"
#endif

#elif _MSC_FULL_VER >= 191225834
#define VS_VERSION_A        "Visual Studio 2017 version 15.5.3"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.5.3"
#define VS_VERSION_N        1912
#define VS_VERSION_F        191225834
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.5.3"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.5.3"
#endif

#elif _MSC_FULL_VER >= 191225831
#define VS_VERSION_A        "Visual Studio 2017 version 15.5.2"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.5.2"
#define VS_VERSION_N        1912
#define VS_VERSION_F        191225831
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.5.2"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.5.2"
#endif

#elif _MSC_FULL_VER >= 191125547
#define VS_VERSION_A        "Visual Studio 2017 version 15.4.5"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.4.5"
#define VS_VERSION_N        1911
#define VS_VERSION_F        191125547
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.4.5"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.4.5"
#endif

#elif _MSC_FULL_VER >= 191125542
#define VS_VERSION_A        "Visual Studio 2017 version 15.4.4"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.4.4"
#define VS_VERSION_N        1911
#define VS_VERSION_F        191125542
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.4.4"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.4.4"
#endif

#elif _MSC_FULL_VER >= 191125507
#define VS_VERSION_A        "Visual Studio 2017 version 15.3.3"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.3.3"
#define VS_VERSION_N        1911
#define VS_VERSION_F        191125507
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.3.3"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.3.3"
#endif

#elif _MSC_FULL_VER >= 191025017
#define VS_VERSION_A        "Visual Studio 2017 version 15.2"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.2"
#define VS_VERSION_N        1910
#define VS_VERSION_F        191025017
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.2"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.2"
#endif

#elif _MSC_FULL_VER >= 191025017
#define VS_VERSION_A        "Visual Studio 2017 version 15.1"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.1"
#define VS_VERSION_N        1910
#define VS_VERSION_F        191025017
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.1"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.1"
#endif

#elif _MSC_FULL_VER >= 191025017
#define VS_VERSION_A        "Visual Studio 2017 version 15.0"
#define VS_VERSION_W        L"Visual Studio 2017 version 15.0"
#define VS_VERSION_N        1910
#define VS_VERSION_F        191025017
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2017 version 15.0"
#else
#define VS_VERSION_T        "Visual Studio 2017 version 15.0"
#endif

#elif _MSC_FULL_VER >= 190024210
#define VS_VERSION_A        "Visual Studio 2015 Update 3 [14.0]"
#define VS_VERSION_W        L"Visual Studio 2015 Update 3 [14.0]"
#define VS_VERSION_N        1900
#define VS_VERSION_F        190024210
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2015 Update 3 [14.0]"
#else
#define VS_VERSION_T        "Visual Studio 2015 Update 3 [14.0]"
#endif

#elif _MSC_FULL_VER >= 190023918
#define VS_VERSION_A        "Visual Studio 2015 Update 2 [14.0]"
#define VS_VERSION_W        L"Visual Studio 2015 Update 2 [14.0]"
#define VS_VERSION_N        1900
#define VS_VERSION_F        190023918
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2015 Update 2 [14.0]"
#else
#define VS_VERSION_T        "Visual Studio 2015 Update 2 [14.0]"
#endif

#elif _MSC_FULL_VER >= 190023506
#define VS_VERSION_A        "Visual Studio 2015 Update 1 [14.0]"
#define VS_VERSION_W        L"Visual Studio 2015 Update 1 [14.0]"
#define VS_VERSION_N        1900
#define VS_VERSION_F        190023506
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2015 Update 1 [14.0]"
#else
#define VS_VERSION_T        "Visual Studio 2015 Update 1 [14.0]"
#endif

#elif _MSC_FULL_VER >= 190023026
#define VS_VERSION_A        "Visual Studio 2015 [14.0]"
#define VS_VERSION_W        L"Visual Studio 2015 [14.0]"
#define VS_VERSION_N        1900
#define VS_VERSION_F        190023026
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2015 [14.0]"
#else
#define VS_VERSION_T        "Visual Studio 2015 [14.0]"
#endif

#elif _MSC_FULL_VER >= 180040629
#define VS_VERSION_A        "Visual Studio 2013 Update 5 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 Update 5 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180040629
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 Update 5 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 Update 5 [12.0]"
#endif

#elif _MSC_FULL_VER >= 180031101
#define VS_VERSION_A        "Visual Studio 2013 Update 4 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 Update 4 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180031101
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 Update 4 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 Update 4 [12.0]"
#endif

#elif _MSC_FULL_VER >= 180030723
#define VS_VERSION_A        "Visual Studio 2013 Update 3 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 Update 3 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180030723
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 Update 3 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 Update 3 [12.0]"
#endif

#elif _MSC_FULL_VER >= 180030501
#define VS_VERSION_A        "Visual Studio 2013 Update 2 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 Update 2 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180030501
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 Update 2 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 Update 2 [12.0]"
#endif

#elif _MSC_FULL_VER >= 180021005
#define VS_VERSION_A        "Visual Studio 2013 Update 1 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 Update 1 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180021005
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 Update 1 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 Update 1 [12.0]"
#endif

#elif _MSC_FULL_VER >= 180021005
#define VS_VERSION_A        "Visual Studio 2013 [12.0]"
#define VS_VERSION_W        L"Visual Studio 2013 [12.0]"
#define VS_VERSION_N        1800
#define VS_VERSION_F        180021005
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2013 [12.0]"
#else
#define VS_VERSION_T        "Visual Studio 2013 [12.0]"
#endif

#elif _MSC_FULL_VER >= 170061030
#define VS_VERSION_A        "Visual Studio 2012 Update 4 [11.0]"
#define VS_VERSION_W        L"Visual Studio 2012 Update 4 [11.0]"
#define VS_VERSION_N        1700
#define VS_VERSION_F        170061030
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2012 Update 4 [11.0]"
#else
#define VS_VERSION_T        "Visual Studio 2012 Update 4 [11.0]"
#endif

#elif _MSC_FULL_VER >= 170060610
#define VS_VERSION_A        "Visual Studio 2012 Update 3 [11.0]"
#define VS_VERSION_W        L"Visual Studio 2012 Update 3 [11.0]"
#define VS_VERSION_N        1700
#define VS_VERSION_F        170060610
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2012 Update 3 [11.0]"
#else
#define VS_VERSION_T        "Visual Studio 2012 Update 3 [11.0]"
#endif

#elif _MSC_FULL_VER >= 170060315
#define VS_VERSION_A        "Visual Studio 2012 Update 2 [11.0]"
#define VS_VERSION_W        L"Visual Studio 2012 Update 2 [11.0]"
#define VS_VERSION_N        1700
#define VS_VERSION_F        170060315
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2012 Update 2 [11.0]"
#else
#define VS_VERSION_T        "Visual Studio 2012 Update 2 [11.0]"
#endif

#elif _MSC_FULL_VER >= 170051106
#define VS_VERSION_A        "Visual Studio 2012 Update 1 [11.0]"
#define VS_VERSION_W        L"Visual Studio 2012 Update 1 [11.0]"
#define VS_VERSION_N        1700
#define VS_VERSION_F        170051106
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2012 Update 1 [11.0]"
#else
#define VS_VERSION_T        "Visual Studio 2012 Update 1 [11.0]"
#endif

#elif _MSC_FULL_VER >= 170050727
#define VS_VERSION_A        "Visual Studio 2012 [11.0]"
#define VS_VERSION_W        L"Visual Studio 2012 [11.0]"
#define VS_VERSION_N        1700
#define VS_VERSION_F        170050727
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2012 [11.0]"
#else
#define VS_VERSION_T        "Visual Studio 2012 [11.0]"
#endif

#elif _MSC_FULL_VER >= 160040219
#define VS_VERSION_A        "Visual Studio 2010 SP1 [10.0]"
#define VS_VERSION_W        L"Visual Studio 2010 SP1 [10.0]"
#define VS_VERSION_N        1600
#define VS_VERSION_F        160040219
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2010 SP1 [10.0]"
#else
#define VS_VERSION_T        "Visual Studio 2010 SP1 [10.0]"
#endif

#elif _MSC_FULL_VER >= 160030319
#define VS_VERSION_A        "Visual Studio 2010 [10.0]"
#define VS_VERSION_W        L"Visual Studio 2010 [10.0]"
#define VS_VERSION_N        1600
#define VS_VERSION_F        160030319
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2010 [10.0]"
#else
#define VS_VERSION_T        "Visual Studio 2010 [10.0]"
#endif

#elif _MSC_FULL_VER >= 150030729
#define VS_VERSION_A        "Visual Studio 2008 SP1 [9.0]"
#define VS_VERSION_W        L"Visual Studio 2008 SP1 [9.0]"
#define VS_VERSION_N        1500
#define VS_VERSION_F        150030729
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2008 SP1 [9.0]"
#else
#define VS_VERSION_T        "Visual Studio 2008 SP1 [9.0]"
#endif

#elif _MSC_FULL_VER >= 150021022
#define VS_VERSION_A        "Visual Studio 2008 [9.0]"
#define VS_VERSION_W        L"Visual Studio 2008 [9.0]"
#define VS_VERSION_N        1500
#define VS_VERSION_F        150021022
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2008 [9.0]"
#else
#define VS_VERSION_T        "Visual Studio 2008 [9.0]"
#endif

#elif _MSC_FULL_VER >= 140050727
#define VS_VERSION_A        "Visual Studio 2005 SP1 [8.0]"
#define VS_VERSION_W        L"Visual Studio 2005 SP1 [8.0]"
#define VS_VERSION_N        1400
#define VS_VERSION_F        140050727
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2005 SP1 [8.0]"
#else
#define VS_VERSION_T        "Visual Studio 2005 SP1 [8.0]"
#endif

#elif _MSC_FULL_VER >= 140050320
#define VS_VERSION_A        "Visual Studio 2005 [8.0]"
#define VS_VERSION_W        L"Visual Studio 2005 [8.0]"
#define VS_VERSION_N        1400
#define VS_VERSION_F        140050320
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 2005 [8.0]"
#else
#define VS_VERSION_T        "Visual Studio 2005 [8.0]"
#endif

#elif _MSC_FULL_VER >= 13106030
#define VS_VERSION_A        "Visual Studio .NET 2003 SP1 [7.1]"
#define VS_VERSION_W        L"Visual Studio .NET 2003 SP1 [7.1]"
#define VS_VERSION_N        1310
#define VS_VERSION_F        13106030
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio .NET 2003 SP1 [7.1]"
#else
#define VS_VERSION_T        "Visual Studio .NET 2003 SP1 [7.1]"
#endif

#elif _MSC_FULL_VER >= 13103077
#define VS_VERSION_A        "Visual Studio .NET 2003 [7.1]"
#define VS_VERSION_W        L"Visual Studio .NET 2003 [7.1]"
#define VS_VERSION_N        1310
#define VS_VERSION_F        13103077
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio .NET 2003 [7.1]"
#else
#define VS_VERSION_T        "Visual Studio .NET 2003 [7.1]"
#endif

#elif _MSC_FULL_VER >= 13009466
#define VS_VERSION_A        "Visual Studio .NET 2002 [7.0]"
#define VS_VERSION_W        L"Visual Studio .NET 2002 [7.0]"
#define VS_VERSION_N        1300
#define VS_VERSION_F        13009466
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio .NET 2002 [7.0]"
#else
#define VS_VERSION_T        "Visual Studio .NET 2002 [7.0]"
#endif

#elif _MSC_FULL_VER >= 12008804
#define VS_VERSION_A        "Visual Studio 6.0 SP6"
#define VS_VERSION_W        L"Visual Studio 6.0 SP6"
#define VS_VERSION_N        1200
#define VS_VERSION_F        12008804
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 6.0 SP6"
#else
#define VS_VERSION_T        "Visual Studio 6.0 SP6"
#endif

#elif _MSC_FULL_VER >= 12008804
#define VS_VERSION_A        "Visual Studio 6.0 SP5"
#define VS_VERSION_W        L"Visual Studio 6.0 SP5"
#define VS_VERSION_N        1200
#define VS_VERSION_F        12008804
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 6.0 SP5"
#else
#define VS_VERSION_T        "Visual Studio 6.0 SP5"
#endif

#elif _MSC_VER >= 1200
#define VS_VERSION_A        "Visual Studio 6.0"
#define VS_VERSION_W        L"Visual Studio 6.0"
#define VS_VERSION_N        1200
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 6.0"
#else
#define VS_VERSION_T        "Visual Studio 6.0"
#endif

#elif _MSC_VER >= 1100
#define VS_VERSION_A        "Visual Studio 97 [5.0]"
#define VS_VERSION_W        L"Visual Studio 97 [5.0]"
#define VS_VERSION_N        1100
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio 97 [5.0]"
#else
#define VS_VERSION_T        "Visual Studio 97 [5.0]"
#endif

#elif _MSC_VER >= 1020
#define VS_VERSION_A        "Visual C++ 4.1"
#define VS_VERSION_W        L"Visual C++ 4.1"
#define VS_VERSION_N        1020
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual C++ 4.1"
#else
#define VS_VERSION_T        "Visual C++ 4.1"
#endif

#elif _MSC_VER >= 1010
#define VS_VERSION_A        "Visual C++ 4.1"
#define VS_VERSION_W        L"Visual C++ 4.1"
#define VS_VERSION_N        1010
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual C++ 4.1"
#else
#define VS_VERSION_T        "Visual C++ 4.1"
#endif

#elif _MSC_VER >= 1000
#define VS_VERSION_A        "Visual C++ 4.0"
#define VS_VERSION_W        L"Visual C++ 4.0"
#define VS_VERSION_N        1000
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual C++ 4.0"
#else
#define VS_VERSION_T        "Visual C++ 4.0"
#endif

#elif _MSC_VER >= 900
#define VS_VERSION_A        "Visual C++ 2.0"
#define VS_VERSION_W        L"Visual C++ 2.0"
#define VS_VERSION_N        900
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual C++ 2.0"
#else
#define VS_VERSION_T        "Visual C++ 2.0"
#endif

#elif _MSC_VER >= 800
#define VS_VERSION_A        "Visual C++ 1.0"
#define VS_VERSION_W        L"Visual C++ 1.0"
#define VS_VERSION_N        800
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual C++ 1.0"
#else
#define VS_VERSION_T        "Visual C++ 1.0"
#endif

#elif _MSC_VER >= 700
#define VS_VERSION_A        "Microsoft C/C++ 7.0"
#define VS_VERSION_W        L"Microsoft C/C++ 7.0"
#define VS_VERSION_N        700
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Microsoft C/C++ 7.0"
#else
#define VS_VERSION_T        "Microsoft C/C++ 7.0"
#endif

#elif _MSC_VER >= 600
#define VS_VERSION_A        "Microsoft C 6.0"
#define VS_VERSION_W        L"Microsoft C 6.0"
#define VS_VERSION_N        600
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Microsoft C 6.0"
#else
#define VS_VERSION_T        "Microsoft C 6.0"
#endif

#else
#define VS_VERSION_A        "Visual Studio"
#define VS_VERSION_W        L"Visual Studio"
#define VS_VERSION_N        _MSC_VER
#define VS_VERSION_F        _MSC_FULL_VER
#ifdef UNICODE
#define VS_VERSION_T        L"Visual Studio"
#else
#define VS_VERSION_T        "Visual Studio"
#endif
#endif


#endif
