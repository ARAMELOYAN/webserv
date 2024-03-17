#include "Server.hpp"

int main()
{
	try
	{
		Server webserv("127.0.0.1", 8080);
	}
	catch(std::excepiton &e)
	{
		std::cout << e.what() << endl;
	}
	return 0;
}
