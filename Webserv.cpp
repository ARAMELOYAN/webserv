#include "Webserv.hpp"

Webserv::Webserv(std::vector<Config> &serv): _servers(serv)
{
	FD_ZERO(&_all_sockets);
	FD_ZERO(&_read_fds);
	FD_SET(_servers.begin()->getSockId(), &_all_sockets);
	std::cout << "Set up select fd sets\n";
	std::cout << "WEBSERV\n";
	socket_create();
	int fd_max = _servers.begin()->getSockId();
	while (1)
	{
		_read_fds = _all_sockets;
		int status = select(fd_max + 1, &_read_fds, NULL, NULL, NULL);
		std::cout << "Listening on port \n";
		if (status == -1)
			exit(1);
		if (FD_ISSET(_servers.begin()->getSockId(), &_read_fds))
		{
			std::cout << _servers.begin()->getSockId() << "\n";
			accept_connection();
		}
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
}

void Webserv::accept_connection()
{
	int client_fd = accept(_servers.begin()->getSockId(), NULL, NULL);
	if (client_fd == -1)
		throw Socket_error(std::string("Accept error: ") + strerror(errno));
	FD_SET(client_fd, &_all_sockets);
	if (client_fd > fd_max)
		fd_max = client_fd;
	std::cout << "Accepted new connection on client socket " << client_fd << ".\n";
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
		throw Socket_error(std::string("Send error to client ") + "client_fd" + ": " + strerror(errno));
}
