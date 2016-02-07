#ifndef __mws_net_httpconnectionlistener_hpp_defined
#define __mws_net_httpconnectionlistener_hpp_defined

#include "ConnectionListener.hpp"
#include "HTTPConnection.hpp"

namespace mws
{
	namespace net
	{
		class HTTPConnectionListener : public ConnectionListener
		{
			void onNewConnection(netlib::StreamSocket &&) override;
		public:
			HTTPConnectionListener(netlib::AddressFamily);
			HTTPConnectionListener(netlib::SocketAddress const&);
		};
	}
}
#endif