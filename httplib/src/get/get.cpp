#include "pch.h"

#include "get.hpp"

void _appendToHeader(std::string& private_member, const char* szValue, const char* szHeaderName);

namespace httplib {
	RESPONSE GetRequest::SendRequest(Address szURI, Port nPort) {
		return 0;
	}

	bool GetRequest::AddHeader(HeaderType Header, HeaderValue szValue) {
		switch (Header) {
		case HeaderType::Accept:
			_appendToHeader(m_szHeaders, (char*)szValue, "Accept");
			break;
		case HeaderType::Accept_Encoding:
			_appendToHeader(m_szHeaders, (char*)szValue, "Accept-Encoding");
			break;
		case HeaderType::Authorization:
			_appendToHeader(m_szHeaders, (char*)szValue, "Authorization");
			break;
		case HeaderType::Connection:
			_appendToHeader(m_szHeaders, (char*)szValue, "Connection");
			break;
		case HeaderType::Content_Encoding:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Encoding");
			break;
		case HeaderType::Content_Length:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Length");
			break;
		case HeaderType::Content_Type:
			_appendToHeader(m_szHeaders, (char*)szValue, "Content-Type");
			break;
		case HeaderType::User_Agent:
			_appendToHeader(m_szHeaders, (char*)szValue, "User-Agent");
			break;
		case HeaderType::Upgrade:
			_appendToHeader(m_szHeaders, (char*)szValue, "Upgrade");
			break;
		default:
			return false;
		}
		
		return true;
	}
}

void _appendToHeader(std::string &private_member, const char* szValue, const char* szHeaderName) {
	private_member += szHeaderName;
	private_member += ":";
	private_member += szValue;
	private_member += "\r\n";
}