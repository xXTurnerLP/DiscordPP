#include "pch.h"

#include "../../httplib/src/HTTPMethod/HeaderTypes.h"

#include "../../httplib/src/HTTPMethod/get/get.hpp"
#include "../../httplib/src/HTTPMethod/post/post.hpp"

#ifdef _DEBUG
#pragma comment(lib, "../httplib/bin/Debug_x86/httplib_Debug_x86.lib")
#else
#pragma comment(lib, "../httplib/bin/Release_x86/httplib_Release_x86.lib")
#endif

int main() {
	//ToDo: Switch to WINDOWS sub, make gui with console to the side.
	
	httplib::GetRequest get(stdout);
	get.AddHeader(httplib::HeaderType::Authorization, "token");
	int nGet = get.SendRequest("discord.com/api/channels/707338290872844398/messages?limit=1");

	httplib::PostRequest post(stdout);
	post.AddHeader(httplib::HeaderType::Content_Type, "application/json");
	post.AddHeader(httplib::HeaderType::Authorization, "token");
	int nPost = post.SendRequest("{\"content\":\"Hello from WinHTTP!\"}", "discordapp.com/api/channels/707338290872844398/messages");

	printf("HTTP/GET Method: %d\n\nData: %s\n\n\n\nHTTP/POST Method: %d\n\nData: %s\n\n\n\n", nGet, get.GetData(), nPost, post.GetData());
}