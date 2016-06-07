#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

class Simulation
{
public:
	Simulation() {}
	virtual ~Simulation() {}

	virtual void launch()          = 0;
//protected:
//	virtual void snr_loop()        = 0;
//	virtual void simulation_loop() = 0;
};

#endif /* SIMULATION_HPP_ */
