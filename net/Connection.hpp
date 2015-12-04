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
			std::string m_base_directory;
			std::string m_host_name;
			std::string m_path;
			std::string m_
			void read_request();
		};
	}
}

#endif