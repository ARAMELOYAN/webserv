#include "Webserv.hpp"

Webserv::Webserv(std::vector<Config> &serv): _server(serv)
{
	std::cout << "SERVER\n";
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

Webserv::Socket_error::Socket_error(std::string what):_what(what)
{
}

Webserv::Socket_error::~Socket_error() throw()
{
}

const char* Webserv::Socket_error::what() const throw()
{
	return _what.c_str();
}

void Webserv::socket_create()
{

	_server_socket = socket(_server.begin()->getSa().sin_family, SOCK_STREAM, 0);
	if (_server_socket == -1)
		throw  Socket_error(std::string("[Webserv] Socket error: ") + strerror(errno));
	std::cout << "[Webserv] Created server socket fd: " << _server_socket << "\n";
	if (bind(_server_socket, (struct sockaddr *)&_server.begin()->getSa(), sizeof _server.begin()->getSa()) != 0)
		throw Socket_error(std::string("[Webserv] Bind error: ") + strerror(errno));
	std::cout << "[Webserv] Bound socket to localhost port " << _server.begin()->getPort() << "\n";
	std::cout << "[Webserv] Listening on port " << _server.begin()->getPort() << "\n";
	if (listen(_server_socket, 10) != 0)
		throw Socket_error(std::string("[Webserv] Listen error: ") + strerror(errno));
	FD_ZERO(&_all_sockets);
	FD_ZERO(&_read_fds);
	FD_SET(_server_socket, &_all_sockets);
	std::cout << "[Webserv] Set up select fd sets\n";
}

void Webserv::accept_connection()
{

	int client_fd = accept(_server_socket, NULL, NULL);
	if (client_fd == -1)
		throw Socket_error(std::string("[Webserv] Accept error: ") + strerror(errno));
	FD_SET(client_fd, &_all_sockets);
	if (client_fd > fd_max)
		fd_max = client_fd;
	std::cout << "[Webserv] Accepted new connection on client socket " << client_fd << ".\n";
	//request(client_fd);
	responce(client_fd);
	close(client_fd);
	FD_CLR(client_fd, &_all_sockets);
}

void Webserv::responce(int sock)
{
	memset(&_responce_msg, '\0', sizeof _responce_msg);
	sprintf(_responce_msg, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	if (send(sock, _responce_msg, strlen(_responce_msg), 0) == -1)
		throw Socket_error(std::string("[Webserv] Send error to client ") + "client_fd" + ": " + strerror(errno));
}
