#include "ConnectionHandler.hpp"

#include <cassert>
#include <thread>

namespace mws::http
{
	ConnectionHandler::ConnectionHandler():
		m_available(true)
	{
	}

	ConnectionHandler::~ConnectionHandler()
	{
	}

	void ConnectionHandler::handle(
		Server * server,
		Connection && connection)
	{
		assert(available());

		m_available = false;
		std::thread(
			handle_job,
			this,
			server,
			std::move(connection)).detach();
	}

	void ConnectionHandler::handle_job(
		ConnectionHandler * handler,
		Server * server,
		Connection && connection)
	{
		assert(handler != nullptr);
		assert(server != nullptr);
		assert(connection.exists());

		server->handle(std::move(connection));
		handler->m_available = true;
	}
}