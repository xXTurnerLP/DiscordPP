#include "pch.h"

#include "../../httplib/src/HTTPMethod/HeaderTypes.h"

#include "../../httplib/src/HTTPMethod/get/get.hpp"

#ifdef _DEBUG
#pragma comment(lib, "../httplib/bin/Debug_x86/httplib_Debug_x86.lib")
#else
#pragma comment(lib, "../httplib/bin/Release_x86/httplib_Release_x86.lib")
#endif

int main() {
	//ToDo: Switch to WINDOWS sub, make gui with console to the side.
	
	httplib::GetRequest get(stdout);
	get.AddHeader(httplib::HeaderType::Authorization, "token");
	int nGet = get.SendRequest("discord.com/api/channels/678958544606724115/messages?limit=1");

	printf("HTTP/GET Method: %d\n\nData: %s\n\n\n\nHTTP/POST Method: %d\n\nData: %s\n\n\n\n", nGet, get.GetData(), 0, "null");
}