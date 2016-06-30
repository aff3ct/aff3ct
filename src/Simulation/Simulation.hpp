#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

class Simulation
{
public:
	Simulation() {}
	virtual ~Simulation() {}

	virtual void launch() = 0;
};

#endif /* SIMULATION_HPP_ */
