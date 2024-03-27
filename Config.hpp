#pragma once
#include <iostream>

class Config
{
		int					_socket;
		struct sockaddr_in	_sa;
	public:
		Config(unsigned short port, unsigned int ip = INADDR_LOOPBACK)
		{
			memset(&_sa, 0, sizeof _sa);
			_sa.sin_family = AF_INET;
			_sa.sin_addr.s_addr = htonl(ip);
			_sa.sin_port = htons(port);
			_socket = socket(_sa.sin_family, SOCK_STREAM, 0);
			if (_socket == -1)
				std::cout << "Socket error: " << strerror(errno) << "\n";
			std::cout << "Created server socket fd: " << _socket << "\n";
			if (bind(_socket, (struct sockaddr *)&_sa, sizeof _sa) != 0)
				std::cout << "Bind error: " << strerror(errno) << "\n";
			std::cout << "Bound socket to localhost port " << getPort() << "\n";
			std::cout << "Listening on port " << getPort()<< "\n";
			if (listen(_socket, 10) != 0)
				std::cout << "Listen error: " << strerror(errno) << "\n";
		}
		~Config()
		{
		};
		unsigned int getIp() const			{return ntohl(_sa.sin_addr.s_addr);}
		unsigned short getPort() const		{return ntohs(_sa.sin_port);}
		const sockaddr_in& getSa() const	{return _sa;}
		int getSockId() const				{return _socket;}
};
