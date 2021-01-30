#include "Tools/Interface/Interface_get_set_frozen_bits.hpp"

#include <sstream>
#include "Tools/Exception/exception.hpp"

using namespace aff3ct::tools;

void Interface_get_set_frozen_bits
::assert_frozen_bits(const std::vector<bool>& frozen_bits, const int K, const int N)
{
	if (frozen_bits.size() != N)
	{
		std::stringstream message;
		message << "The frozen_bits size is invalid ('N' = " << N << ", 'Size' = "
				<< frozen_bits.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto k = 0; 
	for (auto i = 0; i < N; i++) if (frozen_bits[i] == 0) k++;
	
	if (K != k)
	{
		std::stringstream message;
		message << "The number of information bits in the frozen_bits is invalid ('K' = " << K << ", 'k' = "
				<< k << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}
