#include "URL.hpp"

namespace mws
{
	bool reserved(char c);
	bool unreserved(char c);

	bool compare_case_insensitive(std::string const& stra, char const* strb)
	{
		if(!strb)
			return false;
		if(strlen(strb) != stra.length())
			return false;

		for(size_t i = 0; i<stra.size(); i++)
		{
			if(!strb[i])
				return false;

			if(tolower((unsigned char)stra[i]) != tolower((unsigned char) strb[i]))
				return false;
		}
		return true;
	}
	Scheme parse_scheme(std::string const& string)
	{
		if(compare_case_insensitive(string, "http")) return Scheme::HTTP;
		if(compare_case_insensitive(string, "https")) return Scheme::HTTPS;
		if(compare_case_insensitive(string, "ftp")) return Scheme::FTP;

		return Scheme::Invalid;
	}

	URL::URL(Scheme scheme, std::string host, std::string path, std::string query, std::string fragment, bool valid):
		m_scheme(scheme),
		m_host(std::move(host)),
		m_path(std::move(path)),
		m_query(std::move(query)),
		m_fragment(std::move(fragment)),
		m_valid(valid) { }

	URL::URL() : m_scheme(Scheme::Invalid), m_valid(false) { }
	URL::URL(URL && move):
		m_scheme(std::move(move.m_scheme)),
		m_host(std::move(move.m_host)),
		m_path(std::move(move.m_path)),
		m_query(std::move(move.m_query)),
		m_fragment(std::move(move.m_fragment)),
		m_valid(move.m_valid) { }

	URL & URL::operator=(URL && move)
	{
		if(this == &move)
			return *this;

		m_scheme = move.m_scheme;
		m_host = std::move(move.m_host);
		m_path = std::move(move.m_path);
		m_query = std::move(move.m_query);
		m_fragment = std::move(move.m_fragment);
		m_valid = move.m_valid;

		return *this;
	}

	URL::URL(std::string const& url) { parse(url, this); }


	bool URL::parse(std::string const& _original_url, URL * out)
	{
		std::string url;
		if(!percent_url_unescape(_original_url, &url))
		{
			if(out)
				out->m_valid = false;
			return false;
		}


		if(out)
			out->m_valid = true;

		size_t const scheme_end = url.find("://");
		if(scheme_end == std::string::npos)
		{
			if(out)
				out->m_valid = false;
			return false;
		}
		
		Scheme const scheme = parse_scheme(url.substr(0,scheme_end));
		if(scheme == Scheme::Invalid)
		{
			if(out)
				out->m_valid = false;
			return false;
		}

		else if(out)
			out->m_scheme = scheme;

		size_t const host_begin = scheme_end + 3;
		if(url.length() <= host_begin)	
		{
			if(out)
				out->m_valid = false;
			return false;
		}

		size_t const host_end = url.find_first_of("/?#", host_begin);
		if(host_end == std::string::npos)
		{
			if(out)
			{
				out->m_host = url.substr(host_begin);
				out->m_path.clear();
				out->m_query.clear();
				out->m_fragment.clear();
			}
			return true;
		} else if(out)
			out->m_host = url.substr(host_begin, host_end-host_begin);
		
		size_t const path_begin = host_end + 1;
		if(url.length() <= path_begin)
		{
			if(out)
			{
				out->m_path.clear();
				out->m_query.clear();
				out->m_fragment.clear();
			}
			return true;
		}

		size_t const path_end = url.find_first_of("?#", path_begin);
		if(path_end == std::string::npos)
		{
			if(out)
			{
				out->m_path = url.substr(path_begin);
				out->m_query.clear();
				out->m_fragment.clear();
			}
			return true;
		} else if(out)
			out->m_path = url.substr(path_begin, path_end-path_begin);


		size_t const query_begin = path_end + 1;
		if(url.length() <= query_begin)
		{
			if(out)
			{
				out->m_query.clear();
				out->m_fragment.clear();
			}
			return true;
		}
		
		size_t const query_end = url.find('#', query_begin);
		if(query_end == std::string::npos)
		{
			if(out)
			{
				out->m_query = url.substr(query_begin);
				out->m_fragment.clear();
			}
			return true;
		} else if(out)
			out->m_query = url.substr(query_begin, query_end-query_begin);

		size_t const fragment_begin = query_end + 1;
		if(url.length() <= fragment_begin)
		{
			if(out)
				out->m_fragment.clear();
			return true;
		} else if(out)
			out->m_fragment = url.substr(fragment_begin);

		return true;
	}

