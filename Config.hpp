#pragma once
#include <iostream>

class Config
{
		struct sockaddr_in	_sa;
		unsigned int		_ip;
		unsigned short		_port;
	public:
		Config(unsigned short port): _port(port)
		{
			_ip = INADDR_LOOPBACK;
			memset(&_sa, 0, sizeof _sa);
			_sa.sin_family = AF_INET;
			_sa.sin_addr.s_addr = htonl(_ip);
			_sa.sin_port = htons(_port);
			std::cout << "CONFIG\n";
		}
		~Config()
		{
		};
		unsigned int getIp() const {return _ip;}
		unsigned short getPort() const {return _port;}
		const sockaddr_in& getSa() const {return _sa;}
};
