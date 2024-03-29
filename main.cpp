#include <iostream>
#include "Webserv.hpp"
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
		const Config serv = Config(4242);
		const Config serv1 = Config(8080);
		std::vector<Config> servs;
		servs.insert(servs.end(), serv);
		servs.insert(servs.end(), serv1);
		Webserv http(servs);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
