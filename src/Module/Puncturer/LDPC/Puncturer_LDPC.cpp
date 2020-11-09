#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Puncturer/LDPC/Puncturer_LDPC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_LDPC<B,Q>
::Puncturer_LDPC(const int &K,
                 const int &N,
                 const int &N_cw,
                 const std::vector<bool> &pattern)
: Puncturer<B,Q>(K, N, N_cw)
{
	const std::string name = "Puncturer_LDPC";
	this->set_name(name);

	auto N_pattern = (int)pattern.size();
	pattern_bits = pattern;

	if (N_pattern == 0)
	{
		std::stringstream message;
		message << "'pattern' shouldn't be empty.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	Z = (int)(N_cw/N_pattern);
	if (Z < 1)
	{
		std::stringstream message;
		message << "'Z' has to be strictly positive ('Z' = " << Z << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto bit_count = 0; for (auto j = 0; j < N_pattern; j++) bit_count += pattern_bits[j] ? 1 : 0;

	if (this->N != Z * bit_count)
	{
		std::stringstream message;
		message << "'N' has to be equal to 'Z' * 'bit_count' ('N' = " << this->N << ", Z' = " << Z
		        << ", 'bit_count' = " << bit_count << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename B, typename Q>
Puncturer_LDPC<B,Q>* Puncturer_LDPC<B,Q>
::clone() const
{
	auto m = new Puncturer_LDPC(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Puncturer_LDPC<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	auto k = 0;
	for (auto i = 0; i < (int)pattern_bits.size(); i++)
		if (pattern_bits[i])
		{
			std::copy_n(X_N1 + (i * this->Z), this->Z, X_N2 + k);
			k += Z;
		}
}

template <typename B, typename Q>
void Puncturer_LDPC<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	auto k = 0;
	for (auto i = 0; i < (int)pattern_bits.size(); i++)
		if (pattern_bits[i])
		{
			std::copy_n(Y_N1 + k, this->Z, Y_N2 + (i * this->Z));
			k += Z;
		}
		else
			std::fill_n(Y_N2 + (i * this->Z), this->Z, (Q)0);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Puncturer_LDPC<B_8,Q_8>;
template class aff3ct::module::Puncturer_LDPC<B_16,Q_16>;
template class aff3ct::module::Puncturer_LDPC<B_32,Q_32>;
template class aff3ct::module::Puncturer_LDPC<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_LDPC<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
