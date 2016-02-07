#include "HTTPRequest.hpp"
#include <cctype>
namespace mws
{
	namespace net
	{
		HTTPRequestMethod request_method(char const * name)
		{
			if(!name)
				return HTTPRequestMethod::INVALID;
			
			if(strcmp(name, "GET") == 0)
				return HTTPRequestMethod::GET;
			else if(strcmp(name, "OPTIONS"))
				return HTTPRequestMethod::OPTIONS;
			else if(strcmp(name, "HEAD"))
				return HTTPRequestMethod::HEAD;
			else if(strcmp(name, "POST"))
				return HTTPRequestMethod::POST;
			else if(strcmp(name, "DELETE"))
				return HTTPRequestMethod::DELETE;
			else if(strcmp(name, "TRACE"))
				return HTTPRequestMethod::TRACE;
			else if(strcmp(name, "CONNECT"))
				return HTTPRequestMethod::CONNECT;
			else
				return HTTPRequestMethod::INVALID;
			
		}

		HTTPRequestFirstLine::HTTPRequestFirstLine(String const& line)
		{
			parse(line, *this);
		}

		bool HTTPRequestFirstLine::parse(String const& line, HTTPRequestFirstLine &out)
		{
			return 3 == line.scanf("%s%!_%s%!_HTTP/%s",
				{ &out.m_method, &out.m_path, &out.m_version });
		}
	}
}