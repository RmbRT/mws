#ifndef __mws_net_connection_hpp_defined
#define __mws_net_connection_hpp_defined

#include <netlib/Socket.hpp>
#include "..\String.hpp"

namespace mws
{
	namespace net
	{
		class Connection
		{
		protected:
			netlib::StreamSocket m_socket;
			netlib::StreamSocket const& socket() const;
			netlib::StreamSocket & socket();

			String receive();
			void send(String);

		public:
			Connection(netlib::StreamSocket &&socket);
			Connection(Connection &&);

			Connection & operator=(Connection&&);
			Connection & operator=(Connection const&) = delete;
		};
	}
}

#endif