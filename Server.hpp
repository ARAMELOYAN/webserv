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
#include <Config.hpp>
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
	char				_responce_msg[BUFSIZ];
	struct timeval		_timer;
	Config&				_cfg;
	public:
	Server(const Config &cfg); //accept and store Config class object where stored all configuratins data
	~Server();
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
	request(client_fd);
	responce(client_fd);
	close(client_fd);
	FD_CLR(client_fd, all_sockets);
}

void Server::responce(int sock)
{
	memset(&_responce_msg, '\0', sizeof _responce_msg);
	sprintf(_responce_msg, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	if (send(client_fd, _responce_msg, strlen(_responce_msg), 0) == -1)
		throw Socket_error(std::string("[Server] Send error to client ") + "client_fd" + ": " + strerror(errno));
}

Server::Server(const Config &cgf): _conf(cfg)
{
	socket_create();
	int fd_max = _server_socket;

	while (1)
	{
		_read_fds = _all_sockets;
		int status = select(fd_max + 1, &_read_fds, NULL, NULL, NULL);
		if (status == -1)
			exit(1);
		if (FD_ISSET(_server_socket, &_read_fds))
			accept_connection();
	}
}
