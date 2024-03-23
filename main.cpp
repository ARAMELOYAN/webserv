#include <iostream>
#include "Server.hpp"
#include "Config.hpp"
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#define CONFIG_FILE ./webserv.conf

int main()
{
	try
	{
		const Config conf = Config();
		std::vector<Config> confs;
		confs.insert(confs.begin(), conf);
		Server serv(*confs.begin());
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
