#include "pch.h"

#include "../../httplib/src/get/get.hpp"

#ifdef _DEBUG
#pragma comment(lib, "../httplib/bin/Debug_x86/httplib_Debug_x86.lib")
#else
#pragma comment(lib, "../httplib/bin/Release_x86/httplib_Release_x86.lib")
#endif

int main() {
	//ToDo: Switch to WINDOWS sub, make gui with console to the side.
	
	httplib::GetRequest get(stdout);
	get.AddHeader(HeaderType::Accept, "*/*");
	int nGet = get.SendRequest("discordapp.com/api/invites/QP78V8B");

	printf("\n\nReturn Code: %d\n\nData: %s\n", nGet, get.GetData());
}