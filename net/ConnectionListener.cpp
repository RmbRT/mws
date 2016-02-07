#include "ConnectionListener.hpp"
#include <type_traits>
#include <cstdio>
#include <thread>

namespace mws
{
	namespace net
	{
		ConnectionListener::ConnectionListener(netlib::AddressFamily family):
			m_listen(family),
			m_bound(false) { }

		ConnectionListener::ConnectionListener(netlib::SocketAddress const& listen_addr):
			m_listen(listen_addr.family),
			m_bound(false)
		{
			bind(listen_addr);
		}

		bool ConnectionListener::bind(netlib::SocketAddress const& listen_addr)
		{
			if(!m_listen.exists())
			{
				m_listen = std::move(netlib::StreamSocket(listen_addr.family));
			}
			if(m_bound)
				return false;
			
			return m_bound = m_listen.bind(listen_addr);
		}

		bool ConnectionListener::listen()
		{
			if(!m_bound || !m_listen.listen())
				return false;

			while(true)
			{
				if(m_listen.pending())
				{
					std::thread t([](ConnectionListener *listener) {
						netlib::StreamSocket out;
						listener->m_listen.accept(out);

						netlib::SocketAddress sa = out.address();
						if(sa.family == netlib::AddressFamily::IPv4)
							printf("new connection: %u.%u.%u.%u:%u\n",
							sa.address.ipv4.address.d0,
							sa.address.ipv4.address.d1,
							sa.address.ipv4.address.d2,
							sa.address.ipv4.address.d3,
							sa.address.ipv4.port);

						listener->onNewConnection(std::move(out));
					}, this);
					t.detach();
				}
			}
		}
	}
}