#include "pch.h"

#include "dataAvail.h"

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

void dataAvail_worker(RequestData* Data, LPVOID lpvDataLength, HINTERNET hRequest) {
	if (*(DWORD*)lpvDataLength > 0) {
		if (Data->szDataRead == nullptr) {
			Data->szDataRead = new char[*(DWORD*)lpvDataLength + 1];
			Data->szDataRead[*(DWORD*)lpvDataLength] = 0;

			WinHttpReadData(hRequest, Data->szDataRead, *(DWORD*)lpvDataLength, NULL);
		} else {
			char* temp_buffer = new char[strlen(Data->szDataRead) + 1];
			temp_buffer[strlen(Data->szDataRead)] = 0;
			memcpy(temp_buffer, Data->szDataRead, strlen(Data->szDataRead)); //-V575 (PVS-Studio FalseAlarm)
			delete[] Data->szDataRead;

			Data->szDataRead = new char[strlen(temp_buffer) + *(DWORD*)lpvDataLength + 1];
			Data->szDataRead[strlen(temp_buffer) + *(DWORD*)lpvDataLength] = 0;
			memcpy(Data->szDataRead, temp_buffer, strlen(temp_buffer));

			WinHttpReadData(hRequest, Data->szDataRead + strlen(temp_buffer), *(DWORD*)lpvDataLength, NULL);

			delete[] temp_buffer;
		}
	} else Data->m_nBitFlags += (~(Data->m_nBitFlags << 12) & 0b1000);
}