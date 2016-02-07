#ifndef __mws_net_httpconnection_hpp_defined
#define __mws_net_httpconnection_hpp_defined

#include "Connection.hpp"
#include "../String.hpp"


namespace mws
{
	namespace net
	{
		class HTTPConnection : private Connection
		{
			String m_request;
		protected:
			void bad_request();
		public:
			HTTPConnection(netlib::StreamSocket &&);

			void receive_request();
			void send_answer();
			String const& request() const;
		};
	}
}

#endif