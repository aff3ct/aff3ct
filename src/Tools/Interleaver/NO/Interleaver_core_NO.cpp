#include <numeric>

#include "Interleaver_core_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_NO<T>
::Interleaver_core_NO(const int size, const int n_frames)
: Interleaver_core<T>(size, "NO", false, n_frames)
{
}

template <typename T>
void Interleaver_core_NO<T>
::gen_lut(T *lut, const int frame_id)
{
	std::iota(lut, lut + this->get_size(), 0);
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_NO<uint8_t >;
template class aff3ct::tools::Interleaver_core_NO<uint16_t>;
template class aff3ct::tools::Interleaver_core_NO<uint32_t>;
template class aff3ct::tools::Interleaver_core_NO<uint64_t>;
// ==================================================================================== explicit template instantiation
