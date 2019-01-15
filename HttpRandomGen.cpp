#include "StdAfx.h"
#include "HttpRandomGen.h"

#include <boost/asio/ip/tcp.hpp>

HttpRandomGen::HttpRandomGen(void)
{
	httpServer = "www.random.org";
	isFull = false;
	used = 0;

	std::stringstream ss;
	ss << "/integers/?num=" << bufferSize << "&min=1&max=6&col=1&base=10&format=plain&rnd=new";
	httpPath = ss.str();
}

HttpRandomGen::~HttpRandomGen(void)
{
}

int HttpRandomGen::getRandom(int min, int max)
{
	if (!isFull || used >= bufferSize)
	{
		makeHttpCall();
		parseResponse();
		isFull = true;
		used = 0;
	}
	return sequence[used++];
}

void HttpRandomGen::makeHttpCall()
{
	try
	{
		boost::asio::ip::tcp::iostream s;
		s.expires_from_now(boost::posix_time::seconds(10));

		std::string returnStream;

		s.connect(httpServer, "http");
		if (!s)
		{
			std::cout << "HTTP Call Failure: Unable to connect: " << s.error().message() << "\n";
			return;
		}

		s << "GET " << httpPath << " HTTP/1.0\r\n";
		s << "Host: " << httpServer << "\r\n";
		s << "Accept: */*\r\n";
		s << "Connection: close\r\n\r\n";

		std::string http_version;
		s >> http_version;
		unsigned int status_code;
		s >> status_code;
		std::string status_message;
		std::getline(s, status_message);
		if (!s || http_version.substr(0, 5) != "HTTP/")
		{
			std::cout << "HTTP Call Failure: Invalid response\n";
			return;
		}
		if (status_code != 200)
		{
			std::cout << "HTTP Call Failure: Response returned with status code " << status_code << "\n";
			return;
		}

		std::string header;
		while (std::getline(s, header) && header != "\r")
		{
			// std::cout << header << "\n";
		}

		std::stringstream ss;
		ss << s.rdbuf();
		httpResponse = ss.str();
	}
	catch (std::exception& e)
	{
		std::cout << "HTTP Call Failure: Exception: " << e.what() << "\n";
	}
}

void HttpRandomGen::parseResponse()
{
	std::stringstream ss(httpResponse);
	for (int i=0;i<bufferSize;i++)
	{
		ss >> sequence[i];
	}
}