#include "Request.hpp"
#include "../Error.hpp"


#include <cctype>
#include <cstring>

namespace mws::http
{
	RequestMethod request_method(char const * name)
	{
		if(!name)
			return RequestMethod::kInvalid;

		if(strcmp(name, "GET") == 0)
			return RequestMethod::kGet;
		else if(strcmp(name, "OPTIONS") == 0)
			return RequestMethod::kOptions;
		else if(strcmp(name, "HEAD") == 0)
			return RequestMethod::kHead;
		else if(strcmp(name, "POST") == 0)
			return RequestMethod::kPost;
		else if(strcmp(name, "DELETE") == 0)
			return RequestMethod::kDelete;
		else if(strcmp(name, "TRACE") == 0)
			return RequestMethod::kTrace;
		else if(strcmp(name, "CONNECT") == 0)
			return RequestMethod::kConnect;
		else
			return RequestMethod::kInvalid;
	}

	RequestFirstLine::RequestFirstLine(
		String const& line)
	{
		if(!parse(line, *this))
			throw DataError("Could not parse request line", line);
	}

	bool RequestFirstLine::parse(String const& line, RequestFirstLine &out)
	{
		return 3 == line.scanf("%s %s HTTP/%s",
			{ &out.m_method, &out.m_path, &out.m_version });
	}

	Request::Request(
		RequestFirstLine firstline,
		MessageHeader header,
		String body):
		m_firstline(std::move(firstline)),
		m_header(std::move(header)),
		m_body(std::move(body))
	{
	}
}