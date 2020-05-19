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
	typedef int RESPONSE;
	typedef const char* Address;
	typedef const char* HeaderValue;
	typedef short Port;
	
	class GetRequest {
	public:
		GetRequest(FILE* temp_pSTDOUT);
		~GetRequest();

		RESPONSE SendRequest(Address szURI, Port nPort = 443);
		bool AddHeader(HeaderType Header, HeaderValue szValue);

		const char* GetData();

	private:
		std::string m_szHeaders;
		char** m_szData;
	};
}