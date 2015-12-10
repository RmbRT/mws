#include "Connection.hpp"

namespace mws
{
	namespace net
	{
		netlib::StreamSocket const& Connection::socket() const { return m_socket; }
		netlib::StreamSocket & Connection::socket() { return m_socket; }
	}
}