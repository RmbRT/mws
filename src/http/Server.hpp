#ifndef __mws_http_server_hpp_defined
#define __mws_http_server_hpp_defined

#include <netlib/x/ConnectionListener.hpp>
#include "Connection.hpp"
#include "../Error.hpp"

#include <cstddef>

namespace mws
{
	namespace http
	{
		/** A Server that receives HTTP requests. */
		class Server : netlib::x::ConnectionListener
		{
		public:
			/** Creates a server that does not listen for connections yet. */
			Server() = default;
			/** Creates a server and listens to the given address.
			@param[in] address:
				The address to listen to.
			@throw NetworkError
				If the listening fails. */
			Server(
				netlib::SocketAddress const& address);

			/** Listens to the given address.
			@param[in] address:
				The address to listen to.
			@throw NetworkError
				If the listening fails. */
			void listen(
				netlib::SocketAddress const& address);

			using netlib::x::ConnectionListener::unlisten;
			using netlib::x::ConnectionListener::listening;

			/** Handles an incoming connection.
			@param[in] connection:
				The connection to handle. */
			virtual void handle(
				Connection && connection) = 0;

			/** Handles incoming connections in a blocking loop.
			@param[in] parallel_connections:
				The maximum number of connections to handle at the same time. Pass `0` or `1` for single threaded handling, one connection at a time, or pass more to allow multi threaded connection handling.
			@throw  */
			void handle_connections(
				std::size_t parallel_connections);

		protected:
			/** Accepts an incoming connection.
			@return
				The incoming connection.
			@throw NetworkError
				If no connection could be accepted. */
			Connection accept();
		};
	}
}

#endif