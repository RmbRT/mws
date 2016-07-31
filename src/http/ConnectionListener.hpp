#ifndef __mws_http_connectionlistener_hpp_defined
#define __mws_http_connectionlistener_hpp_defined

#include "ConnectionListener.hpp"
#include "Connection.hpp"

namespace mws
{
	namespace http
	{
		class ConnectionListener : public ConnectionListener
		{
			void onNewConnection(netlib::StreamSocket &&) override;
		public:
			ConnectionListener(netlib::AddressFamily);
			ConnectionListener(netlib::SocketAddress const&);
		};
	}
}
#endif