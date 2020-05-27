#pragma once

#include "pch.h"

#include "../HTTPRequest.hpp"

namespace httplib {
	class PostRequest : public HTTPRequest {
	public:
		PostRequest(FILE* temp_pSTDOUT);

		RESPONSE SendRequest(const char* szData, Address szURI, Port nPort = 443);
	};
}