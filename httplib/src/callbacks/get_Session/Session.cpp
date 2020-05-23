#include "pch.h"

#include "Session.h"

#include "workers/dataAvail/dataAvail.h"
#include "workers/headersAvail/headersAvail.h"

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
		std::thread(headersAvail_worker, &httplib::g_nBitFlags, hInternet).detach();
		WinHttpQueryDataAvailable(hInternet, NULL);
	}
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE: {
		std::thread(dataAvail_worker, &httplib::g_nBitFlags, lpvStatusInformation, hInternet).detach();
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
		printf("Session handler: Redirection was attempted, and canceled. Redirect Destination: %s\n", (char*)lpvStatusInformation);
		break;
	}
}