#ifndef __mws_net_httpmessage_hpp_defined
#define __mws_net_httpmessage_hpp_defined

#include <vector>
#include "../String.hpp"

namespace mws
{
	namespace net
	{
		class HTTPMessageHeaderField
		{
			String
				m_field_name,
				m_field_value;
		public:
			HTTPMessageHeaderField(String field_name, String field_value);

			HTTPMessageHeaderField(HTTPMessageHeaderField &&);
			HTTPMessageHeaderField &operator=(HTTPMessageHeaderField &&);

			String const& name() const;
			String const& value() const;
		};


		class HTTPMessageHeader
		{
			std::vector<HTTPMessageHeaderField> m_fields;

			/* Looks up the field with the given name.
			@param[in] name: The name of the field that should be looked up.
			@return: The address of the looked up field, if exists, otherwise null. */
			String const * find(String const& name) const;
		public:
			HTTPMessageHeader() = default;
			HTTPMessageHeader(HTTPMessageHeader &&);
			HTTPMessageHeader &operator=(HTTPMessageHeader &&);

			/* Adds a field with the given name and value, if no field with the given name
			exists yet.
			@param[in] name: The name of the field that should be added.
			@param[in] value: The value of the field that should be added.
			@return: true, if the field was added, false if it existed already.*/
			bool add(String name, String value);
			/* Returns the value of the field with the given name, if exists,
			otherwise, returns <default_value>.
			@param[in] name: The name of the field that should be looked up.
			@param[in] default_value: The value that should be returned if no field
				with the given name was found.
			@return: If a field with the given name exists, then its value is returned.
				Otherwise, returns <default_value>. */
			String const &get(String const& name, String const& default_value) const;

			/* Converts the header to a string, where each line of text contains a
			field name and value and is terminated by CRLF (no blank line at the end
			of the string, so that it can be appended with additional header data). */
			String to_string() const;
		};

		class HTTPMessage
		{
			String m_start_line;
			HTTPMessageHeader m_header;
			String m_body;
		public:
			HTTPMessage(
				String start_line,
				HTTPMessageHeader header,
				String body);

			HTTPMessage(HTTPMessage &&);
			HTTPMessage &operator=(HTTPMessage &&);

			String const& start_line() const;
			HTTPMessageHeader const& header() const;
			String const& body() const;

			String to_string() const;
		};
	}
}

#endif