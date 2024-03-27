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
		}
		~Config()
		{
		};
		unsigned int getIp() const			{return ntohl(_sa.sin_addr.s_addr);}
		unsigned short getPort() const		{return ntohs(_sa.sin_port);}
		const sockaddr_in& getSa() const	{return _sa;}
		int getSockId() const				{return _socket;}
		void setSockId(int fd)				{_socket = fd;}
};
