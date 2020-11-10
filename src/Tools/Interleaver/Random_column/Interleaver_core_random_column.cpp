#include <algorithm>
#include <sstream>
#include <vector>

#include "Tools/Interleaver/Random_column/Interleaver_core_random_column.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_random_column<T>
::Interleaver_core_random_column(const int size, const int n_cols, const int seed, const bool uniform)
: Interleaver_core<T>(size, "random_column", uniform),
  seed(seed), rd_engine(), n_cols(n_cols), col_size(size / n_cols)
{
	if (col_size * n_cols != size)
	{
		std::stringstream message;
		message << "'size' has to be equal to 'n_cols' * 'col_size' ('size' = " << size
		        << ", 'n_cols' = " << n_cols << ", 'col_size' = " << col_size << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	rd_engine.seed(seed);
	this->init();
}

template <typename T>
Interleaver_core_random_column<T>* Interleaver_core_random_column<T>
::clone() const
{
	auto t = new Interleaver_core_random_column(*this);
	return t;
}

template <typename T>
void Interleaver_core_random_column<T>
::gen_lut(T *lut, const size_t frame_id)
{
	std::vector<T> pi_temp(col_size); // interleaver lookup table for a column

	for (auto column = 0; column < n_cols; column++)
	{
		for (auto c = 0; c < col_size; c++)
			pi_temp[c] = c + column * col_size;

		std::shuffle(pi_temp.begin(), pi_temp.end(), rd_engine);

		for (auto c = 0; c < col_size; c++)
			lut[c + column * col_size] = pi_temp[c];
	}
}

template <typename T>
void Interleaver_core_random_column<T>
::set_seed(const int seed)
{
	this->seed = seed;
	rd_engine.seed(seed);
	this->init();
}

template <typename T>
void Interleaver_core_random_column<T>
::reinitialize()
{
	this->set_seed(this->seed);
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_random_column<uint8_t >;
template class aff3ct::tools::Interleaver_core_random_column<uint16_t>;
template class aff3ct::tools::Interleaver_core_random_column<uint32_t>;
template class aff3ct::tools::Interleaver_core_random_column<uint64_t>;
// ==================================================================================== explicit template instantiation
