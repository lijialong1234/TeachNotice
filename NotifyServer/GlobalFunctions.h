#ifndef _GLOBALFUNCTIONS_H
#define _GLOBALFUNCTIONS_H
#include <Tlhelp32.h>
#include <tchar.h>
#include <locale.h> 
#include <iostream>
#include "Jason\json.h"
bool set_string(std::wstring& stupid_std_string, const wchar_t* str);
std::wstring UTF8ToUnicode(const std::string& str);
std::string UnicodeToUTF8(const std::wstring& str);
std::wstring ANSIToUnicode(const std::string& str);
std::string UnicodeToANSI(const std::wstring& str);
std::wstring StringToWstring(const std::string str);
std::string WstringToString(const std::wstring str);
int UTF82C(const char* pIn, std::string& strOut);
int C2UTF8(const char* pIn, std::string& strOut);
bool ToUCS2(LPTSTR pcOut, int nOutLen, const char* kpcIn);
bool ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn);
BOOL  GetIpByDomainName(char *szHost, char szIp[MAX_PATH], int *nCount);
__int64 TimeDiffinSecond(SYSTEMTIME left, SYSTEMTIME right);
int RestartMYSQLService(TCHAR *pszServiceName, BOOL bRestartifRun = FALSE);
#endif //_GLOBALFUNCTIONS_H