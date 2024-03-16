#pragma once
#include <iostream>

class Request{
	public:
		Request(std::string msg);
		~Request();
		void parse(std::string str);
		void GET(std::string str);
		void POST(std::string str);
		void DELETE(std::string str);
};
