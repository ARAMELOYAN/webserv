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
	std::cout << fd_max << "\n";

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
	std::vector<Config>::iterator server = _servers.begin();
	//while (server != _servers.end())
	{
		server->setSockId(socket(AF_INET, SOCK_STREAM, 0));
		if (server->getSockId() == -1)
			throw  Socket_error(std::string("Socket error: ") + strerror(errno));
		std::cout << "Created server socket fd: " << server->getSockId() << "\n";
		if (bind(server->getSockId(), (struct sockaddr *)&server->getSa(), sizeof server->getSa()) != 0)
			throw Socket_error(std::string("Bind error: ") + strerror(errno));
		std::cout << "Bound socket to localhost port " << server->getPort() << "\n";
		if (listen(server->getSockId(), 10) != 0)
			throw Socket_error(std::string("Listen error: ") + strerror(errno));
		std::cout << "Listening on port " << server->getPort()<< "\n";
		server++;
	}
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
