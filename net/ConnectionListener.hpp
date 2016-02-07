#ifndef __mws_net_connectionlistener_hpp_defined
#define __mws_net_connectionlistener_hpp_defined

#include "Connection.hpp"
#include <netlib/Socket.hpp>

namespace mws
{
	namespace net
	{
		class ConnectionListener
		{
			netlib::StreamSocket m_listen;
			bool m_bound;
		protected:
			/* Called by @[listen] (blocking). Use to implement connection handlers.
			@param[in] socket: The socket of the incoming connection. */
			virtual void onNewConnection(netlib::StreamSocket &&socket) = 0;

		public:
			ConnectionListener(netlib::AddressFamily family);
			ConnectionListener(netlib::SocketAddress const& listen_addr);

			ConnectionListener(ConnectionListener &&);
			ConnectionListener &operator=(ConnectionListener &&);
			ConnectionListener(ConnectionListener const&) = delete;
			ConnectionListener &operator=(ConnectionListener const&) = delete;

			/* Whether the listen socket was successfully bound. */
			bool bound() const;
			/* Tries to bind the listen socket to the given address and port.
			@param[in] listen_addr: The port and address that should be bound to.
			@return: Whether the binding succeeded or not. */
			bool bind(netlib::SocketAddress const& listen_addr);

			/* Listens for incoming connections. Requires that an address was bound.
			Note: this is a blocking call. */
			bool listen();
		};
	}
}

#endif