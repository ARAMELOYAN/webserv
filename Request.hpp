#pragma once
#include <iostream>

class Request{
		std::string _request;
	public:
		Request()
		{
			std::cout << "Request\n";
		}

		void append(const char* str)
		{
			_request.append(str);
		}

		~Request()
		{};

		void parse(std::string str){};
};
