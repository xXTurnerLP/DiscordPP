#include "pch.h"

#include "dataAvail.h"

namespace httplib {
	extern char* g_szDataBuffer;
}

void dataAvail_worker(unsigned short* pnBitfield, LPVOID lpvDataLength, HINTERNET hRequest) {
	if (*(DWORD*)lpvDataLength > 0) {
		if (httplib::g_szDataBuffer == nullptr) {
			httplib::g_szDataBuffer = new char[*(DWORD*)lpvDataLength + 1];
			httplib::g_szDataBuffer[*(DWORD*)lpvDataLength] = 0;

			WinHttpReadData(hRequest, httplib::g_szDataBuffer, *(DWORD*)lpvDataLength, NULL);
		} else {
			char* temp_buffer = new char[strlen(httplib::g_szDataBuffer) + 1];
			temp_buffer[strlen(httplib::g_szDataBuffer)] = 0;
			memcpy(temp_buffer, httplib::g_szDataBuffer, strlen(httplib::g_szDataBuffer));
			delete[] httplib::g_szDataBuffer;

			httplib::g_szDataBuffer = new char[strlen(temp_buffer) + *(DWORD*)lpvDataLength + 1];
			httplib::g_szDataBuffer[strlen(temp_buffer) + *(DWORD*)lpvDataLength] = 0;
			memcpy(httplib::g_szDataBuffer, temp_buffer, strlen(temp_buffer));

			WinHttpReadData(hRequest, httplib::g_szDataBuffer + strlen(temp_buffer), *(DWORD*)lpvDataLength, NULL);

			delete[] temp_buffer;
		}
	} else *pnBitfield += (~(*pnBitfield << 12) & 0b1000);
}