#include "pch.h"

#include "../HeaderTypes.h"

#include "get.hpp"

namespace httplib {
	GetRequest::GetRequest(FILE* temp_pSTDOUT) : HTTPRequest(temp_pSTDOUT) {}

	RESPONSE GetRequest::SendRequest(Address szURI, Port nPort) {
		return HTTPRequest::HTTPRequest_Internal(L"GET", nullptr, szURI, nPort);
	}
}