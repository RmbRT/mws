#include "Server.hpp"
#include "ConnectionHandler.hpp"

#include <vector>

namespace mws::http
{
	Server::Server(
		netlib::SocketAddress const& address)
	{
		listen(address);
	}

	void Server::listen(
		netlib::SocketAddress const& address)
	{
		if(!netlib::x::ConnectionListener::listen(address))
			throw NetworkError("failed to listen to requested address");
	}

	void Server::handle_connections(
		std::size_t parallel_connections)
	{
		Connection c;
		if(parallel_connections > 1)
		{
			std::vector<ConnectionHandler> handlers(parallel_connections);

			while(listening())
			{
				if(!(c = accept()))
					continue;

				bool dispatched = false;
				do {
					for(auto & handler: handlers)
						if(handler.available())
						{
							handler.handle(this, std::move(c));
							dispatched = true;
							break;
						}

					if(!dispatched)
						std::this_thread::yield();
				} while(!dispatched);
			}
		} else
		{
			while(listening())
			{
				if(!(c = accept()))
					continue;

				handle(std::move(c));
			}
		}
	}

	Connection Server::accept()
	{
		netlib::x::Connection conn = netlib::x::ConnectionListener::accept();
		if(!conn.exists())
			throw NetworkError("Failed to accept connection.");
		else
			return std::move(conn);
	}
}