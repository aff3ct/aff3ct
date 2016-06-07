#ifndef SIMULATION_EXIT_HPP_
#define SIMULATION_EXIT_HPP_

#include <vector>
#include "../../Tools/MIPP/mipp.h"

#include "../Simulation.hpp"

template <typename B, typename Q>
class Simulation_EXIT : public Simulation
{
public:
	Simulation_EXIT() : Simulation() {}
	virtual ~Simulation_EXIT() {}
	
	virtual void launch() = 0;

protected:
	static double measure_mutual_info_avg  (const mipp::vector<Q>& llrs, const mipp::vector<B>& bits             );
	static double measure_mutual_info_histo(const mipp::vector<Q>& llrs, const mipp::vector<B>& bits, const int N);
};

#endif /* SIMULATION_EXIT_HPP_ */
