#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4242

int create_server_socket(void);
void accept_new_connection(int listener_socket, fd_set *all_sockets, int *fd_max);
void read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket);

int main(void)
{
	int server_socket;
	int status;
	fd_set all_sockets;
	fd_set read_fds;
	int fd_max;
	struct timeval timer;

	timer.tv_sec = 2;
	FD_ZERO(&all_sockets);
	FD_ZERO(&read_fds);
	printf("---- SERVER ----\n\n");
	server_socket = create_server_socket();
	if (server_socket == -1)
		return (1);
	printf("[Server] Listening on port %d\n", PORT);
	status = listen(server_socket, 10);
	if (status != 0)
	{
		fprintf(stderr, "[Server] Listen error: %s\n", strerror(errno));
		return (3);
	}
	FD_SET(server_socket, &all_sockets);
	fd_max = server_socket;
	printf("[Server] Set up select fd sets\n");
	while (1)
	{
		read_fds = all_sockets;
		status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
		if (status == -1)
		{
			fprintf(stderr, "[Server] Select error: %s\n", strerror(errno));
			exit(1);
		}
		else if (status == 0)
		{
			printf("[Server] Waiting...\n");
			continue;
		}
		for (int i = 0; i <= fd_max; i++)
		{
			if (FD_ISSET(i, &read_fds) == 0)
				continue ;
			printf("[%d] Ready for I/O operation\n", i);
			if (i == server_socket)
				accept_new_connection(server_socket, &all_sockets, &fd_max);
			else 
				read_data_from_socket(i, &all_sockets, fd_max, server_socket);
		}
	}
	return (0);
}

int create_server_socket(void)
{
	struct sockaddr_in sa;
	int socket_fd;
	int status;

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sa.sin_port = htons(PORT);
	socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		fprintf(stderr, "[Server] Socket error: %s\n", strerror(errno));
		return (-1);
	}
	printf("[Server] Created server socket fd: %d\n", socket_fd);
	status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
	if (status != 0)
	{
		fprintf(stderr, "[Server] Bind error: %s\n", strerror(errno));
		return (-1);
	}
	printf("[Server] Bound socket to localhost port %d\n", PORT);
	return (socket_fd);
}

void accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max)
{
	int client_fd;
	char msg_to_send[BUFSIZ];
	int status;

	client_fd = accept(server_socket, NULL, NULL);
	if (client_fd == -1)
	{
		fprintf(stderr, "[Server] Accept error: %s\n", strerror(errno));
		return ;
	}
	FD_SET(client_fd, all_sockets);
	if (client_fd > *fd_max)
		*fd_max = client_fd;
	printf("[Server] Accepted new connection on client socket %d.\n", client_fd);
	memset(&msg_to_send, '\0', sizeof msg_to_send);
	sprintf(msg_to_send, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
	if (status == -1)
		fprintf(stderr, "[Server] Send error to client %d: %s\n", client_fd, strerror(errno));
}

void read_data_from_socket(int socket, fd_set *all_sockets, int fd_max, int server_socket)
{
	char buffer[BUFSIZ];
	char msg_to_send[BUFSIZ];
	int bytes_read;
	int status;

	memset(&buffer, '\0', sizeof buffer);
	bytes_read = recv(socket, buffer, BUFSIZ, 0);
	if (bytes_read <= 0)
	{
		if (bytes_read == 0)
			printf("[%d] Client socket closed connection.\n", socket);
		else
			fprintf(stderr, "[Server] Recv error: %s\n", strerror(errno));
		close(socket);
		FD_CLR(socket, all_sockets);
	}
	else
	{
		memset(&msg_to_send, '\0', sizeof msg_to_send);
		sprintf(msg_to_send, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
		for (int j = 0; j <= fd_max; j++)
		{
			if (FD_ISSET(j, all_sockets) && j != server_socket && j != socket)
			{
				status = send(j, msg_to_send, strlen(msg_to_send), 0);
				if (status == -1)
					fprintf(stderr, "[Server] Send error to client fd %d: %s\n", j, strerror(errno));
			}
		}
	}
}
