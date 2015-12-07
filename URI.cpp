#include "URI.hpp"

namespace mws
{
	bool reserved(char c);
	bool unreserved(char c);
	bool excluded(char c);

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

	URI::URI() { }
	URI::URI(URI && move):
		m_scheme(std::move(move.m_scheme)),
		m_scheme_specific(std::move(move.m_scheme_specific)) { }

	URI & URI::operator=(URI && move)
	{
		if(this == &move)
			return *this;

		m_scheme = std::move(move.m_scheme);
		m_scheme_specific = std::move(move.m_scheme_specific);

		return *this;
	}

	URI::URI(std::string const& uri) { parse(uri, this); }


	bool URI::parse(std::string const& _original_uri, URI * out)
	{
		if(out)
			*out = std::move(URI());
		std::string uri;
		if(!percent_uri_unescape(percent_uri_escape(_original_uri, false, false), &uri))
			return false;

		size_t const scheme_end = uri.find(":");
		if(scheme_end == std::string::npos)
			return false;
		if(out)
			out->m_scheme = uri.substr(0,scheme_end);
		return SchemeSpecificPart::parse(
			uri.substr(scheme_end+1, std::string::npos),
			out? &out->m_scheme_specific : nullptr);
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

	bool excluded(char c)
	{
		static bool res[256] = { };
		static unsigned char const tvals[] = " <>#%\"{}|\\^[]`";
		static bool initialized = false;

		if(!initialized)
		{
			for(size_t i = _countof(res); i--;)
				res[i] = false;
			for(size_t i = 0; i<=0x1f; i++)
				res[i] = true;
			for(size_t i = 0;tvals[i];i++)
				res[tvals[i]] = true;

			res[0x7f] = true;
			initialized = true;
		}

		return res[(unsigned char)c];
	}

	inline unsigned char hex(char c)
	{
		assert(isxdigit((unsigned char) c));
		return isalpha((unsigned char) c) ? tolower((unsigned char) c) - 'a' + 0xa : c - '0';
	}

	inline unsigned char tohex(char c)
	{
		assert((unsigned char) c <= 0xf);
		return c < 0xa ? c + '0' : c-0xa + 'a';
	}

	std::string percent_uri_escape(std::string const& str, bool escape_reserved, bool escape_unreserved)
	{
		std::string out;
		for(size_t i = 0; i<str.length(); i++)
		{
			// do not escape?
			if(((!escape_reserved && reserved(str[i])) || (!escape_unreserved && unreserved(str[i]))) && !excluded(str[i]))
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


	bool percent_uri_unescape(std::string const& str, std::string * out)
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
						char const original = (hex(str[i+1])<<4) | hex(str[i+2]);
						if( original != '%' && (reserved(original) || unreserved(original) || original=='#'))
						{
							temp += original;
							i += 2;
						}
						else
							temp+=str[i];
					}
				else
					return false;
			}
			else if(out)
				temp += str[i];
		}

		if(out)
			*out = std::move(temp);
	}
	
	std::string const& URI::scheme() const { return m_scheme; }
	std::string const& URI::host() const { return m_host; }
	std::string const& URI::path() const { return m_path; }
	std::string const& URI::query() const { return m_query; }
	std::string const& URI::fragment() const { return m_fragment; }
	bool URI::valid() const { return m_valid; }


	std::string to_string(URI const& uri)
	{
		if(!uri.valid())
			return std::string();
		else return to_string(uri.scheme()).
			append("://").append(uri.host()).
			append("/").append(uri.path()).
			append("?").append(uri.query()).
			append("#").append(uri.fragment());
	}
}