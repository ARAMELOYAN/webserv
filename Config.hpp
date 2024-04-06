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
			this->setSockId(socket(AF_INET, SOCK_STREAM, 0));
			if (this->getSockId() == -1)
				throw  Socket_error(std::string("Socket error: ") + strerror(errno));
			std::cout << "Socket created fd: " << this->getSockId() << "\n";
			if (bind(this->getSockId(), (struct sockaddr *)&this->getSa(), sizeof this->getSa()) != 0)
				throw Socket_error(std::string("Bind error: ") + strerror(errno));
			std::cout << "Bound socket to localhost port " << this->getPort() << "\n";
			if (listen(this->getSockId(), 10) != 0)
				throw Socket_error(std::string("Listen error: ") + strerror(errno));
			std::cout << "Listening on port " << this->getPort()<< "\n";
		}
		~Config()
		{
			close(_socket);
			std::cout << _socket << " Server closed\n";
		};
		Config(const Config& cnf)
		{
			_socket = cnf._socket;
			_sa = cnf._sa;
			std::cout << "Config copy constructor\n";
		}
		unsigned int getIp() const			{return ntohl(_sa.sin_addr.s_addr);}
		unsigned short getPort() const		{return ntohs(_sa.sin_port);}
		const sockaddr_in& getSa() const	{return _sa;}
		int getSockId() const				{return _socket;}
		void setSockId(int fd)				{_socket = fd;}
		class Socket_error: public std::exception {
				std::string _what;
			public:
				Socket_error(std::string what):_what(what){};
				virtual const char* what() const throw()
				{
					return _what.c_str();
				}
				virtual ~Socket_error() _NOEXCEPT {};
		};
};
