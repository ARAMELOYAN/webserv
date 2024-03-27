#ifndef SERVER_HPP
# define SERVER_HPP
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <map>
#include "Config.hpp"
#include "Request.hpp"
#define BUFSIZ 1024

class Webserv
{
		int 					fd_max;
		fd_set					_all_sockets;
		fd_set					_read_fds;
		char					_request_msg[BUFSIZ];
		char					_responce_msg[BUFSIZ];
		struct timeval			_timer;
		std::vector<Config>&	_server;
		std::map<int, Request>	_client;
	public:
		Webserv(std::vector<Config> &cfg); //accept and store Config class object where stored all configurations data
		void socket_create();
		void accept_connection();
		void responce(int sock);
	
		class Socket_error: public std::exception {
				std::string _what;
			public:
				Socket_error(std::string what);
				virtual const char* what() const throw();
				virtual ~Socket_error() _NOEXCEPT;
		};
};
#endif
