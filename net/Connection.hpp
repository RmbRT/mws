#ifndef __mws_net_connection_hpp_defined
#define __mws_net_connection_hpp_defined

#include <netlib/NetLib.hpp>
#include <string>

namespace mws
{
	namespace net
	{
		class Connection
		{
			netlib::StreamSocket m_socket;
			
		public:
			netlib::StreamSocket const& socket() const;
			netlib::StreamSocket & socket();
		};
	}
}

#endif