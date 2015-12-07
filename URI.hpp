#ifndef __mws_uri_hpp_defined
#define __mws_uri_hpp_defined

#include <string>

#include <netlib/NetLib.hpp>
namespace mws
{
	class URI;
	class Scheme;
	class SchemeSpecificPart;
	class HierarchyPart;


	std::string to_string(URI const& uri);
	/* Escapes a string, and additionally escapes unreserved characters as well.*/
	std::string percent_uri_escape(std::string const& str, bool escape_reserved, bool escape_unreserved);
	bool percent_uri_unescape(std::string const& str, std::string * out);

	
	/* Describes an Uniform Resource Identifier, as it is used by the web server when resources are requested.
	An URI consists of a scheme, usually denoting the protocol used to access the resource, a host name, a path, locating the resource on the host system, a query, and a fragment.
	*/
	class URI
	{
		Scheme m_scheme;
		SchemeSpecificPart m_scheme_specific;
	public:
		/* Default constructor, initializes the URI to invalid. */
		URI();
		URI(URI && move);
		URI & operator=(URI && move);

		/* Tries to parse an URI from a string.
		@see: URI::parse.*/
		URI(std::string const& uri);

		/* Tries to parse an URI from a string.
		@param[in] uri: the text representation of the URI.
		@param[out] out: the URI object to be parsed into. If null, it will be ignored.
		@return: true, if uri is a valid URI string, and out has successfully been initialized, otherwise false.*/
		static bool parse(std::string const& uri, URI *out);

		/* Returns the Scheme part of the URI. */
		Scheme const& scheme() const;
		/* Returns the Scheme specific part of the URI. */
		SchemeSpecificPart const& scheme_specific() const;
		/* Returns the validity of the URI. */
		bool valid() const;
	};

	class Scheme
	{
		std::string m_name;
		bool m_valid;
	public:
		Scheme();
		Scheme(std::string name);
		Scheme(Scheme && scheme);

		static bool parse(std::string const&, Scheme * out);

		std::string const& name() const;
		bool valid() const;
	};

	enum class SchemeSpecificPartType
	{
		HierarchyPart,
		OpaquePart
	};

	class SchemeSpecificPart
	{
		HierarchyPart m_hier_part;
		std::string m_opaque_part;

	public:
		SchemeSpecificPart();
		SchemeSpecificPart(std::string const& scheme_specific);

		static bool parse(std::string const& scheme_specific, SchemeSpecificPart * out);

		HierarchyPart const * hier_part() const;
		std::string const * opaque_part() const;

		bool valid() const;
	};

	
	enum class PathType
	{
		Net,
		Abs
	};
	class HierarchyPart
	{
		/* A hierarchy part of a URI must at least contain one of the following. */
		std::string
			m_authority, /* The authority part. */
			m_abs_path; /* The absolute part.*/
	public:
		
		PathType path_type() const;
	};

}

#endif