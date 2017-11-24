#include "Connection.hpp"

#include <cstdlib>

namespace mws::http
{
	Connection::Connection(
		netlib::x::Connection && connection):
		netlib::x::Connection(std::move(connection))
	{
	}

	String Connection::receive_line()
	{
		String line;

		std::vector<netlib::x::byte_t> buffer;
		for(;;)
		{
			netlib::x::byte_t in = receive();
			if(in == '\r')
			{
				if(receive() == '\n')
					return std::move(line);
				else
					throw DataError("Expected \\n after \\r");
			}
			else
				line.append(in);
		}
	}

	std::vector<netlib::x::byte_t> Connection::receive(size_t bytes)
	{
		std::vector<netlib::x::byte_t> ret;

		if(!netlib::x::Connection::receive(ret, bytes))
			throw NetworkError("Failed to receive data.");

		return std::move(ret);
	}

	netlib::x::byte_t Connection::receive()
	{
		return receive(1).front();
	}

	Request Connection::receive_request()
	{
		RequestFirstLine firstline(receive_line());

		MessageHeader header;
		for(;;)
		{

			String line = receive_line();

			if(line.empty())
				break;

			String key, value;
			if(2 != line.scanf("%+:%!_%+", { &key, &value }))
				throw DataError("Invalid header line", line.append("|").append(key).append('|').append(value));
			header.add(key, value);
		}


		String const * content_length = header.find("Content-Length");
		String contents;
		if(content_length)
		{
			size_t bytes = std::atoll(content_length->c_str());

			std::vector<netlib::x::byte_t> buffer = receive(bytes);

			contents.resize(buffer.size());
			for(size_t i = buffer.size(); i--;)
				contents[i] = buffer[i];
		}

		return Request(
			std::move(firstline),
			std::move(header),
			std::move(contents));
	}

	void Connection::send_message(
		Message const& message)
	{
		String data = message.to_string();

		if(!netlib::x::Connection::send(data.c_str(), data.length()))
			throw NetworkError("Could not send data.");
	}
}