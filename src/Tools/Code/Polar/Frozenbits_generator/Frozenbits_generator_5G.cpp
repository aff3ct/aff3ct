#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cstdint>
#include <vector>
#include <string>
#include <cmath>

#include "Tools/Arguments/Types/File_system/File_system.hpp"
#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator_5G.hpp"

using namespace aff3ct::tools;

Frozenbits_generator_5G
::Frozenbits_generator_5G(const int K, const int N)
: Frozenbits_generator_file(K, N,
                            tools::modify_path<tools::Is_file>("conf/cde/awgn_polar_codes/5G/N_1024.pc")),
  m((int)std::log2(N))
{
	std::string message = "5G frozen bits generation undefined for N > " + std::to_string(N_max);
	if (N > N_max)
		throw invalid_argument(__FILE__, __LINE__, __func__, message.c_str());
}

Frozenbits_generator_5G
::~Frozenbits_generator_5G()
{
}

void Frozenbits_generator_5G
::evaluate()
{
	std::vector<uint32_t> best_channels_mother(N_max);

	if(!load_channels_file(filename, best_channels_mother))
		throw invalid_argument(__FILE__, __LINE__, __func__, "'" + filename + "' file does not exist.");

	int j = 0;

	for (unsigned i = 0; i != best_channels_mother.size(); i++)
		if(best_channels_mother[i] < (unsigned) this->N)
			this->best_channels[j++] = best_channels_mother[i];

	if (j != this->N)
		throw runtime_error(__FILE__, __LINE__, __func__, "Wrong size.");
}

