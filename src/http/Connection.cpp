#include "Connection.hpp"
#include <type_traits>
#include <cstdio>
#include <ctime>
#include "Message.hpp"
#include <string>

namespace mws
{
	namespace http
	{
		Connection::Connection(
			netlib::StreamSocket &&socket):
			Connection(std::move(socket)) { }

		void Connection::receive_request()
		{
			std::vector<byte_t> request;
			if(!receive(request))
				return false;
			else
				
		}

		void Connection::send_answer()
		{

			String method, path, version;
			size_t scanned = m_request.scanf("%s %s %s", {&method,&path,&version});
			printf("scanned : %zu\n", scanned),
			printf("method: (%p) %s\n", method.c_str(), method.c_str());
			printf("path: (%p) %s\n", path.c_str(), path.c_str());
			printf("version: (%p) %s\n", version.c_str(), version.c_str());

			String content = "<h1>response</h1>your request was:<br /><pre>\"";
			content.append(m_request).append("\"</pre>");
			String timestr;
			std::time_t time;
			std::time(&time);
			timestr = std::ctime(&time);
			timestr = timestr.substring(0,timestr.indexof(timestr.find_last('\n')));
			
			content.append(timestr).append(" time of response.<br />");
			content.append('"').append(method).append("\",\"").append(path).append("\",\"")
				.append(version).append('"');

			MessageHeader header;
			header.add("Date", timestr);
			header.add("Server", "Miasma Web Server (" __TIMESTAMP__ ")");
			header.add("Content-Type", "text/html; charset=UFT-8");
			header.add("Content-Length", std::to_string(content.length()).c_str());
			header.add("Accept-Ranges", "bytes");
			header.add("Connection", "close");
			
			Message message("/1.1 200 OK", std::move(header), content);
			String msg = message.to_string();
			puts(msg.c_str());
			send(msg);
			m_socket.shutdown(netlib::Shutdown::kBoth);
			m_socket.close();
		}

		String const& Connection::request() const
		{
			return m_request;
		}
	}
}