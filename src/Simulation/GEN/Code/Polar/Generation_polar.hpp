#ifndef GENERATION_POLAR_HPP_
#define GENERATION_POLAR_HPP_

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"
#include "Tools/params.h"

#include "Generator/Polar/SC/Generator_polar_SC_sys.hpp"
#include "Module/Decoder/Polar/SC/Patterns/Pattern_SC_interface.hpp"

#include "Simulation/Simulation.hpp"

namespace aff3ct
{
class Generation_polar : public Simulation
{
protected:
	// simulation parameters
	const parameters params;

	// data vector
	mipp::vector<int> frozen_bits; // known bits (alias frozen bits) are set to true

	// code specifications
	float code_rate;
	float sigma;

	// patterns
	std::vector<Pattern_SC_interface*> patterns_SC;
	Pattern_SC_interface* pattern_SC_rate0;
	Pattern_SC_interface* pattern_SC_rate1;

	Frozenbits_generator<int> *fb_generator;

	// generator
	Generator_polar_SC_sys *generator;

	// file into generate the decoder
	std::string  directory;
	std::string  fileName;
	std::fstream dec_file;
	std::fstream short_dec_file;
	std::fstream graph_file;
	std::fstream short_graph_file;

public:
	Generation_polar(const parameters& params);

	virtual ~Generation_polar();

	void launch();
};
}

#endif /* GENERATION_POLAR_HPP_ */
