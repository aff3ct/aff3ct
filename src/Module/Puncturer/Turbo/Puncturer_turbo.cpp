#include <stdexcept>

#include "Tools/general_utils.h"

#include "Puncturer_turbo.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename Q>
Puncturer_turbo<B,Q>
::Puncturer_turbo(const int &K,
                  const int &N,
                  const int &tail_bits,
                  const std::string &pattern,
                  const bool buff_enc,
                  const int n_frames,
                  const std::string name)
: Puncturer<B,Q>(K, N + tail_bits, K * 3 + tail_bits, n_frames, name),
  pattern_bits(3), buff_enc(buff_enc), tail_bits(tail_bits)
{
	if (tail_bits < 0)
		throw std::invalid_argument("aff3ct::module::Puncturer_turbo: \"tail_bits\" has to be positive.");

	auto str_array = string_split(pattern, ',');

	if (str_array.size() != 3)
		throw std::invalid_argument("aff3ct::module::Puncturer_turbo: \"pattern\" should give 3 different set "
		                            "delimited by a comma.");

	if (str_array[0].size() != str_array[1].size() || str_array[0].size() != str_array[2].size())
		throw std::invalid_argument("aff3ct::module::Puncturer_turbo: \"pattern\" should give 3 different set "
		                            "delimited by a comma and each of those sets has to contains an equal number "
		                            "of bits.");

	auto period = (int)str_array[0].size();

	if (this->K % period)
		throw std::invalid_argument("aff3ct::module::Puncturer_turbo: \"K\" has to be a multiple of \"n_bits\".");

	pattern_bits[0].resize(period);
	pattern_bits[1].resize(period);
	pattern_bits[2].resize(period);

	for (auto i = 0; i < 3; i++)
		for (auto j = 0; j < period; j++)
		{
			char c[2] = {str_array[i][j], '\0'};
			pattern_bits[i][j] = std::stoi(std::string(c)) ? true : false;
		}

	auto bit_sys_count = 0; for (auto j = 0; j < period; j++) bit_sys_count += pattern_bits[0][j] ? 1 : 0;
	auto bit_pa1_count = 0; for (auto j = 0; j < period; j++) bit_pa1_count += pattern_bits[1][j] ? 1 : 0;
	auto bit_pa2_count = 0; for (auto j = 0; j < period; j++) bit_pa2_count += pattern_bits[2][j] ? 1 : 0;

	auto bit_count = bit_sys_count + bit_pa1_count + bit_pa2_count;

	if ((this->N - tail_bits) != (K / period) * bit_count)
		throw std::invalid_argument("aff3ct::module::Puncturer_turbo: \"N - tail_bits\" has to be equal to "
		                            "\"(K / period) * bit_count\".");
}

template <typename B, typename Q>
Puncturer_turbo<B,Q>
::~Puncturer_turbo()
{
}

template <typename B, typename Q>
void Puncturer_turbo<B,Q>
::_puncture(const B *X_N1, B *X_N2) const
{
	const auto period = pattern_bits[0].size();

	auto k = 0;
	if (this->buff_enc)
	{
		for (auto j = 0; j < 3; j++)
		{
			auto p = 0;
			for (auto i = 0; i < this->K; i++)
			{
				if (pattern_bits[j][p])
					X_N2[k++] = X_N1[this->K * j +i];

				p = (p +1) % period;
			}
		}
	}
	else
	{
		auto p = 0;
		for (auto i = 0; i < this->K; i++)
		{
			if (pattern_bits[0][p]) X_N2[k++] = X_N1[i * 3 +0];
			if (pattern_bits[1][p]) X_N2[k++] = X_N1[i * 3 +1];
			if (pattern_bits[2][p]) X_N2[k++] = X_N1[i * 3 +2];

			p = (p +1) % period;
		}
	}

	std::copy(X_N1 + 3 * this->K, X_N1 + 3 * this->K + this->tail_bits, X_N2 + k);
}

template <typename B, typename Q>
void Puncturer_turbo<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2) const
{
	const auto period = pattern_bits[0].size();

	auto k = 0;
	if (this->buff_enc)
	{
		for (auto j = 0; j < 3; j++)
		{
			auto p = 0;
			for (auto i = 0; i < this->K; i++)
			{
				Y_N2[this->K * j +i] = pattern_bits[j][p] ? Y_N1[k++] : (Q)0;

				p = (p +1) % period;
			}
		}
	}
	else
	{
		auto p = 0;
		for (auto i = 0; i < this->K; i++)
		{
			Y_N2[i * 3 +0] = pattern_bits[0][p] ? Y_N1[k++] : (Q)0;
			Y_N2[i * 3 +1] = pattern_bits[1][p] ? Y_N1[k++] : (Q)0;
			Y_N2[i * 3 +2] = pattern_bits[2][p] ? Y_N1[k++] : (Q)0;

			p = (p +1) % period;
		}
	}

	std::copy(Y_N1 + k, Y_N1 + k + this->tail_bits, Y_N2 + 3 * this->K);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Puncturer_turbo<B_8,Q_8>;
template class aff3ct::module::Puncturer_turbo<B_16,Q_16>;
template class aff3ct::module::Puncturer_turbo<B_32,Q_32>;
template class aff3ct::module::Puncturer_turbo<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