	bool reserved(char c)
	{
		static bool res[256] = { };
		static unsigned char const tvals[] = ";/?:@&=+$,";
		static bool initialized = false;

		if(!initialized)
		{
			for(size_t i = _countof(res); i--;)
				res[i] = false;
			for(size_t i = 0;tvals[i];i++)
				res[tvals[i]] = true;

			initialized = true;
		}

		return res[(unsigned char)c];
	}

	bool unreserved(char c)
	{
		static bool res[256] = { };
		static unsigned char const tvals[] =
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"0123456789"
			"-_.!~*'()";
		static bool initialized = false;

		if(!initialized)
		{
			for(size_t i = _countof(res); i--;)
				res[i] = false;
			for(size_t i = 0;tvals[i];i++)
				res[tvals[i]] = true;

			initialized = true;
		}

		return res[(unsigned char)c];
	}

	inline unsigned char hex(char c)
	{
		return isalpha((unsigned char) c) ? tolower((unsigned char) c) - 'a' : c - '0';
	}

	inline unsigned char tohex(char c)
	{
		return c < 0xa ? c + '0' : c + 'a';
	}

	std::string percent_url_escape(std::string const& str, bool escape_unreserved)
	{
		std::string out;
		for(size_t i = 0; i<str.length(); i++)
		{
			// do not escape?
			if(reserved(str[i]) || (!escape_unreserved && unreserved(str[i])))
				out += str[i];
			// it was escaped already? just append.
			else if(str[i] == '%')
			{
				out+=str[i];
				out+=str[i+1];
				out+=str[i+2];
				i+=2;
			}
			// escape it.
			else
			{
				out+='%';
				out+=tohex((unsigned char)str[i] >> 4);
				out+=tohex((unsigned char)str[i] & 0xf);
			}
		}
		return out;
	}

	void URL::escape()
	{
		if(!m_valid)
			return;
		
		m_host = percent_url_escape(m_host, true);
		m_path = percent_url_escape(m_path, true);
		m_query = percent_url_escape(m_query, true);
		m_fragment = percent_url_escape(m_fragment, true);
	}

	bool URL::unescape()
	{
		if(!m_valid)
			return false;

		return m_valid = percent_url_unescape(m_host, &m_host)
			&& percent_url_unescape(m_path, &m_path)
			&& percent_url_unescape(m_query, &m_query)
			&& percent_url_unescape(m_fragment, &m_fragment);
	}

	bool percent_url_unescape(std::string const& str, std::string * out)
	{
		std::string temp;

		for(size_t i = 0; i < str.length(); i++)
		{
			if(str[i] == '%')
			{
				if(str.length() <= i+2)
					return false;

				if(isxdigit((unsigned char)str[i+1]) && isxdigit((unsigned char)str[i+2]))
					if(out)
					{
						char const original = (hex(str[i+1])<<4) + hex(str[i+2]);
						if( original != '%' && reserved(original) || unreserved(original))
							temp += original;
						i += 2;
					}
				else
					return false;
				i+=2;
			}
			else if(out)
				temp += str[i];
		}

		if(out)
			*out = std::move(temp);
	}
	
	Scheme URL::scheme() const { return m_scheme; }
	std::string const& URL::host() const { return m_host; }
	std::string const& URL::path() const { return m_path; }
	std::string const& URL::query() const { return m_query; }
	std::string const& URL::fragment() const { return m_fragment; }
	bool URL::valid() const { return m_valid; }

	std::string to_string(Scheme scheme)
	{
		switch(scheme)
		{
		case mws::Scheme::Invalid:
			return std::string();
			break;
		case mws::Scheme::HTTP:
			return "http";
			break;
		case mws::Scheme::HTTPS:
			return "https";
			break;
		case mws::Scheme::FTP:
			return "fpt";
			break;
		default:
			return std::string();
			break;
		}
	}
}