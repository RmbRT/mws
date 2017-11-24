#ifndef __mws_error_hpp_defined
#define __mws_error_hpp_defined

#include <stdexcept>
#include "String.hpp"

namespace mws
{
	class Error : public std::runtime_error
	{
		String m_what;
	public:
		Error(
			String type,
			String const& message,
			String const& detail = nullptr);

		char const * what() const noexcept override;
	};

	/** Thrown if there was an error in a network function. */
	class NetworkError : public Error
	{
	public:
		NetworkError(
			String const& message,
			String const& detail = nullptr);
	};

	/** Thrown if there was invalid data received from the client. */
	class DataError : public Error
	{
	public:
		DataError(
			String const& message,
			String const& detail = nullptr);
	};
}


#endif