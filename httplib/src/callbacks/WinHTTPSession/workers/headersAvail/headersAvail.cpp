#include "pch.h"

#include "headersAvail.h"

struct RequestData {
	// XXXXXXXXXXXX0000 = X (12bit): Reserved for HTTP RETURN STATUS CODE
	// XXXXXXXXXXXX0001 = request succsessfully finished + valid http return status code available
	// XXXXXXXXXXXX0010 = return SendRequest() function with 0 (Ivalid URI)
	// XXXXXXXXXXXX0100 = return SendRequest() function with -1 (SSL Certificate error)
	// XXXXXXXXXXXX1000 = is the request finished / should the function return
	USHORT m_nBitFlags;
	char* m_szData;
};

void headersAvail_worker(RequestData* Data, HINTERNET hRequest) {
	wchar_t* szHeaderData = nullptr;
	DWORD dwHeaderDataSize = 0;

	WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		szHeaderData = new wchar_t[dwHeaderDataSize];
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, szHeaderData, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
	}

	if (dwHeaderDataSize == 0) {
		Data->m_nBitFlags = 0;
		Data->m_nBitFlags += (~(Data->m_nBitFlags << 14) & 0b0010);
		Data->m_nBitFlags += (~(Data->m_nBitFlags << 12) & 0b1000);
		return;
	}

	char szBufferCode[8] = {};
	wcstombs(szBufferCode, szHeaderData, 8);

	unsigned short nStatusCode = atoi(szBufferCode);

	Data->m_nBitFlags += nStatusCode << 4;
	Data->m_nBitFlags += (~(Data->m_nBitFlags << 15) & 0b0001);
}