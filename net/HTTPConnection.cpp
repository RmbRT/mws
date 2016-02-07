#include "HTTPConnection.hpp"
#include <type_traits>
#include <cstdio>
#include <ctime>
#include "HTTPMessage.hpp"
#include <string>

namespace mws
{
	namespace net
	{
		HTTPConnection::HTTPConnection(netlib::StreamSocket &&socket):
			Connection(std::move(socket)) { }

		void HTTPConnection::receive_request()
		{
			m_request = receive();
		}

		void HTTPConnection::send_answer()
		{

			String m, p, v;
			size_t scanned = m_request.scanf("%s %s %s", {&m,&p,&v});
			printf("scanned : %i\n", scanned),
			printf("m: (%x) %s\n", m.c_str(), m.c_str());
			printf("p: (%x) %s\n", p.c_str(), p.c_str());
			printf("v: (%x) %s\n", v.c_str(), v.c_str());

			String content = "<h1>response</h1>your request was:<br /><pre>\"";
			content.append(m_request).append("\"</pre>");
			String timestr;
			std::time_t time;
			std::time(&time);
			timestr = std::ctime(&time);
			timestr = timestr.substring(0,timestr.indexof(timestr.find_last('\n')));
			
			content.append(timestr).append(" time of response.<br />");
			content.append('"').append(m).append("\",\"").append(p).append("\",\"")
				.append(v).append('"');

			HTTPMessageHeader header;
			header.add("Date", timestr);
			header.add("Server", "Miasma Web Server (" __TIMESTAMP__ ")");
			header.add("Content-Type", "text/html; charset=UFT-8");
			header.add("Content-Length", std::to_string(content.length()).c_str());
			header.add("Accept-Ranges", "bytes");
			header.add("Connection", "close");
			
			HTTPMessage message("HTTP/1.1 200 OK", std::move(header), content);
			String msg = message.to_string();
			puts(msg.c_str());
			send(msg);
			m_socket.shutdown(netlib::Shutdown::Both);
			m_socket.close();
		}

		String const& HTTPConnection::request() const
		{
			return m_request;
		}
	}
}