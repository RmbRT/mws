#include "Error.hpp"

namespace mws
{
	Error::Error(
		String type,
		String const& message,
		String const& detail):
		std::runtime_error(""),
		m_what(std::move(type.append(": ").append(message)))
	{
		if(!detail.empty())
			m_what.append(" (").append(detail).append(')');
	}

	char const * Error::what() const noexcept
	{
		return m_what.c_str();
	}

	NetworkError::NetworkError(
		String const& message,
		String const& detail):
		Error("Network Error", message, detail)
	{
	}

	DataError::DataError(
		String const& message,
		String const& detail):
		Error("Data Error", message, detail)
	{
	}
}