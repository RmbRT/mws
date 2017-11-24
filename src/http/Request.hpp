#ifndef __mws_http_request_hpp_defined
#define __mws_http_request_hpp_defined

#include "../String.hpp"
#include "Message.hpp"
#include "../Error.hpp"

namespace mws::http
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
		/** @throws DataError */
		explicit RequestFirstLine(String const&line);
		static bool parse(String const& line, RequestFirstLine &out);

		inline String const& method() const;
		inline String const& path() const;
		inline String const& version() const;
	};

	class Request
	{
		RequestFirstLine m_firstline;
		MessageHeader m_header;
		String m_body;
	public:
		Request(
			RequestFirstLine firstline,
			MessageHeader header,
			String body);

		inline RequestFirstLine const& firstline() const;
		inline MessageHeader const& header() const;
		inline String const& body() const;
	};
}

#include "Request.inl"

#endif