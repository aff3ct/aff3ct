#include <algorithm>
#include <numeric>

#include "Interleaver_core_random.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_random<T>
::Interleaver_core_random(const int size, const int seed, const bool uniform, const int n_frames)
: Interleaver_core<T>(size, "random", uniform, n_frames), rd_engine()
{
	rd_engine.seed(seed);
}

template <typename T>
void Interleaver_core_random<T>
::gen_lut(T *lut, const int frame_id)
{
	std::iota   (lut, lut + this->get_size(), 0);
	std::shuffle(lut, lut + this->get_size(), rd_engine);
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_random<uint8_t >;
template class aff3ct::tools::Interleaver_core_random<uint16_t>;
template class aff3ct::tools::Interleaver_core_random<uint32_t>;
template class aff3ct::tools::Interleaver_core_random<uint64_t>;
// ==================================================================================== explicit template instantiation
