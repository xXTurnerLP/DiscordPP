#include "pch.h"

#include "../../httplib/src/get/get.hpp"

#ifdef _DEBUG
#pragma comment(lib, "../httplib/bin/Debug_x86/httplib_Debug_x86.lib")
#else
#pragma comment(lib, "../httplib/bin/Release_x86/httplib_Release_x86.lib")
#endif

int main() {
	//ToDo: Switch to WINDOWS sub, make gui with console to the side.
	
	httplib::GetRequest get;
	get.AddHeader(HeaderType::Upgrade, "websocket");
}