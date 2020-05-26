#include "pch.h"

#include "dataAvail.h"

struct RequestData {
	// XXXXXXXXXXXX0000 = X (12bit): Reserved for HTTP RETURN STATUS CODE
	// XXXXXXXXXXXX0001 = request succsessfully finished + valid http return status code available
	// XXXXXXXXXXXX0010 = return SendRequest() function with 0 (Ivalid URI)
	// XXXXXXXXXXXX0100 = return SendRequest() function with -1 (SSL Certificate error)
	// XXXXXXXXXXXX1000 = is the request finished / should the function return
	USHORT m_nBitFlags;
	char* m_szData;
};

void dataAvail_worker(RequestData* Data, LPVOID lpvDataLength, HINTERNET hRequest) {
	if (*(DWORD*)lpvDataLength > 0) {
		if (Data->m_szData == nullptr) {
			Data->m_szData = new char[*(DWORD*)lpvDataLength + 1];
			Data->m_szData[*(DWORD*)lpvDataLength] = 0;

			WinHttpReadData(hRequest, Data->m_szData, *(DWORD*)lpvDataLength, NULL);
		} else {
			char* temp_buffer = new char[strlen(Data->m_szData) + 1];
			temp_buffer[strlen(Data->m_szData)] = 0;
			memcpy(temp_buffer, Data->m_szData, strlen(Data->m_szData)); //-V575 (PVS-Studio FalseAlarm)
			delete[] Data->m_szData;

			Data->m_szData = new char[strlen(temp_buffer) + *(DWORD*)lpvDataLength + 1];
			Data->m_szData[strlen(temp_buffer) + *(DWORD*)lpvDataLength] = 0;
			memcpy(Data->m_szData, temp_buffer, strlen(temp_buffer));

			WinHttpReadData(hRequest, Data->m_szData + strlen(temp_buffer), *(DWORD*)lpvDataLength, NULL);

			delete[] temp_buffer;
		}
	} else Data->m_nBitFlags += (~(Data->m_nBitFlags << 12) & 0b1000);
}