#include "HTTPConnectionListener.hpp"
#include <thread>
#include <netlib/NetLib.hpp>

namespace mws
{
	namespace net
	{
		HTTPConnectionListener::HTTPConnectionListener(netlib::AddressFamily family):
			ConnectionListener(family) { }
		HTTPConnectionListener::HTTPConnectionListener(netlib::SocketAddress const& listen_addr):
			ConnectionListener(listen_addr) { }

		void HTTPConnectionListener::onNewConnection(netlib::StreamSocket &&socket)
		{
			HTTPConnection conn(std::move(socket));
			conn.receive_request();
			std::printf("got request:\n%s(end of request.)\n", conn.request().c_str());
			conn.send_answer();
		}
	}
}