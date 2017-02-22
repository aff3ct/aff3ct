#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include "Tools/params.h"
#include "Tools/Factory/Polar/Factory_frozenbits_generator.hpp"
#include "Tools/Code/Polar/nodes_parser.h"

#include "Generator/Polar/SC/Generator_polar_SC_sys.hpp"

#include "Generator/Polar/SC/Generator_polar_SC_sys.hpp"
#include "Generator/Polar/SCL/Generator_polar_SCL_sys.hpp"

#include "Generation_polar.hpp"

Generation_polar
::Generation_polar(const parameters& params)
: Simulation(),
  params          (params                 ),
  frozen_bits     (params.code.N          ),
  code_rate       (0.f                    ),
  sigma           (0.f                    ),
  polar_patterns  (                       ),
  polar_pattern_r0(nullptr                ),
  polar_pattern_r1(nullptr                ),
  fb_generator    (nullptr                ),
  generator       (nullptr                ),
  directory       (params.decoder.gen_path)
{
#ifdef ENABLE_MPI
	std::clog << bold_yellow("(WW) This simulation is not MPI ready, the same computations will be launched ")
	          << bold_yellow("on each MPI processes.") << std::endl;
#endif

	// pattern allocations
	int idx_r0, idx_r1;
	std::string type = params.decoder.type == "SC" ? "SC" : "SCL";
	polar_patterns   = nodes_parser(params.decoder.polar_nodes, idx_r0, idx_r1, type);
	polar_pattern_r0 = polar_patterns[idx_r0];
	polar_pattern_r1 = polar_patterns[idx_r1];

	float snr = params.simulation.snr_min;

	code_rate = (float)params.code.K / (float)params.code.N;
	sigma     = (float)1.0 / std::sqrt((float)2.0 * code_rate * std::pow((float)10.0, (snr / (float)10.0)));

	// build the frozen bits generator
	fb_generator = Factory_frozenbits_generator<int>::build(params);
	check_errors(fb_generator, "Frozenbits_generator<int>");

	// generate the frozen bits
	fb_generator->set_sigma(sigma);
	fb_generator->generate(frozen_bits);

	// work only for SC and systematic encoding...
	if (params.decoder.type == "SCL")
		fileName  = "Decoder_polar_SCL_fast_CA_sys_N" + std::to_string(params.code.N) +
		                                         "_K" + std::to_string(params.code.K) +
		                                       "_SNR" + std::to_string((int)(snr*10));
	else if (params.decoder.type == "SC")
		fileName  = "Decoder_polar_SC_fast_sys_N"   + std::to_string(params.code.N) +
		                                     "_K"   + std::to_string(params.code.K) +
		                                     "_SNR" + std::to_string((int)(snr*10));

	dec_file  .open((directory + "/" + fileName + ".hpp").c_str(), std::ios_base::out);
	graph_file.open((directory + "/" + fileName + ".dot").c_str(), std::ios_base::out);
	if (params.decoder.type == "SCL")
		generator = new Generator_polar_SCL_sys(params.code.K, params.code.N, snr, frozen_bits,
		                                        polar_patterns, *polar_pattern_r0, *polar_pattern_r1,
		                                        dec_file, graph_file);
	else if (params.decoder.type == "SC")
	{
		short_dec_file  .open((directory + "/" + fileName + ".short.hpp").c_str(), std::ios_base::out);
		short_graph_file.open((directory + "/" + fileName + ".short.dot").c_str(), std::ios_base::out);

		generator = new Generator_polar_SC_sys(params.code.K, params.code.N, snr, frozen_bits,
		                                       polar_patterns, *polar_pattern_r0, *polar_pattern_r1,
		                                       dec_file, short_dec_file, graph_file, short_graph_file);
	}
	else
	{
		std::cerr << bold_red("(EE) Unsupported type of decoder, exiting.") << std::endl;
		std::exit(-1);
	}
}

Generation_polar
::~Generation_polar()
{
	if (dec_file        .is_open()) dec_file        .close();
	if (short_dec_file  .is_open()) short_dec_file  .close();
	if (graph_file      .is_open()) graph_file      .close();
	if (short_graph_file.is_open()) short_graph_file.close();

	if (generator    != nullptr) delete generator;
	if (fb_generator != nullptr) delete fb_generator;

	for (unsigned i = 0; i < polar_patterns.size(); i++)
		delete polar_patterns[i]; // memory leak possible with patter_SC_rate0 and 1
}

