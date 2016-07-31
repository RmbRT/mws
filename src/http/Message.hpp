#ifndef __mws_http_message_hpp_defined
#define __mws_http_message_hpp_defined

#include <vector>
#include "../String.hpp"

namespace mws
{
	namespace http
	{
		/** A key / value pair for message headers. */
		class MessageHeaderField
		{
			String m_field_name;
			String m_field_value;
		public:
			MessageHeaderField(String field_name, String field_value);

			MessageHeaderField(MessageHeaderField &&);
			MessageHeaderField &operator=(MessageHeaderField &&);

			String const& name() const;
			String const& value() const;
		};


		/** Stores the options of a http message header. */
		class MessageHeader
		{
			std::vector<MessageHeaderField> m_fields;

			/** Looks up the field with the given name.
			@param[in] name: The name of the field that should be looked up.
			@return: The address of the looked up field, if exists, otherwise null. */
			String const * find(String const& name) const;
		public:
			MessageHeader() = default;
			MessageHeader(MessageHeader &&);
			MessageHeader &operator=(MessageHeader &&);

			/** Adds a field with the given name and value.
			@param[in] name:
				The name of the field that should be added.
			@param[in] value:
				The value of the field that should be added.
			@return
				true, if the field was added, false if it existed already. */
			bool add(String name, String value);
			/** Returns the value of the field with the given name.
			@param[in] name: The name of the field that should be looked up.
			@param[in] default_value: The value that should be returned if no field
				with the given name was found.
			@return
				If a field with the given name exists, then its value is returned. Otherwise, returns `default_value`. */
			String const &get(
				String const& name,
				String const& default_value) const;


			/** Converts the header to a string, where each line of text contains a
			field name and value and is terminated by CRLF (no blank line at the end
			of the string, so that it can be appended with additional header data). */
			String to_string() const;
		};

		/** A http message, with a status / request line, header and body. */
		class Message
		{
			/** The status / request line. */
			String m_start_line;
			/** The message header. */
			MessageHeader m_header;
			/** The message body. */
			String m_body;
		public:
			Message(
				String start_line,
				MessageHeader header,
				String body);

			Message(Message &&);
			Message &operator=(Message &&);

			String const& start_line() const;
			MessageHeader const& header() const;
			String const& body() const;

			String to_string() const;
		};
	}
}

#endif