#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Puncturer/Turbo/Puncturer_turbo.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_turbo<B,Q>
::Puncturer_turbo(const int &K,
                  const int &N,
                  const int &tail_bits,
                  const std::vector<std::vector<bool>>& pattern_bits,
                  const bool buff_enc)
: Puncturer<B,Q>(K, N, K * 3 + tail_bits),
  pattern_bits(pattern_bits), buff_enc(buff_enc), tail_bits(tail_bits)
{
	const std::string name = "Puncturer_turbo";
	this->set_name(name);

	if (tail_bits < 0)
	{
		std::stringstream message;
		message << "'tail_bits' has to be positive ('tail_bits' = " << tail_bits << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N != compute_N(K, tail_bits, pattern_bits))
	{
		std::stringstream message;
		message << "'N' has to be equal to ('K' / 'period') * 'bit_count' + 'tail_bits' ('N' = " << N
		        << ", 'tail_bits' = " << tail_bits << ", 'K' = " << K << ", 'period' = " << get_period(pattern_bits)
		        << ", 'pattern' = " << display_pattern(pattern_bits) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename B, typename Q>
Puncturer_turbo<B,Q>* Puncturer_turbo<B,Q>
::clone() const
{
	auto m = new Puncturer_turbo(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Puncturer_turbo<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	const auto period = get_period(pattern_bits);

	auto k = 0;
	if (this->buff_enc)
	{
		int off[3] = {0, this->K + this->tail_bits/4, this->N_cw - (this->K + this->tail_bits/4)};
		for (auto j = 0; j < 3; j++)
		{
			auto p = 0;
			auto o = off[j];
			for (auto i = 0; i < this->K; i++)
			{
				if (pattern_bits[j][p])
					X_N2[k++] = X_N1[o +i];
				p = (p +1) % period;
			}

			if (j == 0)
			{
				std::copy(X_N1 + o + this->K, X_N1 + o + this->K + this->tail_bits/4, X_N2 + k);
				k += this->tail_bits/4;
			}
			else if (j == 1)
			{
				std::copy(X_N1 + o + this->K, X_N1 + o + this->K + this->tail_bits/2, X_N2 + k);
				k += this->tail_bits/2;
			}
			else
			{
				std::copy(X_N1 + o + this->K, X_N1 + o + this->K + this->tail_bits/4, X_N2 + k);
				k += this->tail_bits/4;
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

		std::copy(X_N1 + 3 * this->K, X_N1 + 3 * this->K + this->tail_bits, X_N2 + k);
	}
}

template <typename B, typename Q>
void Puncturer_turbo<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	const auto period = get_period(pattern_bits);

	auto k = 0;
	if (this->buff_enc)
	{
		int off[3] = {0, this->K + this->tail_bits/4, this->N_cw - (this->K + this->tail_bits/4)};
		for (auto j = 0; j < 3; j++)
		{
			auto p = 0;
			auto o = off[j];
			for (auto i = 0; i < this->K; i++)
			{
				Y_N2[o +i] = pattern_bits[j][p] ? Y_N1[k++] : (Q)0;
				p = (p +1) % period;
			}

			if (j == 0)
			{
				std::copy(Y_N1 + k, Y_N1 + k + this->tail_bits/4, Y_N2 + o + this->K);
				k += this->tail_bits/4;
			}
			else if (j == 1)
			{
				std::copy(Y_N1 + k, Y_N1 + k + this->tail_bits/2, Y_N2 + o + this->K);
				k += this->tail_bits/2;
			}
			else
			{
				std::copy(Y_N1 + k, Y_N1 + k + this->tail_bits/4, Y_N2 + o + this->K);
				k += this->tail_bits/4;
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

		std::copy(Y_N1 + k, Y_N1 + k + this->tail_bits, Y_N2 + 3 * this->K);
	}
}

template <typename B, typename Q>
std::string Puncturer_turbo<B,Q>
::display_pattern(const std::vector<std::vector<bool>>& pattern_bits)
{
	std::string m;

	for(auto &v : pattern_bits)
	{
		for(const auto &vb : v)
			m += std::to_string(vb);

		m += ",";
	}

	if (m.size())
		m.erase(m.size() -1);

	return m;
}

template <typename B, typename Q>
unsigned Puncturer_turbo<B,Q>
::get_period(const std::vector<std::vector<bool>>& pattern_bits)
{
	if (pattern_bits.size() == 0)
		return 0;

	return (unsigned)pattern_bits.front().size();
}

template <typename B, typename Q>
unsigned Puncturer_turbo<B,Q>
::get_bit_count(const std::vector<std::vector<bool>>& pattern_bits)
{
	unsigned bit_count = 0;
	for (unsigned i = 0; i < pattern_bits.size(); i++)
		for (unsigned j = 0; j < pattern_bits[i].size(); j++)
			bit_count += pattern_bits[i][j] ? 1 : 0;

	return bit_count;
}

template <typename B, typename Q>
void Puncturer_turbo<B,Q>
::check_pattern(const int K, const std::vector<std::vector<bool>>& pattern_bits)
{
	if (pattern_bits.size() != 3) // pattern_bits[0] == bit systematic, pattern_bits[1] == parity 1, pattern_bits[2] == bit parity 2
	{
		std::stringstream message;
		message << "'pattern' should give 3 different set delimited by a comma ('pattern' = "
		        << display_pattern(pattern_bits) << ", 'pattern_bits.size()' = " << pattern_bits.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (pattern_bits[0].size() != pattern_bits[1].size() || pattern_bits[0].size() != pattern_bits[2].size())
	{
		std::stringstream message;
		message << "'pattern' sets have to contains an equal number of bits ('pattern' = " << display_pattern(pattern_bits)
		        << ", 'pattern_bits[0].size()' = " << pattern_bits[0].size()
		        << ", 'pattern_bits[1].size()' = " << pattern_bits[1].size()
		        << ", 'pattern_bits[2].size()' = " << pattern_bits[2].size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto period = get_period   (pattern_bits);
	bool allone = get_bit_count(pattern_bits) == period * pattern_bits.size();

	if (!allone && (K % period != 0))
	{
		std::stringstream message;
		message << "'period' has to be a multiple of 'K' or all bits must be at '1' ('period' = " << period
		        << ", 'K' = " << K << ", 'pattern' = " << display_pattern(pattern_bits) << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
int Puncturer_turbo<B,Q>
::compute_N(const int K, const int tail_bits, const std::vector<std::vector<bool>>& pattern_bits)
{
	check_pattern(K, pattern_bits);

	auto period    = get_period   (pattern_bits);
	auto bit_count = get_bit_count(pattern_bits);

	if (period)
		return K * bit_count / period + tail_bits;
	else
		return 0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Puncturer_turbo<B_8,Q_8>;
template class aff3ct::module::Puncturer_turbo<B_16,Q_16>;
template class aff3ct::module::Puncturer_turbo<B_32,Q_32>;
template class aff3ct::module::Puncturer_turbo<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_turbo<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
