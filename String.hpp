#ifndef __mws_string_hpp_defined
#define __mws_string_hpp_defined

#include <crtdefs.h>

namespace mws
{
	typedef char char_t;
	
	class String
	{
		char_t * m_data;
		size_t m_capacity;
		size_t m_size;
	public:
		static size_t const npos = ~0;

		~String();
		String();
		String(String const& copy);
		String(String && move);
		String(char_t const * str);
		String& operator=(String const& copy);
		String& operator=(String && move);
		String& operator=(char_t const * str);

		bool operator==(char_t const * rhs) const;
		bool operator!=(char_t const * rhs) const;
		bool operator==(String const& rhs) const;
		bool operator!=(String const& rhs) const;

		bool starts_with(String const& rhs) const;
		bool ends_with(String const& rhs) const;
		char_t const * find_first(String const& rhs, size_t look_start = 0) const;
		char_t const * find_first(char_t const * rhs, size_t look_start = 0) const;
		char_t const * find_first(char_t rhs, size_t look_start = 0) const;
		char_t const * find_last(String const& rhs, size_t look_before = npos) const;
		char_t const * find_last(char_t const * rhs, size_t look_before = npos) const;
		char_t const * find_last(char_t rhs, size_t look_before = npos) const;

		char_t const * find_first_of(char_t const * characters, size_t look_start = 0) const;
		
		void split_on_r(char_t const* position, size_t delim_size, String &out);
		void split_on_r(size_t position, size_t delim_size, String &out);
		void split_on_l(char_t const* position, size_t delim_size, String &out);
		void split_on_l(size_t position, size_t delim_size, String &out);

		bool contains(String const& rhs) const;

		size_t indexof(char_t const * character) const;

		String& append(String const& other);
		String& append(char_t const * other);
		String& append(char_t other);

		String substring(size_t start, size_t end = npos) const;

		char_t const * c_str() const;

		char_t & operator[](size_t index);
		char_t const& operator[](size_t index) const;

		size_t length() const;
		bool empty() const;

		void reserve(size_t capacity);
	private:
		void invalidate();
		void copy_content(size_t offset, char const * data, size_t size);
		void free();
	};
}


#endif