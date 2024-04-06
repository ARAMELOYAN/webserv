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
	//	const Config serv1 = Config(8080);
		std::vector<const Config *> servs;
		servs.push_back(&serv);
	//	servs.push_back(&serv1);
		Webserv http(servs);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
