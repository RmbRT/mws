#include "String.hpp"
#include <memory>
#include <cassert>
#include <cctype>
#include <cstring>

namespace mws
{
	String::~String()
	{
		free();
	}
	String::String() : m_data(nullptr), m_capacity(0), m_size(0) { }
	String::String(String const& copy) : m_data(nullptr), m_size(0), m_capacity(0)
	{
		*this = copy;
	}
	String::String(String && move) : m_data(move.m_data), m_capacity(move.m_capacity), m_size(move.m_size)
	{
		move.invalidate();
	}
	String::String(char_t const * str) : m_data(nullptr), m_size(0), m_capacity(0)
	{
		*this = str;
	}
	String& String::operator=(String const& copy)
	{
		if(this == &copy)
			return *this;

		copy_content(0, copy.m_data, copy.m_size);
		m_size = copy.m_size;

		return *this;
	}

	String& String::operator=(String && move)
	{
		if(this == &move)
			return *this;
		
		free();

		m_data = move.m_data;
		m_size = move.m_size;
		m_capacity = move.m_capacity;

		move.invalidate();

		return *this;
	}

	String& String::operator=(char_t const * str)
	{
		m_size = str ? std::strlen(str) : 0;
		copy_content(0, str, m_size);
		return *this;
	}

	String& String::operator=(nullptr_t)
	{
		m_size = 0;
		copy_content(0, nullptr, 0);
		return *this;
	}

	bool String::operator==(char_t const * rhs) const
	{
		if(rhs == m_data)
			return true;

		if(!rhs)
			return !m_size;

		for(size_t i = 0; i<m_size; i++)
			if(rhs[i] != m_data[i])
				return false;

		return true;
	}

	bool String::operator!=(char_t const * rhs) const
	{
		if(rhs == m_data)
			return false;

		if(!rhs)
			return m_size;

		for(size_t i = 0; i<m_size; i++)
			if(rhs[i] != m_data[i])
				return true;

		return false;
	}

	bool String::operator==(String const& rhs) const
	{
		if(&rhs == this)
			return true;
		if(rhs.length() != length())
			return false;

		for(size_t i = 0; i<m_size; i++)
			if(rhs.m_data[i] != m_data[i])
				return false;

		return true;
	}

	bool String::operator!=(String const& rhs) const
	{
		if(&rhs == this)
			return false;

		for(size_t i = 0; i<m_size; i++)
			if(rhs.m_data[i] != m_data[i])
				return true;

		return false;
	}
	
	bool String::starts_with(String const& rhs) const
	{
		if(&rhs == this)
			return true;
		if(rhs.m_size > m_size)
			return false;
		
		for(size_t i = 0; i<rhs.m_size; i++)
			if(m_data[i] != rhs.m_data[i])
				return false;

		return true;
	}

	bool String::ends_with(String const& rhs) const
	{
		if(&rhs == this)
			return true;

		if(rhs.m_size > m_size)
			return false;

		for(size_t i = m_size - rhs.m_size; i<m_size; i++)
			if(m_data[i] != rhs.m_data[i])
				return false;

		return true;
	}

	char_t const * String::find_first(String const& rhs, size_t look_start) const
	{
		if(&rhs == this)
			return m_data;

		if(rhs.m_size > m_size)
			return nullptr;

		size_t const sz_diff = m_size - rhs.m_size;

		for(size_t i = look_start; i<=sz_diff; i++)
		{
			bool found = true;
			for(size_t j = 0; j<rhs.m_size; j++)
				if(m_data[i+j] != rhs.m_data[j])
				{
					found = false;
					break;
				}
			if(found)
				return &m_data[i];
		}

		return nullptr;
	}
	
