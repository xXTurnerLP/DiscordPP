#include "pch.h"

enum class HeaderType : char {
	Accept,
	Accept_Encoding,
	Authorization,
	Connection,
	Content_Encoding,
	Content_Length,
	Content_Type,
	User_Agent,
	Upgrade,
	Referer
};

namespace httplib {
	typedef unsigned long RESPONSE;
	typedef const char* Address;
	typedef const char* HeaderValue;
	typedef short Port;
	
	class GetRequest {
	public:
		GetRequest(FILE* temp_pSTDOUT);
		RESPONSE SendRequest(Address szURI, Port nPort = 443);
		bool AddHeader(HeaderType Header, HeaderValue szValue);

	private:
		std::string m_szHeaders;
	};
}