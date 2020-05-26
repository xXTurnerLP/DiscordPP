#include "pch.h"

#include "HeaderTypes.h"

#include "../callbacks/WinHTTPSession/Session.h"

#include "HTTPRequest.hpp"

void _appendToHeader(std::string& private_member, const char* szValue, const char* szHeaderName);

namespace httplib {
	HTTPRequest::HTTPRequest(FILE* temp_pSTDOUT) {
		*stdout = *temp_pSTDOUT;

		RequestData = new requestData_t();
		RequestData->m_nBitFlags = 0b0000'0000'0000'0000;
		RequestData->m_szData = nullptr;
	}

	HTTPRequest::~HTTPRequest() {
		if (RequestData->m_szData != nullptr) delete[] RequestData->m_szData;
		delete RequestData;
	}

	bool HTTPRequest::AddHeader(HeaderType Header, HeaderValue szValue) {
		switch (Header) {
		case HeaderType::Accept:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Accept");
			break;
		case HeaderType::Accept_Encoding:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Accept-Encoding");
			break;
		case HeaderType::Authorization:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Authorization");
			break;
		case HeaderType::Connection:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Connection");
			break;
		case HeaderType::Content_Encoding:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Content-Encoding");
			break;
		case HeaderType::Content_Length:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Content-Length");
			break;
		case HeaderType::Content_Type:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Content-Type");
			break;
		case HeaderType::User_Agent:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "User-Agent");
			break;
		case HeaderType::Upgrade:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Upgrade");
			break;
		case HeaderType::Referer:
			_appendToHeader(m_szHeadersOut, (char*)szValue, "Referer");
			break;
		default:
			return false;
		}

		return true;
	}

	const char* HTTPRequest::GetData() {
		return RequestData->m_szData;
	}

	RESPONSE HTTPRequest::HTTPRequest_Internal(HTTPMethod szMethod, const char* szData, Address szURI, Port nPort) {
		char* pStartEndpoint = (char*)strchr(szURI, L'/');

		HINTERNET hSession = WinHttpOpen(L"", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, WINHTTP_FLAG_ASYNC);
		if (hSession) {
			void* phSession_Callback = WinHttpSetStatusCallback(hSession, (WINHTTP_STATUS_CALLBACK)hSession_Callback, WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 0);
			if (phSession_Callback == WINHTTP_INVALID_STATUS_CALLBACK) {
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpen() async at WinHttpSetStatusCallback() (Cannot install callback function): %u\n", GetLastError()); // This is temporary. Should not be used.
			}
		} else {
			// TODO: Display the error message in the gui/console of DiscordPP
			printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpen() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}

		wchar_t* szwConnectionUri;

		if (pStartEndpoint != nullptr) {
			szwConnectionUri = new wchar_t[pStartEndpoint - szURI + 1];
			ZeroMemory(szwConnectionUri, (pStartEndpoint - szURI + 1) * sizeof(wchar_t));
			mbstowcs(szwConnectionUri, szURI, pStartEndpoint - szURI);
		} else {
			szwConnectionUri = new wchar_t[strlen(szURI) + 1];
			ZeroMemory(szwConnectionUri, (strlen(szURI) + 1) * sizeof(wchar_t));
			mbstowcs(szwConnectionUri, szURI, strlen(szURI));
		}

		HINTERNET hConnection = WinHttpConnect(hSession, szwConnectionUri, nPort, 0);
		if (!hConnection) {
			// TODO: Display the error message in the gui/console of DiscordPP
			printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpConnect() async: %u\n", GetLastError()); // This is temporary. Should not be used.
			if (GetLastError() == 12005)
				return 0;
		}
		delete[] szwConnectionUri;

		wchar_t* szwEndpointUri;
		if (pStartEndpoint != nullptr) {
			szwEndpointUri = new wchar_t[(strlen(szURI) - (pStartEndpoint - szURI)) + 1];
			ZeroMemory(szwEndpointUri, ((strlen(szURI) - (pStartEndpoint - szURI)) + 1) * sizeof(wchar_t));
			mbstowcs(szwEndpointUri, szURI + (pStartEndpoint - szURI), (strlen(szURI) - (pStartEndpoint - szURI)));
		} else {
			szwEndpointUri = (wchar_t*)L"/";
		}

		const wchar_t* szDataAcceptTypes[] = {L"", 0};
		HINTERNET hRequest = WinHttpOpenRequest(hConnection, szMethod, szwEndpointUri, L"HTTP/2.0", WINHTTP_NO_REFERER, szDataAcceptTypes, WINHTTP_FLAG_SECURE);
		if (!hRequest) {
			// TODO: Display the error message in the gui/console of DiscordPP
			printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}
		if (pStartEndpoint != nullptr) delete[] szwEndpointUri;

		wchar_t* szwHeaders = strcmp(m_szHeadersOut.c_str(), "") ? new wchar_t[m_szHeadersOut.length() + 1] : nullptr;
		if (szwHeaders != nullptr) {
			ZeroMemory(szwHeaders, (m_szHeadersOut.length() + 1) * sizeof(wchar_t));
			mbstowcs(szwHeaders, m_szHeadersOut.c_str(), m_szHeadersOut.length());
		}

		if (strcmp(m_szHeadersOut.c_str(), "") != 0 && szwHeaders != nullptr) {
			if (!WinHttpAddRequestHeaders(hRequest, szwHeaders, wcslen(szwHeaders), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}

		if (szwHeaders != nullptr) {
			if (!WinHttpSendRequest(hRequest, szwHeaders, wcslen(szwHeaders), WINHTTP_NO_REQUEST_DATA, NULL, 0, (DWORD_PTR)&RequestData))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		} else {
			if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, NULL, 0, (DWORD_PTR)&RequestData))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}

		DWORD dwOption = WINHTTP_DISABLE_REDIRECTS;
		WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &dwOption, sizeof(dwOption));

		while (1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if ((RequestData->m_nBitFlags >> 3) & 1) {
				WinHttpCloseHandle(hRequest);
				WinHttpCloseHandle(hConnection);
				WinHttpCloseHandle(hSession);
				if ((RequestData->m_nBitFlags >> 2) & 1) {
					return -1;
				} else if ((RequestData->m_nBitFlags >> 1) & 1) {
					return 0;
				} else if ((RequestData->m_nBitFlags >> 0) & 1) {
					return RequestData->m_nBitFlags >> 4;
				}
			}
		}
	}
}

void _appendToHeader(std::string& private_member, const char* szValue, const char* szHeaderName) {
	private_member += szHeaderName;
	private_member += ":";
	private_member += szValue;
	private_member += "\r\n";
}