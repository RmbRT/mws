#include "Connection.hpp"
#include <type_traits>
#include <cstdio>

namespace mws
{
	namespace net
	{
		Connection::Connection(netlib::StreamSocket &&socket):
			m_socket(std::move(socket)) { }
		Connection::Connection(Connection &&move):
			m_socket(std::move(move.m_socket)) { }

		Connection &Connection::operator=(Connection &&move)
		{
			if(this == &move)
				return *this;

			m_socket = std::move(move.m_socket);

			return *this;
		}

		String Connection::receive()
		{
			String instr;
			char_t in[1024];
			while(m_socket.pending())
			{
				size_t c = m_socket.recv(in, sizeof(in)-1);
				in[c] = '\0';
				instr.append(in);
			}

			return std::move(instr);
		}

		void Connection::send(String str)
		{
			if(!m_socket.send(str.c_str(), str.length()))
				puts("error while sending string:"), puts(str.c_str());
		}


		netlib::StreamSocket const& Connection::socket() const { return m_socket; }
		netlib::StreamSocket & Connection::socket() { return m_socket; }
	}
}