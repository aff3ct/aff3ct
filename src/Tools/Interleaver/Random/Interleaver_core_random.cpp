#include <algorithm>
#include <numeric>

#include "Tools/Interleaver/Random/Interleaver_core_random.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_random<T>
::Interleaver_core_random(const int size, const int seed, const bool uniform)
: Interleaver_core<T>(size, "random", uniform),
  seed(seed), rd_engine()
{
	rd_engine.seed(seed);
	this->init();
}

template <typename T>
Interleaver_core_random<T>* Interleaver_core_random<T>
::clone() const
{
	auto t = new Interleaver_core_random(*this);
	return t;
}

template <typename T>
void Interleaver_core_random<T>
::gen_lut(T *lut, const size_t frame_id)
{
	std::iota   (lut, lut + this->get_size(), 0);
	std::shuffle(lut, lut + this->get_size(), rd_engine);
}

template <typename T>
void Interleaver_core_random<T>
::set_seed(const int seed)
{
	this->seed = seed;
	rd_engine.seed(seed);
	this->init();
}

template <typename T>
void Interleaver_core_random<T>
::reinitialize()
{
	this->set_seed(this->seed);
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_random<uint8_t >;
template class aff3ct::tools::Interleaver_core_random<uint16_t>;
template class aff3ct::tools::Interleaver_core_random<uint32_t>;
template class aff3ct::tools::Interleaver_core_random<uint64_t>;
// ==================================================================================== explicit template instantiation
