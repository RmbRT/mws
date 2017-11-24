#ifndef __mws_http_connectionhandler_hpp_defined
#define __mws_http_connectionhandler_hpp_defined

#include "Server.hpp"
#include "../defines.hpp"
#include <thread>

namespace mws::http
{
	class ConnectionHandler
	{
		std::thread m_handler;
		bool m_available;
	public:
		ConnectionHandler();
		~ConnectionHandler();

		MWS_INL bool available() const;

		void handle(
			Server * server,
			Connection && connection);
	private:
		static void handle_job(
			ConnectionHandler * handler,
			Server * server,
			Connection && connection);
	};
}

#include "ConnectionHandler.inl"

#endif