	char_t const * String::find_first(char_t const * rhs, size_t look_start) const
	{
		if(rhs == m_data)
			return m_data;
		if(!rhs)
			return nullptr;

		size_t const rhs_size = std::strlen(rhs);
		if(rhs_size > m_size)
			return nullptr;

		size_t const sz_diff = m_size - rhs_size;

		for(size_t i = look_start; i<=sz_diff; i++)
		{
			bool found = true;
			for(size_t j = 0; j<rhs_size; j++)
				if(m_data[i+j] != rhs[j])
				{
					found = false;
					break;
				}
			if(found)
				return &m_data[i];
		}

		return nullptr;
	}

	char_t const * String::find_first(char_t rhs, size_t look_start) const
	{
		for(size_t i = look_start; i<m_size; i++)
			if(m_data[i] == rhs)
				return &m_data[i];
		return nullptr;
	}


	char_t const * String::find_last(String const& rhs, size_t look_before) const
	{
		if(&rhs == this)
			return m_data;

		if(rhs.m_size > m_size)
			return nullptr;
		
		if(look_before > m_size)
			look_before = m_size;

		for(size_t i = look_before - rhs.m_size+1; i--;)
		{
			bool found = true;
			for(size_t j = 0; j<rhs.m_size; j++)
				if(m_data[i+j] != rhs.m_data[j])
				{
					found = false;
					break;
				}
			if(found)
				return &m_data[i];
		}

		return nullptr;
	}

	char_t const * String::find_last(char_t const * rhs, size_t look_before) const
	{
		if(rhs == m_data)
			return m_data;
		if(!rhs)
			return nullptr;

		size_t const rhs_size = std::strlen(rhs);

		if(rhs_size > m_size)
			return nullptr;
		
		if(look_before > m_size)
			look_before = m_size;

		for(size_t i = look_before - rhs_size+1; i--;)
		{
			bool found = true;
			for(size_t j = 0; j<rhs_size; j++)
				if(m_data[i+j] != rhs[j])
				{
					found = false;
					break;
				}
			if(found)
				return &m_data[i];
		}

		return nullptr;
	}

	char_t const * String::find_last(char_t rhs, size_t look_before) const
	{
		if(look_before > m_size)
			look_before = m_size;

		for(size_t i = look_before; i--;)
			if(m_data[i] == rhs)
				return &m_data[i];

		return nullptr;
	}

	char_t const * String::find_first_of(char_t const * characters, size_t look_start) const
	{
		char_t const * min = nullptr;
		while(*characters)
			if(auto const pos = find_first(*characters++, look_start))
				if(pos < min || !min)
					min = pos;

		return min;
	}
	
	void String::split_on_r(char_t const * position, size_t delim_size, String &out)
	{
		split_on_r(indexof(position), delim_size, out);
	}

	
	void String::split_on_r(size_t position, size_t delim_size, String &out)
	{
		if(position < m_size)
		{
			out = substring(position+delim_size);
			m_size = position;
			m_data[m_size] = '\0';
		}
		else
			out = std::move(*this);
	}

	
	void String::split_on_l(char_t const * position, size_t delim_size, String &out)
	{
		split_on_l(indexof(position), delim_size, out);
	}

	
	void String::split_on_l(size_t position, size_t delim_size, String &out)
	{
		if(position < m_size)
		{
			out = substring(position+delim_size);
			m_size = position;
			m_data[m_size] = '\0';
		}
		else
			out = nullptr;
	}

