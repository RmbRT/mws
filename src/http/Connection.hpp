#ifndef __mws_http_connection_hpp_defined
#define __mws_http_connection_hpp_defined

#include <netlib/x/Connection.hpp>
#include "../String.hpp"


namespace mws
{
	namespace http
	{
		class Connection : private netlib::x::Connection
		{
		public:
			Connection(
				netlib::x::Connection &&);

			/** Processes the connection.
				Receives a http request, and sends the response. */
			void process();
		};
	}
}

#endif