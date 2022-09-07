#include "au.h"
#include <string>
#include <windows.h>

using namespace std;

char* AnsiToUtf8(const char* pchAnsi)
{
    int uSize = 0;
    WCHAR* pwText = NULL;
    char* pchUtf8 = NULL;
    int nUtf8Len = 0;

    if (NULL == pchAnsi) return NULL;
    do {
        uSize = MultiByteToWideChar(CP_ACP, 0, pchAnsi, -1, NULL, 0);
        if (uSize <= 0) break;
        pwText = (wchar_t*)malloc((uSize + 1) * sizeof(wchar_t));
        if (!pwText) break;
        memset((void*)pwText, 0, sizeof(wchar_t) * (uSize + 1));
        MultiByteToWideChar(CP_ACP, 0, pchAnsi, -1, pwText, uSize);

        //widechar to utf8
        nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, NULL, 0, NULL, NULL);
        if (0 >= nUtf8Len) break;

        pchUtf8 = (char*)malloc(nUtf8Len + 2);
        if (NULL == pchUtf8) break;
        memset(pchUtf8, 0, nUtf8Len + 2);

        nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, pwText, -1, pchUtf8, nUtf8Len, NULL, NULL);
        if (0 >= nUtf8Len)
        {
            free(pchUtf8);
            pchUtf8 = NULL;
            break;
        }
    } while (0);

    if (NULL != pwText) free(pwText);
    return pchUtf8;
}

char* Utf8ToAnsi(const char* pchUtf8)
{
    int uSize = 0;
    WCHAR* pwText = NULL;
    char* pchAnsic = NULL;
    int ansicStrLen = 0;
    if (NULL == pchUtf8)return NULL;

    do {

        uSize = MultiByteToWideChar(CP_UTF8, 0, pchUtf8, -1, NULL, 0);
        if (uSize <= 0) break;
        pwText = (wchar_t*)malloc((uSize + 1) * sizeof(wchar_t));
        if (!pwText) break;
        memset((void*)pwText, 0, sizeof(wchar_t) * (uSize + 1));
        MultiByteToWideChar(CP_UTF8, 0, pchUtf8, -1, pwText, uSize);
        //widechar to utf8
        ansicStrLen = WideCharToMultiByte(CP_ACP, 0, pwText, -1, NULL, 0, NULL, NULL);
        if (0 >= ansicStrLen) break;

        pchAnsic = (char*)malloc(ansicStrLen + 2);
        if (NULL == pchAnsic) break;
        memset(pchAnsic, 0, ansicStrLen + 2);

        ansicStrLen = WideCharToMultiByte(CP_ACP, 0, pwText, -1, pchAnsic, ansicStrLen, NULL, NULL);
        if (0 >= ansicStrLen)
        {
            free(pchAnsic);
            pchAnsic = NULL;
            break;
        }
    } while (0);
    if (NULL != pwText) free(pwText);
    return pchAnsic;
}