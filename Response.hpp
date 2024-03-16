#pragma once
#include <iostream>
#include "Request.hpp"

class Responce{
	std::string _html;
	public:
		Responce(Request& req);
		~Responce();
};
