#include "URI.hpp"
#include <type_traits>

namespace mws
{
	URI::URI(URI && move) :
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

	URI::URI(String scheme, String scheme_specific) :
		m_scheme(std::move(scheme)),
		m_scheme_specific(std::move(scheme_specific)) { }

	URI::URI(String const& uri)
	{
		parse(uri, this);
	}

	String const& URI::scheme() const
	{
		return m_scheme;
	}

	String const& URI::scheme_specific() const
	{
		return m_scheme_specific;
	}

	bool URI::parse(String const& uri, URI * out)
	{
		if(auto const pos = uri.find_first(':'))
		{
			if(out)
			{
				out->m_scheme = uri.substring(0, uri.indexof(pos));
				out->m_scheme_specific = uri.substring(1+uri.indexof(pos));
			}
			return true;
		} else return false;
	}

	HierarchyPart::HierarchyPart() : m_valid(false) { }
	HierarchyPart::HierarchyPart(String const& hierarchy)
	{
		parse(hierarchy, this);
	}
	HierarchyPart::HierarchyPart(HierarchyPart && move) :
		m_userinfo(std::move(move.m_userinfo)),
		m_name(std::move(move.m_name)),
		m_port(std::move(move.m_port)),
		m_path(std::move(move.m_path)),
		m_query(std::move(move.m_query)),
		m_fragment(std::move(move.m_fragment)),
		m_valid(move.m_valid) { move.m_valid = false; }
	HierarchyPart & HierarchyPart::operator=(HierarchyPart && move)
	{
		if(this == &move)
			return *this;

		m_userinfo = std::move(move.m_userinfo);
		m_name = std::move(move.m_name);
		m_port = std::move(move.m_port);
		m_path = std::move(move.m_path);
		m_query = std::move(move.m_query);
		m_fragment = std::move(move.m_fragment);
		m_valid = move.m_valid;
		move.m_valid = false;

		return *this;
	}

	bool HierarchyPart::parse(String const& hierarchy, HierarchyPart * out)
	{
		if(out)
			out->m_userinfo = out->m_name = out->m_port = out->m_path = out->m_query = out->m_fragment = NULL, out->m_valid = false;

		static char_t const order[] = "@:/?#";

		for(size_t i = 0; i<_countof(order)-1; i++)
		{
			char_t const * pos_more = hierarchy.find_first(order[i], 2),
				* pos_less = hierarchy.find_first_of(order+i+1, 2);

			if(pos_more > pos_less && pos_less)
				return false;
		}


		if(hierarchy.indexof(hierarchy.find_first("//")) == 0)
		{
			if(out)
			{
				out->m_userinfo = hierarchy.substring(2);
				out->m_userinfo.split_on_r(out->m_userinfo.find_first('@'),1,out->m_name);
				out->m_name.split_on_l(out->m_name.find_first_of("/?#"),0,out->m_path);
				out->m_path.split_on_l(out->m_path.find_first_of("?#"),0,out->m_query);
				out->m_query.split_on_l(out->m_query.find_first_of("#"),1,out->m_fragment);

				if(out->m_name.contains(":"))
					out->m_name.split_on_l(out->m_name.find_first(':'), 1, out->m_port);

				out->m_path = out->m_path.substring(1);
				out->m_query = out->m_query.substring(1);

				out->m_valid = true;
			}
		}
		else if(hierarchy[0] == '/')
		{
			if(out)
			{
				out->m_path = hierarchy.substring(1);
				out->m_path.split_on_l(out->m_path.find_first_of("?#"),0,out->m_query);
				out->m_query.split_on_l(out->m_query.find_first_of("#"), 1, out->m_fragment);
				out->m_query = out->m_query.substring(1);

				out->m_valid = true;
			}
		}
		else
			return false;
		return true;
	}
	
	String const& HierarchyPart::userinfo() const { return m_userinfo; }
	String const& HierarchyPart::name() const { return m_name; }
	String const& HierarchyPart::port() const { return m_port; }
	String const& HierarchyPart::path() const { return m_path; }
	String const& HierarchyPart::query() const { return m_query; }
	String const& HierarchyPart::fragment() const { return m_fragment; }

	bool HierarchyPart::valid() const { return m_valid; }
}