#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Tools/Display/bash_tools.h"

#include "Frozenbits_generator_file.hpp"
using namespace aff3ct;

template <typename B>
Frozenbits_generator_file<B>
::Frozenbits_generator_file(const int K, const int N, const std::string filename)
: Frozenbits_generator<B>(K, N, 0.f), filename(filename)
{
}

template <typename B>
Frozenbits_generator_file<B>
::Frozenbits_generator_file(const int K, const int N, const float sigma)
: Frozenbits_generator<B>(K, N, sigma), filename("")
{
}

template <typename B>
Frozenbits_generator_file<B>
::~Frozenbits_generator_file()
{
}

template <typename B>
void Frozenbits_generator_file<B>
::evaluate()
{
	if(!load_channels_file(filename))
	{
		//Loaded best channels from given file
		std::cerr << bold_red("(EE) Following file does not exist: ") << bold_red(filename) << std::endl;
		exit(EXIT_FAILURE);
	}
}

template <typename B>
bool Frozenbits_generator_file<B>
::load_channels_file(const std::string& filename)
{
	std::ifstream in_code(filename.c_str());

	if (in_code.is_open())
	{
		std::string trash;
		in_code >> trash; // N
		assert(std::stoi(trash) == this->N);
		in_code >> trash; // type
		in_code >> trash; // sigma

		for (unsigned i = 0; i < this->best_channels.size(); i++)
			in_code >> this->best_channels[i];

		in_code.close();
		return true;
	}
	else
		return false;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Frozenbits_generator_file<B_8>;
template class Frozenbits_generator_file<B_16>;
template class Frozenbits_generator_file<B_32>;
template class Frozenbits_generator_file<B_64>;
#else
template class Frozenbits_generator_file<B>;
#ifndef PREC_32_BIT
template class Frozenbits_generator_file<B_32>;
#endif
#endif
// ==================================================================================== explicit template instantiation
