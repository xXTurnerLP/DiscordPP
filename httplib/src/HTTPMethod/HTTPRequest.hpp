#pragma once

#include "pch.h"

#include "HeaderTypes.h"

namespace httplib {
	typedef const char* Address;
	typedef const char* HeaderValue;
	typedef const wchar_t* HTTPMethod;
	typedef const short Port;
	typedef const int RESPONSE;

	class HTTPRequest {
	public:
		HTTPRequest(FILE* temp_pSTDOUT);
		~HTTPRequest();

		bool AddHeader(HeaderType Header, HeaderValue szValue);

		const char* GetData();

	protected:
		RESPONSE HTTPRequest_Internal(HTTPMethod szMethod, const char* szData, Address szURI, Port nPort = 443);

		std::string m_szHeadersOut;

		struct requestData_t {
			// XXXXXXXXXXXX0000 = X (12bit): Reserved for HTTP RETURN STATUS CODE
			// XXXXXXXXXXXX0001 = request succsessfully finished + valid http return status code available
			// XXXXXXXXXXXX0010 = return SendRequest() function with 0 (Ivalid URI)
			// XXXXXXXXXXXX0100 = return SendRequest() function with -1 (SSL Certificate error)
			// XXXXXXXXXXXX1000 = is the request finished / should the function return
			USHORT nBitFlags;
			char* szDataRead;
			char* szDataWrite;
		} *m_RequestData;
	};
}