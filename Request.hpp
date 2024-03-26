#pragma once
#include <iostream>

class Request{
		std::string _request;
	public:
		Request(std::string msg): _request(msg)
		{
			std::cout << "Request\n";
		}
		~Request()
		{};
		void parse(std::string str){};
};
