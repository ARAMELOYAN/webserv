#include <iostream>
#include <Server.hpp>
#include <Config.hpp>
#include <vector>
#include <signal>
#include <sys/types.h>
#include <sys/wait.h>
#define CONFIG_FILE ./webserv.conf

int main()
{
	std::vector<Config> configs = Config::parse(CONFIG_FILE);
	std::vector<Config>::iterator conf = configs.begin();
	while(conf != configs.end())
	{
		{
			Server(*conf);
		}
		else
			++conf;
	}
	return 0;
}
