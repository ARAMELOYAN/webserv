#pragma once
#include <iostream>

class Config
{
		struct sockaddr_in	_sa;
		std::string			_ip;
		unsigned short		_port;
		int					_server_socket;
	public:
		Config();
		~Config();
};

const std::vector<Config> parce(const std::string& file_name);
