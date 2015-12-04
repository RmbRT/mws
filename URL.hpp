#ifndef __mws_uri_hpp_defined
#define __mws_uri_hpp_defined

#include <string>

#include <netlib/NetLib.hpp>
namespace mws
{
	enum class Scheme : unsigned char
	{
		/* Invalid / uninitialized scheme. */
		Invalid,
		/* HTTP scheme. */
		HTTP,
		/* HTTPS scheme. */
		HTTPS,
		/* FTP scheme. */
		FTP
	};

	class URL;


	/* Parses a scheme from a string.
	@param[in] string: the scheme string.
	@return: Scheme::Invalid, if the scheme string could not be recognized, otherwise its respective Scheme enum value.*/
	Scheme parse_scheme(std::string const& string);
	std::string to_string(Scheme scheme);

	std::string to_string(URL const& url);
	/* Escapes a string, and additionally escapes unreserved characters as well.*/
	std::string percent_url_escape(std::string const& str, bool escape_unreserved);
	bool percent_url_unescape(std::string const& str, std::string * out);

	


	/* Describes an Uniform Resource Locator, as it is used by the web server when resources are requested.
	An URL consists of a scheme, usually denoting the protocol used to access the resource, a host name, a path, locating the resource on the host system, a query, and a fragment.
	*/
	class URL
	{
		/* The scheme part of the URL. */
		Scheme m_scheme;
		std::string
			m_host, /* The host part of the URL. */
			m_path, /* The path part of the URL. */
			m_query, /* The query part of the URL. */
			m_fragment; /* The fragment part of the URL. */
		/* Indicates whether the URL has a valid value. */
		bool m_valid;

		URL(Scheme scheme, std::string host, std::string path, std::string query, std::string fragment, bool valid);
	public:
		/* Default constructor, initializes the URL to invalid. */
		URL();
		URL(URL && move);
		URL & operator=(URL && move);

		/* Tries to parse an URL from a string.
		@see: URL::parse.*/
		URL(std::string const& url);

		/* Tries to parse an URL from a string.
		@param[in] url: the text representation of the URL.
		@param[out] out: the URL object to be parsed into. If null, it will be ignored.
		@return: true, if url is a valid URL string, and out has successfully been initialized, otherwise false.*/
		static bool parse(std::string const& url, URL *out);


		/* Escapes the URL. */
		void escape();
		/* Unescapes the URL. returns whether the URL is valid after unescaping. */
		bool unescape();

		/* Returns the Scheme part of the URL. */
		Scheme scheme() const;
		/* Returns the host part of the URL. */
		std::string const& host() const;
		/* Returns the path part of the URL. */
		std::string const& path() const;
		/* Returns the query part of the URL. */
		std::string const& query() const;
		/* Returns the fragment part of the URL. */
		std::string const& fragment() const;
		/* Returns the validity of the URL. */
		bool valid() const;
	};
}

#endif