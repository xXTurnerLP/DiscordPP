#include "pch.h"

#include "Session.h"

void WINAPI hSession_Callback(
	IN HINTERNET hInternet,
	IN DWORD_PTR dwContext,
	IN DWORD dwInternetStatus,
	IN LPVOID lpvStatusInformation,
	IN DWORD dwStatusInformationLength
) {
	//printf("\n\n--------------------------------\n  - hSession_Callback() call -\n\nhInternet: %X\ndwContext: %u\ndwInternetStatus: %u\nlpvStatusInformation: %X\ndwStatusInformationLength: %u\n--------------------------------\n", hInternet, dwContext, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
	char szServerIP[16] = {};
	DWORD* dwBytes = nullptr;
	WINHTTP_ASYNC_RESULT* async_result = nullptr;

	switch (dwInternetStatus) { //TODO: Display this status in the gui/console of DiscordPP
	case WINHTTP_CALLBACK_STATUS_CLOSING_CONNECTION:
		puts("hSession Status: Connection closed.");
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTED_TO_SERVER:
		wcstombs(szServerIP, (wchar_t*)lpvStatusInformation, 15);
		printf("hSession Status: Successfully connected. IP Address: %s\n", szServerIP);
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTING_TO_SERVER:
		wcstombs(szServerIP, (wchar_t*)lpvStatusInformation, 15);
		printf("hSession Status: Attempting connection. IP Address: %s\n", szServerIP);
		break;
	case WINHTTP_CALLBACK_STATUS_CONNECTION_CLOSED:
		puts("hSession Status: Successfully closed the connection.");
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		dwBytes = (DWORD*)lpvStatusInformation;
		printf("hSession Status: Data available to read. %u bytes available.", *dwBytes);
		break;
	case WINHTTP_CALLBACK_STATUS_HANDLE_CREATED:
		puts("hSession Status: HINTERNET handle created.");
		break;
	case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
		puts("hSession Status: HINTERNET handle closed.");
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
		puts("hSession Status: Headers available to read.");
		break;
	case WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE:
		puts("hSession Status: Received an intermediate (100 level) status code message from the server");
		break;
	case WINHTTP_CALLBACK_STATUS_NAME_RESOLVED:
		std::wcout << "hSession Status: Found IP address of the endpoint server. IP: " << (wchar_t*)lpvStatusInformation << "\n";
		break;
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		puts("hSession Status: Data successfully read.");
		break;
	case WINHTTP_CALLBACK_STATUS_RECEIVING_RESPONSE:
		puts("hSession Status: Waiting for response.");
		break;
	case WINHTTP_CALLBACK_STATUS_REDIRECT:
		puts("hSession Status: Redirecting");
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		async_result = (WINHTTP_ASYNC_RESULT*)lpvStatusInformation;
		switch (async_result->dwResult) {
		case API_RECEIVE_RESPONSE:
			puts("HTTP Request error: The error occurred during a call to WinHttpReceiveResponse.");
			break;
		case API_QUERY_DATA_AVAILABLE:
			puts("HTTP Request error: The error occurred during a call to WinHttpQueryDataAvailable.");
			break;
		case API_READ_DATA:
			puts("HTTP Request error: The error occurred during a call to WinHttpReadData.");
			break;
		case API_WRITE_DATA:
			puts("HTTP Request error: The error occurred during a call to WinHttpWriteData.");
			break;
		case API_SEND_REQUEST:
			puts("HTTP Request error: The error occurred during a call to WinHttpSendRequest.");
			break;
		default:
			puts("HTTP Request error: INTERNAL ERROR, file get.cpp, line 116");
			break;
		}
		printf("hSession Status: Error occured while sending request. dwError (GetLastError) code: %u\n", async_result->dwError);
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_SENT:
		dwBytes = (DWORD*)lpvStatusInformation;
		printf("hSession Status: Data successfully sent. %u bytes sent.\n", *dwBytes);
		break;
	case WINHTTP_CALLBACK_STATUS_RESOLVING_NAME:
		std::wcout << "hSession Status: Resolving IP address. Server: " << (wchar_t*)lpvStatusInformation << "\n";
		break;
	case WINHTTP_CALLBACK_STATUS_RESPONSE_RECEIVED:
		dwBytes = (DWORD*)lpvStatusInformation;
		printf("hSession Status: Recieved successful response. %u bytes recieved.\n", *dwBytes);
		break;
	case WINHTTP_CALLBACK_STATUS_SECURE_FAILURE:
		puts("hSession Status: Cannot sign the SSL certificate from the server. (Read lpvStatusInformation for more info)");
		break;
	case WINHTTP_CALLBACK_STATUS_SENDING_REQUEST:
		puts("hSession Status: Sending request.");
		break;
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
		puts("hSession Status: Send request successfully.");
		break;
	}
}