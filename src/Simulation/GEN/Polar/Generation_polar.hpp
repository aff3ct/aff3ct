#ifndef GENERATION_POLAR_HPP_
#define GENERATION_POLAR_HPP_

#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include "Tools/MIPP/mipp.h"
#include "Tools/params.h"
#include "Tools/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Generator/Generator.hpp"
#include "Module/Decoder/Polar/SC/Patterns/Pattern_SC_interface.hpp"

#include "../../Simulation.hpp"

class Generation_polar : public Simulation
{
protected:
	// simulation parameters
	const t_simulation_param simu_params;
	const t_code_param       code_params;
	const t_encoder_param    enco_params;
	const t_channel_param    chan_params;
	const t_decoder_param    deco_params;

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
	Generator *generator;

	// file into generate the decoder
	std::string  directory;
	std::string  fileName;
	std::fstream dec_file;
	std::fstream short_dec_file;
	std::fstream graph_file;
	std::fstream short_graph_file;

public:
	Generation_polar(const t_simulation_param& simu_params,
	                 const t_code_param&       code_params,
	                 const t_encoder_param&    enco_params,
	                 const t_channel_param&    chan_params,
	                 const t_decoder_param&    deco_params);

	virtual ~Generation_polar();

	void launch();
};

#endif /* GENERATION_POLAR_HPP_ */
