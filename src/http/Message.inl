namespace mws::http
{
	String const& MessageHeaderField::name() const
	{
		return m_field_name;
	}

	String const& MessageHeaderField::value() const
	{
		return m_field_value;
	}

	String const& Message::start_line() const
	{
		return m_start_line;
	}

	MessageHeader const& Message::header() const
	{
		return m_header;
	}

	String const& Message::body() const
	{
		return m_body;
	}
}