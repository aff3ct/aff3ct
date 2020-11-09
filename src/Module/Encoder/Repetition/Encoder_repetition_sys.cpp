#include <string>
#include <algorithm>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Repetition/Encoder_repetition_sys.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Encoder_repetition_sys<B>
::Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding)
: Encoder<B>(K, N), rep_count((N/K) -1), buffered_encoding(buffered_encoding)
{
	const std::string name = "Encoder_repetition_sys";
	this->set_name(name);

	if (N % K)
	{
		std::stringstream message;
		message << "'K' has to be a multiple of 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (!buffered_encoding)
		for (auto k = 0; k < this->K; k++)
			this->info_bits_pos[k] = (rep_count +1) * k;
}

template <typename B>
Encoder_repetition_sys<B>* Encoder_repetition_sys<B>
::clone() const
{
	auto m = new Encoder_repetition_sys(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Encoder_repetition_sys<B>
::_encode(const B *U_K, B *X_N, const size_t frame_id)
{
	// repetition
	if (buffered_encoding)
	{
		// systematic bits
		std::copy(U_K, U_K + this->K, X_N);

		// parity bits
		for (auto i = 1; i <= rep_count; i++)
			std::copy(U_K, U_K + this->K, X_N + i * this->K);
	}
	else
	{
		for (auto i = 0; i < this->K; i++)
		{
			const auto off1 = i * (rep_count +1);
			const auto off2 = off1 +1;

			const auto bit = U_K[i];

			X_N[off1] = bit; // systematic bit

			// parity bits
			for (auto j = 0; j < rep_count; j++)
				X_N[off2 +j] = bit;
		}
	}
}

template <typename B>
bool Encoder_repetition_sys<B>
::is_codeword(const B *X_N)
{
	bool valid = true;

	if (buffered_encoding)
	{
		auto r = 0;
		while (valid && r < rep_count)
		{
			auto k = 0;
			while (k < this->K && (X_N[k] == X_N[(r +1) * this->K +k])) k++;
			valid = k == this->K;
			r++;
		}

		return valid;
	}
	else
	{
		auto k = 0;
		while (valid && k < this->K)
		{
			auto r = 0;
			while (r < rep_count && (X_N[k * (rep_count +1)] == X_N[k * (rep_count +1) + r +1])) r++;
			valid = r == rep_count;
			k++;
		}

		return valid;
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Encoder_repetition_sys<B_8>;
template class aff3ct::module::Encoder_repetition_sys<B_16>;
template class aff3ct::module::Encoder_repetition_sys<B_32>;
template class aff3ct::module::Encoder_repetition_sys<B_64>;
#else
template class aff3ct::module::Encoder_repetition_sys<B>;
#endif
// ==================================================================================== explicit template instantiation
