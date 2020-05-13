#include "pch.h"

#include "headersAvail.h"

void headersAvail_worker(unsigned short* pnBitfield, HINTERNET hRequest) {
	wchar_t* szHeaderData = nullptr;
	DWORD dwHeaderDataSize = 0;

	WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		szHeaderData = new wchar_t[dwHeaderDataSize];
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, szHeaderData, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
	}

	if (dwHeaderDataSize == 0) {
		*pnBitfield = 0;
		*pnBitfield += (~(*pnBitfield << 14) & 0b0010);
		*pnBitfield += (~(*pnBitfield << 12) & 0b1000);
		return;
	}

	char szBufferCode[8] = {};
	wcstombs(szBufferCode, szHeaderData, 8);

	unsigned short nStatusCode = atoi(szBufferCode);

	*pnBitfield += nStatusCode << 4;
	*pnBitfield += (~(*pnBitfield << 15) & 0b0001);
}