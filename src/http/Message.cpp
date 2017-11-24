#include "Message.hpp"

#include <string>

namespace mws::http
{
	MessageHeaderField::MessageHeaderField(
		String field_name,
		String field_value):
		m_field_name(std::move(field_name)),
		m_field_value(std::move(field_value))
	{
	}

	String const * MessageHeader::find(
		char const * name) const
	{
		for(MessageHeaderField const& field: m_fields)
			if(field.name() == name)
				return &field.value();
		return nullptr;
	}

	bool MessageHeader::add(
		String name,
		String value)
	{
		if(find(name.c_str()))
			return false;

		m_fields.push_back(
			MessageHeaderField(
				std::move(name),
				std::move(value)));

		return true;
	}

	String const& MessageHeader::get(
		char const * name,
		String const& default_value) const
	{
		if(String const * val = find(name))
			return *val;
		else
			return default_value;
	}

	String MessageHeader::to_string() const
	{
		size_t len = 0;
		for(MessageHeaderField const& field: m_fields)
			len += field.name().length() + field.value().length() + 4; // ': \r\n'
		String ret;
		ret.reserve(len);

		for(MessageHeaderField const& field: m_fields)
		{
			ret.append(field.name()).append(": ");
			ret.append(field.value()).append("\r\n");
		}
		return std::move(ret);
	}

	Message::Message(
		String start_line,
		MessageHeader header,
		String body):
		m_start_line(std::move(start_line)),
		m_header(std::move(header)),
		m_body(std::move(body))
	{
	}

	String Message::to_string() const
	{
		String header = m_header.to_string();

		String length;
		if(!m_header.find("Content-Length") && !m_body.empty())
		{
			length = "Content-Length: ";
			length.append(std::to_string(m_body.length()).c_str());
			length.append("\r\n");
		}

		size_t len =
			m_start_line.length() +
			header.length() +
			length.length() +
			m_body.length() + 4;
		String ret;
		ret.reserve(len);

		ret.append(m_start_line).append("\r\n").
			append(header).append(length).append("\r\n").
			append(m_body);

		return std::move(ret);
	}
}