#pragma once
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
#define BUFSIZ 1024
#define CONFIG_FILE "./conf/default.conf"

class Server
{
	struct sockaddr_in	_sa;
	std::string			_ip;
	unsigned short		_port;
	int 				_server_socket;
	fd_set				_all_sockets;
	fd_set				_read_fds;
	char				_request_msg[BUFSIZ];
	char				_responce_nsg[BUFSIZ];
	struct timeval		_timer;
	public:
	Server(std::string ip, unsigned short port);
	~Server();
	void socket_create();
	void accept_connection();

	class Socket_error:public std::exception {
		std::string _what;
		public:
		Socket_error(std::string what);
		virtual const char* what() const throw();
		virtual ~Socket_error() _NOEXCEPT;
	};
};

Server::Socket_error::Socket_error(std::string what):_what(what)
{
}

const char* Server::Socket_error::what() const throw()
{
	return _what.c_str();
}

void Server::socket_create()
{

	memset(&_sa, 0, sizeof _sa);
	_sa.sin_family = AF_INET;
	_sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	_sa.sin_port = htons(_port);
	_server_socket = socket(_sa.sin_family, SOCK_STREAM, 0);
	if (_server_socket == -1)
		throw  Socket_error(std::string("[Server] Socket error: ") + strerror(errno));
	std::cout << "[Server] Created server socket fd: " << _server_socket << "\n";
	if (bind(_server_socket, (struct sockaddr *)&_sa, sizeof _sa) != 0)
		throw Socket_error(std::string("[Server] Bind error: ") + strerror(errno));
	std::cout << "[Server] Bound socket to localhost port " << _port << "\n";
	std::cout << "[Server] Listening on port " << _port << "\n";
	if (listen(_server_socket, 10) != 0)
		throw Socket_error(std::string("[Server] Listen error: ") + strerror(errno));

	FD_ZERO(&_all_sockets);
	FD_ZERO(&_read_fds);
	FD_SET(_server_socket, &_all_sockets);
	std::cout << "[Server] Set up select fd sets\n";
}

void Server::accept_connection()
{

	int client_fd = accept(_server_socket, NULL, NULL);
	if (client_fd == -1)
		throw Socket_error(std::string("[Server] Accept error: ") + strerror(errno));
	FD_SET(client_fd, &_all_sockets);
	if (client_fd > fd_max)
		fd_max = client_fd;
	std::cout << "[Server] Accepted new connection on client socket " << client_fd << ".\n";
	memset(&_responce_nsg, '\0', sizeof _responce_nsg);
	sprintf(_responce_nsg, "Welcome. You are client fd [%d]\n", client_fd);
	if (send(client_fd, _responce_nsg, strlen(_responce_nsg), 0) == -1)
		throw Socket_error(std::string("[Server] Send error to client ") + "client_fd" + ": " + strerror(errno));

}

void Server::make_response(int i)
{
	std::cout << "[" << i << "] Got message: " << _request_msg << "\n";
	memset(&_responce_nsg, '\0', sizeof _responce_nsg);
	sprintf(_responce_nsg, "[%d] says: %s", i, _request_msg);
	for (int j = 0; j <= fd_max; j++) {
		if (FD_ISSET(j, &_all_sockets) && j != _server_socket && j != i) {
			if (send(j, _responce_nsg, strlen(_responce_nsg), 0) == -1)
				fprintf(stderr, "[Server] Send error to client fd %d: %s\n", j, strerror(errno));
		}
	}
}

Server::Server(std::string ip, unsigned short port): _ip(ip), _port(port)
{
	std::cout << "---- SERVER ----\n\n";

	socket_create();
	int fd_max = _server_socket;

	_timer.tv_sec = 2;
	_timer.tv_usec = 0;
	while (1) {
		_read_fds = _all_sockets;
		int status = select(fd_max + 1, &_read_fds, NULL, NULL, &_timer);
		if (status == -1) {
			fprintf(stderr, "[Server] Select error: %s\n", strerror(errno));
			exit(1);
		}
		else if (status == 0) {
			std::cout << "[Server] Waiting...\n";
			continue ;
		}
		for (int i = 0; i <= fd_max; i++) {
			if (FD_ISSET(i, &_read_fds) == 0)
				continue ;
			std::cout << "[" << i << "] Ready for I/O operation\n";
			if (i == _server_socket)
				accept_connection();
			else {
				memset(&_request_msg, '\0', sizeof _request_msg);
				int bytes_read = recv(i, _request_msg, BUFSIZ, 0);
				if (bytes_read <= 0) {
					close(i);
					FD_CLR(i, &_all_sockets);
					if (bytes_read == 0)
						std::cout << "[" << i << "] Client socket closed connection.\n";
					else
						throw Socket_error(std::string("[Server] Recv error: ") + strerror(errno));
				}
				else
					make_responce(i);
			}
		}
	}
}
