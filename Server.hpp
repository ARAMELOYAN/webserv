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
#include "Config.hpp"
#define BUFSIZ 1024

class Server
{
		struct sockaddr_in	_sa;
		int 				_server_socket;
		int 				fd_max;
		fd_set				_all_sockets;
		fd_set				_read_fds;
		char				_request_msg[BUFSIZ];
		char				_responce_msg[BUFSIZ];
		struct timeval		_timer;
		const Config&		_cfg;
	public:
		Server(const Config &cfg); //accept and store Config class object where stored all configuratins data
		void socket_create();
		void accept_connection();
		void responce(int sock);
	
		class Socket_error:public std::exception {
			std::string _what;
			public:
			Socket_error(std::string what);
			virtual const char* what() const throw();
			virtual ~Socket_error() _NOEXCEPT;
		};
};
#endif
