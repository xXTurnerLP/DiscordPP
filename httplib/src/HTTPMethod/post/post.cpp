#include "pch.h"

#include "../HeaderTypes.h"

#include "post.hpp"

namespace httplib {
	PostRequest::PostRequest(FILE* temp_pSTDOUT) : HTTPRequest(temp_pSTDOUT) {}

	RESPONSE PostRequest::SendRequest(const char* szData, Address szURI, Port nPort) {
		return HTTPRequest::HTTPRequest_Internal(L"POST", szData, szURI, nPort);
	}
}