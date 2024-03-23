#include "Server.hpp"

Server::Server(const Config &cfg): _cfg(cfg)
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

Server::Socket_error::Socket_error(std::string what):_what(what)
{
}

Server::Socket_error::~Socket_error() throw()
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
	_sa.sin_port = htons(_cfg.get_port());
	_server_socket = socket(_sa.sin_family, SOCK_STREAM, 0);
	if (_server_socket == -1)
		throw  Socket_error(std::string("[Server] Socket error: ") + strerror(errno));
	std::cout << "[Server] Created server socket fd: " << _server_socket << "\n";
	if (bind(_server_socket, (struct sockaddr *)&_sa, sizeof _sa) != 0)
		throw Socket_error(std::string("[Server] Bind error: ") + strerror(errno));
	std::cout << "[Server] Bound socket to localhost port " << _cfg.get_port() << "\n";
	std::cout << "[Server] Listening on port " << _cfg.get_port() << "\n";
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
	//request(client_fd);
	responce(client_fd);
	close(client_fd);
	FD_CLR(client_fd, &_all_sockets);
}

void Server::responce(int sock)
{
	memset(&_responce_msg, '\0', sizeof _responce_msg);
	sprintf(_responce_msg, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	if (send(sock, _responce_msg, strlen(_responce_msg), 0) == -1)
		throw Socket_error(std::string("[Server] Send error to client ") + "client_fd" + ": " + strerror(errno));
}
