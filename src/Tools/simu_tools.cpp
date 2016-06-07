#include <iostream>

#include "bash_tools.h"

#include "simu_tools.h"

void check_errors(void *ptr, std::string str, const int tid)
{
	if (ptr == nullptr)
	{
		std::cerr << bold_red("(EE) Something went wrong during the \"")
		          << bold_red(str) 
		          << bold_red("\" instantiation (tid = ") 
		          << bold_red(std::to_string(tid))
		          << bold_red("), exiting.") 
		          << std::endl;

		exit(EXIT_FAILURE);
	}
}