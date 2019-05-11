#include "stdafx.h"
#include "GlobalFunctions.h"
#include <winsvc.h> 
#include <string.h> 
#define SERVICE_NO_ERROR 0  
#define OPEN_SCMANAGER_ERROR 2  
#define OPEN_SERVICE_ERROR 3  
#define QUERY_SERVICESTATUS_ERROR 4  
#define STOP_SERVICE_ERROR 5  
#define START_SERVICE_ERROR 6  
bool set_string(std::wstring& stupid_std_string, const wchar_t* str)
{
	if (!str) return false;
	stupid_std_string = str;
	return true;
}
/**
通过域名获得IP
*/
BOOL  GetIpByDomainName(char *szHost, char szIp[MAX_PATH], int *nCount)
{
	WSADATA        wsaData;
	HOSTENT   *pHostEnt;
	int             nAdapter = 0;
	struct       sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		//AfxMessageBox("WSAStartup   failed   %s/n", WSAGetLastError());
		return FALSE;
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		if (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);
			char  szBuffer[1024] = { 0 };

			sprintf(szBuffer, "%s", inet_ntoa(sAddr.sin_addr));

			strcpy(szIp, szBuffer);
			//OutputDebugString(szBuffer);
			nAdapter++;
		}

		*nCount = nAdapter;
	}
	else
	{
		DWORD  dwError = GetLastError();
		//CString  csError;
		//csError.Format("%d", dwError);
		//OutputDebugString(csError);
		//OutputDebugString("gethostbyname failed");
		*nCount = 0;
	}
	WSACleanup();
	return TRUE;

}
std::wstring id_or_name_or_text(HELEMENT he)
{
	sciter::dom::element el = he;

	std::wstring name;
	if (set_string(name, el.get_attribute("id").c_str())) return name;
	if (set_string(name, el.get_attribute("name").c_str())) return name;

	name = L"{";
	std::wstring str = el.text().c_str();
	if (str.length())
		name += str;
	name += L"}";
	return name;
}
std::wstring id_or_name_or_tag(HELEMENT he)
{
	if (!he)
		return L"(no element)";
	sciter::dom::element el = he;
	std::wstring name;
	if (set_string(name, el.get_attribute("id").c_str())) return name;
	if (set_string(name, el.get_attribute("name").c_str())) return name;
	name = aux::a2w(el.get_element_type());
	return name;
}

std::wstring UTF8ToUnicode(const std::string& str)
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t *  pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring  rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode;

	return  rt;
}

std::string UnicodeToUTF8(const std::wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_UTF8,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

std::wstring ANSIToUnicode(const std::string& str)
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0);
	wchar_t * pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
		0,
		str.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);
	std::wstring rt;
	rt = (wchar_t*)pUnicode;
	delete pUnicode;
	return rt;
}


std::string UnicodeToANSI(const std::wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	std::string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
int UTF82C(const char* pIn, std::string& strOut)
{
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, NULL, pIn, -1, NULL, 0);
	wchar_t* pwOut = new wchar_t[dwNum + 1];
	memset(pwOut, 0, dwNum * sizeof(wchar_t));
	int ret = MultiByteToWideChar(CP_UTF8, NULL, pIn, -1, pwOut, dwNum);
	dwNum = WideCharToMultiByte(CP_OEMCP, NULL, pwOut, -1, NULL, 0, NULL, FALSE);
	char *psText = new char[dwNum + 1];
	WideCharToMultiByte(CP_OEMCP, NULL, pwOut, -1, psText, dwNum, NULL, FALSE);
	char Last = '\0';
	*(psText + dwNum) = Last;
	strOut = psText;
	delete[]pwOut;
	delete[]psText;
	return ret;
}
int C2UTF8(const char* pIn, std::string& strOut)
{
	DWORD dwNum = MultiByteToWideChar(CP_OEMCP, NULL, pIn, -1, NULL, 0);
	wchar_t* pwOut = new wchar_t[dwNum + 1];
	memset(pwOut, 0, (dwNum) * sizeof(wchar_t));
	int ret = MultiByteToWideChar(CP_OEMCP, NULL, pIn, -1, pwOut, dwNum);
	dwNum = WideCharToMultiByte(CP_UTF8, NULL, pwOut, -1, NULL, 0, NULL, FALSE);
	char *psText = new char[dwNum + 1];
	memset(psText, 0, (dwNum) * sizeof(char));
	WideCharToMultiByte(CP_UTF8, NULL, pwOut, -1, psText, dwNum, 0, 0);
	char Last = '\0';
	*(psText + dwNum) = Last;
	strOut = psText;
	delete[]pwOut;
	delete[]psText;
	return ret;
}
bool ToUCS2(LPTSTR pcOut, int nOutLen, const char* kpcIn)
{
	if (strlen(kpcIn) > 0) {
		// Do the conversion normally
		return MultiByteToWideChar(CP_UTF8, 0, kpcIn, -1, pcOut,
			nOutLen) > 0;
	}
	else if (nOutLen > 1) {
		// Can't distinguish no bytes copied from an error, so handle
		// an empty input string as a special case.
		_tccpy(pcOut, _T(""));
		return true;
	}
	else {
		// Not enough room to do anything!
		return false;
	}
}


//// ToUTF8 ////////////////////////////////////////////////////////////
// Convert a UCS-2 multibyte string to the UTF-8 format required by
// MySQL, and thus MySQL++.
bool ToUTF8(char* pcOut, int nOutLen, LPCWSTR kpcIn)
{
	if (_tcslen(kpcIn) > 0) {
		// Do the conversion normally
		return WideCharToMultiByte(CP_UTF8, 0, kpcIn, -1, pcOut,
			nOutLen, 0, 0) > 0;
	}
	else if (nOutLen > 0) {
		// Can't distinguish no bytes copied from an error, so handle
		// an empty input string as a special case.
		*pcOut = '\0';
		return true;
	}
	else {
		// Not enough room to do anything!
		return false;
	}
}
/************************************************************************/
/*获得两个时间之间相差的秒数                                            */
/************************************************************************/
__int64 TimeDiffinSecond(SYSTEMTIME left, SYSTEMTIME right)
{
	CTime tmLeft(left.wYear, left.wMonth, left.wDay, 0, 0, 0);
	CTime tmRight(right.wYear, right.wMonth, right.wDay, 0, 0, 0);

	CTimeSpan sp = tmLeft - tmRight;
	long SencondsL = left.wHour * 3600 + left.wMinute * 60 + left.wSecond;
	long SencondsR = right.wHour * 3600 + right.wMinute * 60 + right.wSecond;

	return  (__int64)sp.GetDays() * 86400 + (SencondsL - SencondsR);//此处返回秒
}


// 需包含locale、string头文件、使用setlocale函数。
std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

std::string WstringToString(const std::wstring str)
{// wstring转string
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs(p, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}

std::string GetHeadrInfo(json::value info)
{
	json::value JHeadr;
	auto str = info.get_item("UpdateTime").get(L"");
	JHeadr["UpdateTime"] = str;
	JHeadr["Begin"] = info.get_item("Begin").get<int>();
	JHeadr["Count"] = info.get_item("Count").get<int>();
	return WstringToString(JHeadr.to_string().c_str());
}

