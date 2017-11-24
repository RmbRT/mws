namespace mws::http
{
	String const& RequestFirstLine::method() const
	{
		return m_method;
	}

	String const& RequestFirstLine::path() const
	{
		return m_path;
	}
	String const& RequestFirstLine::version() const
	{
		return m_version;
	}

	RequestFirstLine const& Request::firstline() const
	{
		return m_firstline;
	}

	MessageHeader const& Request::header() const
	{
		return m_header;
	}

	String const& Request::body() const
	{
		return m_body;
	}
}