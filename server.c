#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
# define RED	"\33[1;31m"
# define GREEN	"\33[1;32m"
# define YELLOW	"\33[1;33m"
# define BLUE	"\33[1;34m"
# define PINK	"\33[1;35m"
# define CYAN	"\33[1;36m"
# define WHITE	"\33[1;37m"
# define RESET	"\33[0;m"

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

	FD_ZERO(&all_sockets);
	FD_ZERO(&read_fds);
	printf("---- SERVER ----\n\n");
	server_socket = create_server_socket();
	if (server_socket == -1)
		return (1);
	printf("[Server] Listening on port %d\n", PORT);
	if (listen(server_socket, 10) != 0)
	{
		return (3);
	}
	FD_SET(server_socket, &all_sockets);
	fd_max = server_socket;
	while (1)
	{
		read_fds = all_sockets;
		status = select(fd_max + 1, &read_fds, NULL, NULL, NULL);
		if (status == -1)
			exit(1);
		printf(RED"STATUS____%d\n"RESET, status);
		for (int i = 0; i <= fd_max; i++)
		{
			if (FD_ISSET(i, &read_fds) == 0)
				continue ;
			if (i == server_socket)
				accept_new_connection(server_socket, &all_sockets, &fd_max);
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
		return (-1);
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
	printf("[%d] Ready for I/O operation\n", client_fd);
	if (client_fd == -1)
		return ;
	FD_SET(client_fd, all_sockets);
	if (client_fd > *fd_max)
		*fd_max = client_fd;
	printf("[Server] Accepted new connection on client socket %d.\n", client_fd);
	memset(&msg_to_send, '\0', sizeof msg_to_send);
	sprintf(msg_to_send, "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 1234\nConnection: close\n\n<!DOCTYPE html>\n<html>\n<head>\n<title>HELLO HTML</title>\n</head>\n<body>\n<h1>Hello Html from my server</h1>\n</body>\n</html>");
	send(client_fd, msg_to_send, strlen(msg_to_send), 0);
	close(client_fd);
	FD_CLR(client_fd, all_sockets);
}
