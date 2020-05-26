#pragma once

#include "pch.h"

#include "../HTTPRequest.hpp"

namespace httplib {
	class GetRequest : public HTTPRequest {
	public:
		GetRequest(FILE* temp_pSTDOUT);

		RESPONSE SendRequest(Address szURI, Port nPort = 443);
	};
}