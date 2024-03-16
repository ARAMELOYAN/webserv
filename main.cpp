#include <iostream>
#include <Server.hpp>
#include <vector>
#include <signal>
#include <sys/types.h>
#include <sys/wait.h>
#define CONFIG_FILE ./webserv.conf

int main()
{
	std::vector<Server> servers = parse(CONFIG_FILE);
	std::vector<Server>::iterator it = servers.begin();
	while(it != servers.end())
	{
		int pid = fork();
		if (pid == -1)
			exit(3);
		else if (pid == 0)
		{
			Socket(*it);
		}
		else
			++it;
	}
	while (true)
	{
		pid_t terminated_pid = waitpid(-1, &status, 0);
		if (terminated_pid == -1)
		{
			if (errno == ECHILD)
				// No more child processes left
				break;
			else
			{
				// Error handling
				std::cerr << "Error while waiting for child processes: " << strerror(errno) << std::endl;
				return 1;
			}
		}
		else
			std::cout << "Child process " << terminated_pid << " has terminated" << std::endl;
	}
	return 0;
}
