#include "Webserv.hpp"

Webserv::~Webserv()
{
	std::cout << "\e[0;31mWebserv closed\e0m\n";
}

Webserv::Webserv(std::vector<const Config *> &serv): _servers(serv)
{
	_fd_max = 0;
	FD_ZERO(&_all_sockets);
	FD_ZERO(&_read_fds);
	std::cout << "WEBSERV\n";
	std::vector<const Config *>::iterator it = _servers.begin();
	std::map<int, Request>::iterator client_it = _client.begin();
	while (it != _servers.end())
	{
		if (_fd_max < (*it)->getSockId())
			_fd_max = (*it)->getSockId();
		FD_SET(_fd_max, &_all_sockets);
		it++;
	}
	while (1)
	{
		_read_fds = _all_sockets;
		int status = select(_fd_max + 1, &_read_fds, &_write_fds, NULL, NULL);
		if (status == -1)
			exit(1);
		it = _servers.begin();
		while (it != _servers.end())
		{
			if (FD_ISSET((*it)->getSockId(), &_read_fds))
				accept_connection((*it)->getSockId());
			it++;
			std::cout << "\e[0;32mHello\e[0m\n";
		}
		client_it = _client.begin();
		std::cout << "\e[0;31mHello\e[0m\n";
		while (client_it != _client.end())
		{
			if (FD_ISSET(client_it->first, &_read_fds))
				request(client_it->first);
			client_it++;
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

void Webserv::request(int client_fd)
{
	memset(&_requestMsg, 0, sizeof _requestMsg);
	if (recv(client_fd, _requestMsg, 64, 0) <= 0)
	{
		close(client_fd);
		FD_CLR(client_fd, &_all_sockets);
		return ;
	}
	_client[client_fd].append(_requestMsg);
}

void Webserv::accept_connection(int fd)
{
	int client_fd = accept(fd, NULL, NULL);
	if (client_fd == -1)
		throw Socket_error(std::string("Accept error: ") + strerror(errno));
	FD_SET(client_fd, &_all_sockets);
	if (client_fd > _fd_max)
		_fd_max = client_fd;
	_client[client_fd] = Request();
	std::cout << "Accepted new connection on client socket " << client_fd << ".\n";
}

void Webserv::responce(int sock)
{
	memset(&_responceMsg, '\0', sizeof _responceMsg);
	sprintf(_responceMsg, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	if (send(sock, _responceMsg, strlen(_responceMsg), 0) == -1)
		throw Socket_error(std::string("Send error to client ") + "client_fd" + ": " + strerror(errno));
}
