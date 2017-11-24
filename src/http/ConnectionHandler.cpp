#include "ConnectionHandler.hpp"

#include <cassert>

namespace mws::http
{
	ConnectionHandler::ConnectionHandler():
		m_handler(),
		m_available(true)
	{}

	ConnectionHandler::~ConnectionHandler()
	{
		if(m_handler.joinable())
			m_handler.join();
	}

	void ConnectionHandler::handle(
		Server * server,
		Connection && connection)
	{
		assert(available());
		m_available = false;
		m_handler = std::thread(
			handle_job,
			this,
			server,
			std::move(connection));
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