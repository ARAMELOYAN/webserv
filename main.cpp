#include <iostream>
#include "Server.hpp"
#include "Config.hpp"
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
#define CONFIG_FILE ./webserv.conf

int main()
{
	try
	{
		const Config conf = Config();
		std::vector<Config> confs;
		confs.insert(confs.end(), conf);
		Server serv(confs);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