void Generation_polar
::launch()
{
	this->generator->generate();

	if (!params.terminal.disabled)
	{
		std::string tab = "   ";

		std::cerr << "#"                   << std::endl;
		std::cout << "General statistics:" << std::endl;
		std::cout << "-------------------" << std::endl;

		unsigned long n_nodes     = params.code.N -1;
		unsigned long n_nodes_gen = generator->get_n_generated_nodes();

		std::cout << tab << "Generated decoder file name = " << directory << "/" << fileName << ".hpp" << std::endl;
		std::cout << tab << "Associate graph file name   = " << directory << "/" << fileName << ".dot" << std::endl;
		std::cout << tab << "Nodes number before pruning = " << n_nodes                                << std::endl;
		std::cout << tab << "Nodes number  after pruning = " << n_nodes_gen                            << std::endl;
		std::cout << tab << "Terminal nodes (alias pruning rules):"                                    << std::endl;
		for (unsigned p = 0; p < polar_patterns.size(); p++)
		{
			auto cur_pattern_SC = polar_patterns[p];
			if (cur_pattern_SC->is_terminal())
				std::cout << tab << tab << " - " << std::setw(20) << cur_pattern_SC->name() << ": "
				          << std::setw(5)
				          << generator->get_n_generated_nodes_by_pattern(typeid(*cur_pattern_SC).hash_code(), -1) 
				          << " / " << n_nodes_gen << std::endl;
		}
		std::cout << tab << "Non-terminal nodes (alias specialization rules):" << std::endl;
		for (unsigned p = 0; p < polar_patterns.size(); p++)
		{
			auto cur_pattern_SC = polar_patterns[p];
			if (!cur_pattern_SC->is_terminal())
				std::cout << tab << tab << " - " << std::setw(20) << cur_pattern_SC->name() << ": "
				          << std::setw(5)
				          << generator->get_n_generated_nodes_by_pattern(typeid(*cur_pattern_SC).hash_code(), -1) 
				          << " / " << n_nodes_gen << std::endl;
		}

		std::cout << std::endl;

		std::cout << "Per layer (graph depth) statistics:" << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		for (auto d = 0; d < params.code.m +1; d++)
		{
			n_nodes     = (unsigned long)std::pow(2, d);
			n_nodes_gen = generator->get_n_generated_nodes(d);

			std::cout << tab        << "* Graph depth = " << d                               << std::endl;
			std::cout << tab << tab << "Sub-code length = " << std::pow(2, params.code.m -d) << std::endl;
			std::cout << tab << tab << "Nodes number before pruning = " << n_nodes           << std::endl;
			std::cout << tab << tab << "Nodes number  after pruning = " << n_nodes_gen       << std::endl;
			std::cout << tab << tab << "Terminal nodes (alias pruning rules)"                << std::endl;
			for (unsigned p = 0; p < polar_patterns.size(); p++)
			{
				auto cur_pattern_SC = polar_patterns[p];
				if (cur_pattern_SC->is_terminal())
					std::cout << tab << tab << tab << " - " << std::setw(20) << cur_pattern_SC->name() << ": "
					          << std::setw(5)
					          << generator->get_n_generated_nodes_by_pattern(typeid(*cur_pattern_SC).hash_code(), d) 
					          << " / " << n_nodes_gen << std::endl;
			}
			std::cout << tab << tab << "Non-terminal nodes (alias specialization rules):" << std::endl;
			for (unsigned p = 0; p < polar_patterns.size(); p++)
			{
				auto cur_pattern_SC = polar_patterns[p];
				if (!cur_pattern_SC->is_terminal())
					std::cout << tab << tab << tab << " - " << std::setw(20) << cur_pattern_SC->name() << ": "
					          << std::setw(5)
					          << generator->get_n_generated_nodes_by_pattern(typeid(*cur_pattern_SC).hash_code(), d) 
					          << " / " << n_nodes_gen << std::endl;
			}
			if (d < params.code.m)
				std::cout << std::endl;
			else
				std::cerr << "#" << std::endl;
		}
	}
}
