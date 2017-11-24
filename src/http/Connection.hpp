#ifndef __mws_http_connection_hpp_defined
#define __mws_http_connection_hpp_defined

#include "Request.hpp"
#include "Message.hpp"

#include <netlib/x/Connection.hpp>

namespace mws::http
{
	/** Connection. */
	class Connection : netlib::x::Connection
	{
		/** Receives a CRLF terminated line of characters.
		@throws NetworkError
			If there was an error while receiving data.
		@throws DataError
		@return
			The received line, without */
		String receive_line();

		/** @throws NetworkError */
		std::vector<netlib::x::byte_t> receive(size_t bytes);
		/** @throws NetworkError */
		netlib::x::byte_t receive();

	public:
		Connection() = default;
		/** Creates a connection from a netlib connection. */
		Connection(
			netlib::x::Connection && connection);

		using netlib::x::Connection::exists;
		using netlib::x::Connection::operator bool;

		/** Receives a http request.
		@throws NetworkError
			If there is an error in the data transmission.
		@throws DataError
			If the data received was not conforming to protocol.
		@return
			The received request. */
		Request receive_request();

		/** Sends a http message (response).
		@throws NetworkError
			If there is an error while sending data.
		@param[in] message:
			The message to send. */
		void send_message(
			Message const& message);
	};
}

#endif