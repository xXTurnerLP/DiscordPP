#include "pch.h"

#include "Session.h"

#include "workers/dataAvail/dataAvail.h"
#include "workers/headersAvail/headersAvail.h"

struct RequestData {
	// XXXXXXXXXXXX0000 = X (12bit): Reserved for HTTP RETURN STATUS CODE
	// XXXXXXXXXXXX0001 = request succsessfully finished + valid http return status code available
	// XXXXXXXXXXXX0010 = return SendRequest() function with 0 (Ivalid URI)
	// XXXXXXXXXXXX0100 = return SendRequest() function with -1 (SSL Certificate error)
	// XXXXXXXXXXXX1000 = is the request finished / should the function return
	USHORT m_nBitFlags;
	char* szDataRead;
	char* szDataWrite;
};

void WINAPI hSession_Callback(
	IN HINTERNET hInternet,
	IN DWORD_PTR dwContext,
	IN DWORD dwInternetStatus,
	IN LPVOID lpvStatusInformation,
	IN DWORD dwStatusInformationLength
) {
	switch (dwInternetStatus) {
	case WINHTTP_CALLBACK_STATUS_REQUEST_SENT:
		if ((*(RequestData**)dwContext)->szDataWrite == nullptr) {
			WinHttpReceiveResponse(hInternet, 0);
		}
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE: {
		std::thread(headersAvail_worker, *(RequestData**)dwContext, hInternet).detach();
		WinHttpQueryDataAvailable(hInternet, NULL);
	}
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE: {
		std::thread(dataAvail_worker, *(RequestData**)dwContext, lpvStatusInformation, hInternet).detach();
	}
		break;
	case WINHTTP_CALLBACK_STATUS_SECURE_FAILURE:
		(*(RequestData**)dwContext)->m_nBitFlags = 0;
		(*(RequestData**)dwContext)->m_nBitFlags += (~((*(RequestData**)dwContext)->m_nBitFlags << 13) & 0b0100);
		(*(RequestData**)dwContext)->m_nBitFlags += (~((*(RequestData**)dwContext)->m_nBitFlags << 12) & 0b1000);
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		(*(RequestData**)dwContext)->m_nBitFlags = 0;
		(*(RequestData**)dwContext)->m_nBitFlags += (~((*(RequestData**)dwContext)->m_nBitFlags << 14) & 0b0010);
		(*(RequestData**)dwContext)->m_nBitFlags += (~((*(RequestData**)dwContext)->m_nBitFlags << 12) & 0b1000);
		break;
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		WinHttpQueryDataAvailable(hInternet, NULL);
		break;
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
		if ((*(RequestData**)dwContext)->szDataWrite != nullptr) {
			WinHttpWriteData(hInternet, (*(RequestData**)dwContext)->szDataWrite, strlen((*(RequestData**)dwContext)->szDataWrite), NULL);
		}
		break;
	case WINHTTP_CALLBACK_STATUS_WRITE_COMPLETE:
		if ((*(RequestData**)dwContext)->szDataWrite != nullptr) {
			WinHttpReceiveResponse(hInternet, 0);
		}
		break;
	case WINHTTP_CALLBACK_STATUS_REDIRECT:
		printf("Session handler: Redirection was attempted, and canceled. Redirect Destination: %s\n", (char*)lpvStatusInformation);
		break;
	}
}