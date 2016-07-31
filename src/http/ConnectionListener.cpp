#include "ConnectionListener.hpp"
#include <thread>
#include <netlib/netlib.hpp>

namespace mws
{
	namespace http
	{
		ConnectionListener::ConnectionListener(netlib::AddressFamily family):
			ConnectionListener(family) { }
		ConnectionListener::ConnectionListener(netlib::SocketAddress const& listen_addr):
			ConnectionListener(listen_addr) { }

		void ConnectionListener::onNewConnection(netlib::StreamSocket &&socket)
		{
			Connection conn(std::move(socket));
			conn.receive_request();
			std::printf("got request:\n%s(end of request.)\n", conn.request().c_str());
			conn.send_answer();
		}
	}
}