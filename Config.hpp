#pragma once
#include <iostream>

class Config
{
		unsigned int		_ip;
		unsigned short		_port;
	public:
		Config()
		{
			_ip = INADDR_LOOPBACK;
			_port = 4242;
			std::cout << "CONFIG\n";
		}
		~Config()
		{
		};
		unsigned int getIp() const {return _ip;}
		unsigned short getPort() const {return _port;}
};
