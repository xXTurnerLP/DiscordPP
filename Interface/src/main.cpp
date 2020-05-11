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
	get.AddHeader(HeaderType::User_Agent, "Sry u just got hack'd :<");
	get.AddHeader(HeaderType::Referer, "Hacked by Turner :) - https://discord.gg/QP78V8B");
	get.SendRequest("discordapp.com");

	while (!GetAsyncKeyState(VK_ESCAPE)) {
		Sleep(5);
	}
	//ToDo: free handles after request is finished
}