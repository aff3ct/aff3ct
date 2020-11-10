#include <algorithm>
#include <numeric>
#include <vector>
#include <cmath>

#include "Tools/Interleaver/Golden/Interleaver_core_golden.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_golden<T>
::Interleaver_core_golden(const int size, const int seed, const bool uniform)
: Interleaver_core<T>(size, "golden", uniform),
  seed(seed), gen(), dist(0.0, size * 0.1)
{
	gen.seed(seed);
	this->init();
}

template <typename T>
Interleaver_core_golden<T>* Interleaver_core_golden<T>
::clone() const
{
	auto t = new Interleaver_core_golden(*this);
	return t;
}

template <typename T>
void Interleaver_core_golden<T>
::gen_lut(T *lut, const size_t frame_id)
{
	double g  = (std::sqrt(5)-1)/2.0;
	double g2 = g * g;
	double c  = this->get_size() * g2;

	std::vector<double> tab(this->get_size());
	for (auto i = 0; i < this->get_size(); i++)
		tab[i] = std::fmod(i * c + dist(gen), this->get_size());

	std::iota(lut, lut + this->get_size(), 0);

	std::sort(lut, lut + this->get_size(), [&tab](size_t i1, size_t i2)
	{
		return tab[i1] < tab[i2];
	});
}

template <typename T>
void Interleaver_core_golden<T>
::set_seed(const int seed)
{
	this->seed = seed;
	gen.seed(seed);
	this->init();
}

template <typename T>
void Interleaver_core_golden<T>
::reinitialize()
{
	this->set_seed(this->seed);
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_golden<uint8_t >;
template class aff3ct::tools::Interleaver_core_golden<uint16_t>;
template class aff3ct::tools::Interleaver_core_golden<uint32_t>;
template class aff3ct::tools::Interleaver_core_golden<uint64_t>;
// ==================================================================================== explicit template instantiation
