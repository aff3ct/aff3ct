#include <map>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Interleaver/CCSDS/Interleaver_core_CCSDS.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template <typename T>
Interleaver_core_CCSDS<T>
::Interleaver_core_CCSDS(const int size)
: Interleaver_core<T>(size, "CCSDS", false)
{
	this->init();
}

template <typename T>
Interleaver_core_CCSDS<T>* Interleaver_core_CCSDS<T>
::clone() const
{
	auto t = new Interleaver_core_CCSDS(*this);
	return t;
}

template <typename T>
void Interleaver_core_CCSDS<T>
::gen_lut(T *lut, const size_t frame_id)
{
	std::map<T,T> k_1;
	std::map<T,T> k_2;
	k_1[1784] = 8; k_2[1784] = 223 * 1;
	k_1[3568] = 8; k_2[3568] = 223 * 2;
	k_1[7136] = 8; k_2[7136] = 223 * 4;
	k_1[8920] = 8; k_2[8920] = 223 * 5;

	auto size = this->get_size();
	if (k_1.find(size) != k_1.end())
	{
		for (auto i = 0; i < size; i++)
			lut[i] = (T)pi_CCSDS(i, (int)k_1[size], (int)k_2[size]);
	}
	else
	{
		std::stringstream message;
		message << "There is no CCSDS k_1 and k_2 parameters for 'size' = " << size
		        << " (supported sizes are 1784, 3568, 7136 and 8920).";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename T>
int Interleaver_core_CCSDS<T>
::pi_CCSDS(const int &index, const int &k_1, const int &k_2)
{
	constexpr int p[8] = {31, 37, 43, 47, 53, 59, 61, 67};

	auto m = index % 2;
	auto i = index / (2 * k_2);
	auto j = (index / 2) - i * k_2;
	auto t = (19 * i +1) % (k_1 / 2);
	auto q = t % 8;
	auto c = (p[q] * j + 21 * m) % k_2;

	return 2 * (t + c * (k_1 / 2) +1) - m -1;
}

// ==================================================================================== explicit template instantiation
#include <cstdint>
template class aff3ct::tools::Interleaver_core_CCSDS<uint16_t>;
template class aff3ct::tools::Interleaver_core_CCSDS<uint32_t>;
template class aff3ct::tools::Interleaver_core_CCSDS<uint64_t>;
// ==================================================================================== explicit template instantiation
