#include <numeric>

#include "Tools/Interleaver/NO/Interleaver_core_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_NO<T>
::Interleaver_core_NO(const int size)
: Interleaver_core<T>(size, "NO", false)
{
	this->init();
}

template <typename T>
Interleaver_core_NO<T>* Interleaver_core_NO<T>
::clone() const
{
	auto t = new Interleaver_core_NO(*this);
	return t;
}

template <typename T>
void Interleaver_core_NO<T>
::gen_lut(T *lut, const size_t frame_id)
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
