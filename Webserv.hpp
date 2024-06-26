#ifndef SERVER_HPP
# define SERVER_HPP
# include <errno.h>
# include <unistd.h>
# include <netdb.h>
# include <stdio.h>
# include <cstdlib>
# include <sys/select.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/socket.h>
# include <iostream>
# include <string>
# include <exception>
# include <vector>
# include <map>
# include <cstring>
# include "Config.hpp"
# include "Request.hpp"
# define BUFSIZE 1024

class Webserv
{
		int 							_fd_max;
		fd_set							_all_sockets;
		fd_set							_read_fds;
		fd_set							_write_fds;
		char							_requestMsg[BUFSIZ];
		char							_responceMsg[BUFSIZ];
		struct timeval					_timer;
		std::vector<const Config *>&	_servers;
		std::map<int, Request>			_client;
	public:
		Webserv(std::vector<const Config *> &cfg); //accept and store Config class object where stored all configurations data
		~Webserv();
		void socket_create();
		void accept_connection(int fd);
		void request(int sock);
		void responce(int sock);
	
		class Socket_error: public std::exception {
				std::string _what;
			public:
				Socket_error(std::string what);
				virtual const char* what() const throw();
				//virtual ~Socket_error() _NOEXCEPT;
				virtual ~Socket_error(); // linux
		};
};
#endif
