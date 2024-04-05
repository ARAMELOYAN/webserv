#pragma once
#include <iostream>

class Request{
		std::string _request;
	public:
		Request()
		{
		}

		void append(const char* str)
		{
			_request.append(str);
			std::cout << _request << "\n\n";
		}

		~Request()
		{};

		void parse(std::string str){};
};
