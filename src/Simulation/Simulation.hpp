#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include "Tools/Display/bash_tools.h"

class Simulation
{
public:
	Simulation() {}
	virtual ~Simulation() {}

	virtual void launch() = 0;

	static void check_errors(void *ptr, std::string str, const int tid = 0)
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
};

#endif /* SIMULATION_HPP_ */
