#ifndef __mws_uri_hpp_defined
#define __mws_uri_hpp_defined

#include "String.hpp"
#include <vector>
#include <cinttypes>

namespace mws
{
	class URI;
	class HierarchyPart;

	String to_string(URI const& uri);
	/* Escapes a string, and additionally escapes unreserved characters as well.*/
	String percent_uri_escape(String const& str, bool escape_reserved, bool escape_unreserved);
	bool percent_uri_unescape(String const& str, String * out);

	
	/* Describes an Uniform Resource Identifier, as it is used by the web server when resources are requested.
	An URI consists of a scheme, usually denoting the protocol used to access the resource, a host name, a path, locating the resource on the host system, a query, and a fragment.
	*/
	class URI
	{
		String
			m_scheme, /* The scheme part of the URI. */
			m_scheme_specific; /* The scheme specific part of the URI. */
	public:
		URI(URI && move);
		URI &operator=(URI && move);
		/* Constructs an URI from a scheme and a scheme specific part.*/
		URI(String scheme, String scheme_specific);
		/* Tries to parse an URI from a string.
		@see: URI::parse.*/
		URI(String const& uri);

		/* Tries to parse an URI from a string.
		@param[in] uri: the text representation of the URI.
		@param[out] out: the URI object to be parsed into. If null, it will be ignored.
		@return: true, if uri is a valid URI string, and out has successfully been initialized, otherwise false.*/
		static bool parse(String const& uri, URI * out);

		/* Returns the scheme part of the URI. */
		String const& scheme() const;
		/* Returns the scheme specific part of the URI. */
		String const& scheme_specific() const;
	};

	class HierarchyPart
	{
		String
			m_userinfo,
			m_name,
			m_port,
			m_path,
			m_query,
			m_fragment;
		public:
			HierarchyPart(String const& hierarchy);
			HierarchyPart(HierarchyPart && move);

			static bool parse(String const& hierarchy, HierarchyPart * out);
	};
}

#endif