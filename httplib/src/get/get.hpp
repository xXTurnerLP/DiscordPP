#include <Windows.h>

namespace http_get {
	typedef unsigned long RESPONSE;

	class Get {
	public:
		RESPONSE SendRequest();
		void AddHeader();

		//have to make everything ASYNC and make callback functions + lambda functions (sendrequest(lambda here for the managing when the server responses, what to do with this data)
	};
}