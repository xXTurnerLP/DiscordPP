#include "pch.h"

#include "get.hpp"

#include "callbacks/Session.h"

void temp_read(HINTERNET req) {
	wchar_t* szBuffer = nullptr;
	DWORD req_len = 0; // this is a 2 way var, its used to specify the size of OUR buffer then the func returns the total length of the response in THAT SAME var!

	WinHttpQueryHeaders(req, WINHTTP_QUERY_RAW_HEADERS, WINHTTP_HEADER_NAME_BY_INDEX, nullptr, &req_len, WINHTTP_NO_HEADER_INDEX);
	switch (GetLastError()) {
	case ERROR_INSUFFICIENT_BUFFER:
		szBuffer = new wchar_t[req_len];
	}

	WinHttpQueryHeaders(req, WINHTTP_QUERY_RAW_HEADERS, WINHTTP_HEADER_NAME_BY_INDEX, szBuffer, &req_len, WINHTTP_NO_HEADER_INDEX);
	std::cout << "szBuffer address: 0x" << std::hex << szBuffer << std::dec << "\n";

	printf("Data len: %u\n", req_len);

	printf("\nHTTP RESPONSE:\n\n");
	for (unsigned int i = 0; i < req_len; ++i) {
		if (wcsncmp((const wchar_t*)&szBuffer[i], L"\0", 1) == 0) {
			std::wcout << L"\n";
			continue;
		}
		std::wcout << szBuffer[i];
	}
	unsigned long bytes = 0;
	if (WinHttpQueryDataAvailable(req, &bytes)) {
		printf("%u bytes of data available.\n", bytes);
	}

	char data[9600] = {0};
	unsigned long bytesRead = 0;
	if (WinHttpReadData(req, data, bytes, &bytesRead)) {
		printf("%u bytes of data read.\n", bytesRead);
	}

	printf("\n\nData:\n\n%s\n", data);

	// deletes the buffer after use. Heap allocated!
	delete[] szBuffer;
}

void _appendToHeader(std::string& private_member, const char* szValue, const char* szHeaderName);

namespace httplib {
	GetRequest::GetRequest(FILE* temp_pSTDOUT) {
		*stdout = *temp_pSTDOUT;
	}

	RESPONSE GetRequest::SendRequest(Address szURI, Port nPort) {
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

		if (strchr(szURI, L'/') != nullptr) {
			szwConnectionUri = new wchar_t[strchr(szURI, L'/') - szURI + 1];
			ZeroMemory(szwConnectionUri, (strchr(szURI, L'/') - szURI + 1) * sizeof(wchar_t));
			mbstowcs(szwConnectionUri, szURI, strchr(szURI, L'/') - szURI);
		} else {
			szwConnectionUri = new wchar_t[strlen(szURI) + 1];
			ZeroMemory(szwConnectionUri, (strlen(szURI) + 1) * sizeof(wchar_t));
			mbstowcs(szwConnectionUri, szURI, strlen(szURI));
		}
		
		
		HINTERNET hConnection = WinHttpConnect(hSession, szwConnectionUri, nPort, 0);
		if (!hConnection) {
			// TODO: Display the error message in the gui/console of DiscordPP
			printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpConnect() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}
		delete[] szwConnectionUri;

		wchar_t* szwEndpointUri;

		if (strchr(szURI, L'/') != nullptr) {
			szwEndpointUri = new wchar_t[(strlen(szURI) - (strchr(szURI, L'/') - szURI)) + 1];
			ZeroMemory(szwEndpointUri, ((strlen(szURI) - (strchr(szURI, L'/') - szURI)) + 1) * sizeof(wchar_t));
			mbstowcs(szwEndpointUri, szURI + (strchr(szURI, L'/') - szURI) + 1, (strlen(szURI) - (strchr(szURI, L'/') - szURI)));
		} else {
			szwEndpointUri = (wchar_t*)L"/";
		}
		

		const wchar_t* szDataAcceptTypes[] = {L"", 0};
		HINTERNET hRequest = WinHttpOpenRequest(hConnection, L"GET", szwEndpointUri, L"HTTP/2.0", WINHTTP_NO_REFERER, szDataAcceptTypes, WINHTTP_FLAG_SECURE);
		if (!hRequest) {
			// TODO: Display the error message in the gui/console of DiscordPP
			printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}
		if (strchr(szURI, L'/') != nullptr) delete[] szwEndpointUri;

		wchar_t* szwHeaders = strcmp(m_szHeaders.c_str(), "") ? new wchar_t[m_szHeaders.length() + 1] : nullptr;
		if (szwHeaders != nullptr) {
			ZeroMemory(szwHeaders, (m_szHeaders.length() + 1) * sizeof(wchar_t));
			mbstowcs(szwHeaders, m_szHeaders.c_str(), m_szHeaders.length());
		}

		if (strcmp(m_szHeaders.c_str(), "") != 0) {
			if (!WinHttpAddRequestHeaders(hRequest, szwHeaders, wcslen(szwHeaders), WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}
		if (szwHeaders != nullptr) {
			if (!WinHttpSendRequest(hRequest, szwHeaders, wcslen(szwHeaders), WINHTTP_NO_REQUEST_DATA, NULL, 0, 0))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		} else {
			if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, NULL, 0, 0))
				// TODO: Display the error message in the gui/console of DiscordPP
				printf("TEMPORARY: GetLastError() from GetRequest::SendRequest at WinHttpOpenRequest() async: %u\n", GetLastError()); // This is temporary. Should not be used.
		}
		return 0;
	}

	bool GetRequest::AddHeader(HeaderType Header, HeaderValue szValue) {
		switch (Header) {
		case HeaderType::Accept:
			_appendToHeader(m_szHeaders, (char*)szValue, "Accept");
			break;
		case HeaderType::Accept_Encoding:
			_appendToHeader(m_szHeaders, (char*)szValue, "Accept-Encoding");
			break;
		case HeaderType::Authorization:
			_appendToHeader(m_szHeaders, (char*)szValue, "Authorization");
			break;
		case HeaderType::Connection:
			_appendToHeader(m_szHeaders, (char*)szValue, "Connection");
			break;
		case HeaderType::Content_Encoding:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Encoding");
			break;
		case HeaderType::Content_Length:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Length");
			break;
		case HeaderType::Content_Type:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Type");
			break;
		case HeaderType::User_Agent:
			_appendToHeader(m_szHeaders, (char*)szValue, "User-Agent");
			break;
		case HeaderType::Upgrade:
			_appendToHeader(m_szHeaders, (char*)szValue, "Upgrade");
			break;
		case HeaderType::Referer:
			_appendToHeader(m_szHeaders, (char*)szValue, "Referer");
			break;
		default:
			return false;
		}
		
		return true;
	}
}

void _appendToHeader(std::string &private_member, const char* szValue, const char* szHeaderName) {
	private_member += szHeaderName;
	private_member += ":";
	private_member += szValue;
	private_member += "\r\n";
}