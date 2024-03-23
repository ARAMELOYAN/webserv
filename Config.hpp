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
			_port = 8000;
			std::cout << "CONFIG\n";
		}
		~Config()
		{
		};
		const unsigned int get_ip() const {return _ip;}
		const unsigned short get_port() const {return _port;}
};
