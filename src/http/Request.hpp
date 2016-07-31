#ifndef __mws_http_request_hpp_defined
#define __mws_http_request_hpp_defined

#include "../String.hpp"
#include "Message.hpp"

namespace mws
{
	namespace http
	{
		enum class RequestMethod
		{
			kOptions,
			kGet,
			kHead,
			kPost,
			kPut,
			kDelete,
			kTrace,
			kConnect,
			kInvalid
		};

		RequestMethod request_method(char const* name);

		class RequestFirstLine
		{
			String m_method;
			String m_path;
			String m_version;
		public:
			RequestFirstLine() = default;
			explicit RequestFirstLine(String const&line);
			static bool parse(String const& line, RequestFirstLine &out);
		};

		class Request
		{
			RequestFirstLine m_firstline;
			MessageHeader m_header;
			String m_body;
		public:
			
		};
	}
}

#endif