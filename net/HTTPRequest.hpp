#ifndef __mws_net_httprequest_hpp_defined
#define __mws_net_httprequest_hpp_defined

#include "../String.hpp"
#include "HTTPMessage.hpp"

namespace mws
{
	namespace net
	{
		enum class HTTPRequestMethod
		{
			OPTIONS,
			GET,
			HEAD,
			POST,
			PUT,
			DELETE,
			TRACE,
			CONNECT,
			INVALID
		};

		HTTPRequestMethod request_method(char const* name);

		class HTTPRequestFirstLine
		{
			String m_method;
			String m_path;
			String m_version;
		public:
			HTTPRequestFirstLine() = default;
			explicit HTTPRequestFirstLine(String const&line);
			static bool parse(String const& line, HTTPRequestFirstLine &out);
		};

		class HTTPRequest
		{
			HTTPRequestFirstLine m_firstline;
			HTTPMessageHeader m_header;
			String m_body;
		public:
			
		};
	}
}

#endif