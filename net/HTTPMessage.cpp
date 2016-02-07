#include "HTTPMessage.hpp"

namespace mws
{
	namespace net
	{
		HTTPMessageHeaderField::HTTPMessageHeaderField(
			String field_name,
			String field_value):
			m_field_name(std::move(field_name)),
			m_field_value(std::move(field_value)) { }

		HTTPMessageHeaderField::HTTPMessageHeaderField(HTTPMessageHeaderField &&move):
			m_field_name(std::move(move.m_field_name)),
			m_field_value(std::move(move.m_field_value)) { }

		HTTPMessageHeaderField &HTTPMessageHeaderField::operator=(HTTPMessageHeaderField &&move)
		{
			if(this == &move)
				return *this;

			m_field_name = std::move(move.m_field_name);
			m_field_value = std::move(move.m_field_value);

			return *this;
		}

		String const& HTTPMessageHeaderField::name() const { return m_field_name; }
		String const& HTTPMessageHeaderField::value() const { return m_field_value; }


		String const * HTTPMessageHeader::find(String const& name) const
		{
			for(HTTPMessageHeaderField const& field: m_fields)
				if(field.name() == name)
					return &field.value();
			return nullptr;
		}

		HTTPMessageHeader::HTTPMessageHeader(HTTPMessageHeader &&move):
			m_fields(std::move(move.m_fields)) { }

		HTTPMessageHeader &HTTPMessageHeader::operator=(HTTPMessageHeader &&move)
		{
			if(this == &move)
				return *this;

			m_fields = std::move(move.m_fields);

			return *this;
		}

		bool HTTPMessageHeader::add(String name, String value)
		{
			if(find(name))
				return false;
			
			m_fields.push_back(
				HTTPMessageHeaderField(
					std::move(name),
					std::move(value)));

			return true;
		}

		String const& HTTPMessageHeader::get(String const& name, String const& default_value) const
		{
			if(String const * val = find(name))
				return *val;
			else
				return default_value;
		}

		String HTTPMessageHeader::to_string() const
		{
			size_t len = 0;
			for(HTTPMessageHeaderField const& field: m_fields)
				len += field.name().length() + field.value().length() + 4; // ': \r\n'
			String ret;
			ret.reserve(len);
			
			for(HTTPMessageHeaderField const& field: m_fields)
			{
				ret.append(field.name());
				ret.append(": ");
				ret.append(field.value());
				ret.append("\r\n");
			}
			return std::move(ret);
		}

		HTTPMessage::HTTPMessage(
			String start_line,
			HTTPMessageHeader header,
			String body):
			m_start_line(std::move(start_line)),
			m_header(std::move(header)),
			m_body(std::move(body)) { }

		HTTPMessage::HTTPMessage(HTTPMessage &&move):
			m_start_line(std::move(move.m_start_line)),
			m_header(std::move(move.m_header)),
			m_body(std::move(move.m_body)) { }

		String const& HTTPMessage::start_line() const
		{
			return m_start_line;
		}

		HTTPMessageHeader const& HTTPMessage::header() const
		{
			return m_header;
		}

		String const& HTTPMessage::body() const
		{
			return m_body;
		}

		String HTTPMessage::to_string() const
		{
			String header = m_header.to_string();
			size_t len =
				m_start_line.length() +
				header.length() +
				m_body.length() + 4;
			String ret;
			ret.reserve(len);

			ret.append(m_start_line).append("\r\n").
				append(header).append("\r\n").
				append(m_body);

			return std::move(ret);
		}

	}
}