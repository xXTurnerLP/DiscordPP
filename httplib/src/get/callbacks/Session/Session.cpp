#include "pch.h"

#include "Session.h"

#include "workers/dataAvail/dataAvail.h"

namespace httplib {
	extern USHORT g_nBitFlags;
	extern char* g_szDataBuffer;
}

void WINAPI hSession_Callback(
	IN HINTERNET hInternet,
	IN DWORD_PTR dwContext,
	IN DWORD dwInternetStatus,
	IN LPVOID lpvStatusInformation,
	IN DWORD dwStatusInformationLength
) {
	switch (dwInternetStatus) {
	case WINHTTP_CALLBACK_STATUS_REQUEST_SENT:
		WinHttpReceiveResponse(hInternet, 0);
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE: {
		wchar_t* szHeaderData = nullptr;
		DWORD dwHeaderDataSize = 0;

		WinHttpQueryHeaders(hInternet, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
		if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			szHeaderData = new wchar_t[dwHeaderDataSize];
			WinHttpQueryHeaders(hInternet, WINHTTP_QUERY_STATUS_CODE, WINHTTP_HEADER_NAME_BY_INDEX, szHeaderData, &dwHeaderDataSize, WINHTTP_NO_HEADER_INDEX);
		}

		if (dwHeaderDataSize == 0) {
			httplib::g_nBitFlags = 0;
			httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 14) & 0b0010);
			httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 12) & 0b1000);
			return;
		}

		char szBufferCode[8] = {};
		wcstombs(szBufferCode, szHeaderData, 8);

		unsigned short nStatusCode = atoi(szBufferCode);

		httplib::g_nBitFlags += nStatusCode << 4;
		httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 15) & 0b0001);

		WinHttpQueryDataAvailable(hInternet, NULL);
	}
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE: {
		std::thread(dataAvail_worker, &httplib::g_nBitFlags, lpvStatusInformation, hInternet);
	}
		break;
	case WINHTTP_CALLBACK_STATUS_SECURE_FAILURE:
		httplib::g_nBitFlags = 0;
		httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 13) & 0b0100);
		httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 12) & 0b1000);
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		httplib::g_nBitFlags = 0;
		httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 14) & 0b0010);
		httplib::g_nBitFlags += (~(httplib::g_nBitFlags << 12) & 0b1000);
		break;
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		WinHttpQueryDataAvailable(hInternet, NULL);
		break;
	case WINHTTP_CALLBACK_STATUS_REDIRECT:
		printf("Session handler: Redirection was attempted, and canceled. Redirect Destination: %s\n", lpvStatusInformation);
		break;
	}
}