	size_t String::scanf(char const * format, std::initializer_list<String *> const& out) const
	{
		char const * my = c_str();
		if(!format)
			return 0;
		size_t scanned;
		for(scanned = 0; *format; )
		{
			if(!*my)
				return scanned;
			switch(*format)
			{
			case '\0':
				{
					return scanned;
				} break;
			case '%':
				{
					bool suppress = false;
					if(format[1] == '!')
					{
						suppress = true;
						format++;
					}
					switch(format[1])
					{
					case '%':
						{
							if(*my == '%')
							{
								++my;
							} else return scanned;
						} break;
					case '_': // \s+
						{
							size_t count = 0;
							while(my[count] && std::isspace((unsigned char)(my[count])))
								count++;
							if(!suppress)
								if(count && out.size() > scanned && out.begin()[scanned])
									*out.begin()[scanned++] = substring(indexof(my), indexof(my+count));
								else
									return scanned;
							
							my += count;
						} break;
					case 's': // \S+
						{
							size_t count = 0;
							while(my[count] && !std::isspace((unsigned char)(my[count])))
								count++;
							if(!suppress)
								if(count && out.size() > scanned && out.begin()[scanned])
									*out.begin()[scanned++] = substring(indexof(my), indexof(my+count));
								else
									return scanned;
							my += count;
							
						} break;
					case '+': // .+
					case '*': // .*
						{
							char_t break_on = format[2];
							size_t count = 0;
							while(my[count] && (my[count] != break_on))
								count++;
							
							if(!count && format[1] == '+')
								return scanned;

							if(!suppress)
								if(count && out.size() > scanned && out.begin()[scanned])
									*out.begin()[scanned++] = substring(indexof(my), indexof(my+count));
								else
									return scanned;

								my += count;
						} break;
					default:
						{
							printf("scanf: invalid format : %s", format);
							return scanned;
						} break;
					}
					format += 2;
				} break;
			default:
				{
					if(*my++ != *format++)
						return scanned;
				} break;
			}
		}
		return scanned;
	}

	bool String::contains(String const& rhs) const
	{
		return find_first(rhs) != nullptr;
	}

	size_t String::indexof(char_t const * character) const
	{
		if(character >= m_data && character < m_data+m_size)
			return character-m_data;
		else
			return npos;
	}

	String& String::append(String const& other)
	{
		copy_content(m_size, other.m_data, other.m_size);

		return *this;
	}

	String& String::append(char_t const * other)
	{
		copy_content(m_size, other, std::strlen(other));

		return *this;
	}

	String& String::append(char_t other)
	{
		m_data[m_size] = other;
		reserve(++m_size);
		m_data[m_size] = '\0';

		return *this;
	}

	String String::substring(size_t start, size_t end) const
	{
		if(!m_data)
			return nullptr;

		if(start > end)
		{
			puts("String::scanf: start > end, intended?");
			return nullptr;
		}

		if(start>m_size)
			start = m_size;
		if(end>m_size)
			end = m_size;
		
		String str;
		str.copy_content(0, &m_data[start], end-start);
		if(str.m_data)
			str.m_data[str.m_size] = '\0';
		return std::move(str);
	}


	char_t const * String::c_str() const
	{
		return m_size ? m_data : "";
	}

	char_t & String::operator[](size_t index)
	{
		assert(index < m_size);
		return m_data[index];
	}
	char_t const& String::operator[](size_t index) const
	{
		assert(index < m_size);
		return m_data[index];
	}


	size_t String::length() const
	{
		return m_size;
	}

	bool String::empty() const
	{
		return !m_size;
	}

	void String::invalidate()
	{
		m_data = nullptr;
		m_size = m_capacity = 0;
	}

	void String::reserve(size_t capacity)
	{
		if(capacity > m_capacity)
		{
			m_capacity = capacity;
			m_data = (char_t *)std::realloc(m_data, m_capacity + 1);
			if(!m_size) m_data[m_size] = '\0';
			m_data[m_capacity] = '\0';
		}
	}

	void String::resize(size_t size)
	{
		reserve(size);
		m_size = size;
	}

	void String::clear()
	{
		copy_content(0,nullptr,0);
	}

	void String::copy_content(size_t offset, char const * data, size_t size)
	{
		reserve(offset+size);
		if(!data)
			data = "";

		if(offset + size > m_size)
			m_size = offset + size;
		if(m_data)
			std::memcpy(m_data+offset, data, size+1); // also copy zero-byte.
	}

	void String::free()
	{
		if(m_data)
			std::free(m_data);
	}
}