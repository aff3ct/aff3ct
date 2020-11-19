#include <string>
#include <algorithm>
#include <type_traits>
#include <mipp.h>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Modem/BPSK/Modem_BPSK_fast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_BPSK_fast<B,R,Q>
::Modem_BPSK_fast(const int N, const bool disable_sig2)
: Modem<B,R,Q>(N),
  disable_sig2(disable_sig2),
  two_on_square_sigma((R)1.0)
{
	const std::string name = "Modem_BPSK_fast";
	this->set_name(name);

	if (disable_sig2)
		this->set_demodulator(false);
}

template <typename B, typename R, typename Q>
Modem_BPSK_fast<B,R,Q>* Modem_BPSK_fast<B,R,Q>
::clone() const
{
	auto m = new Modem_BPSK_fast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_BPSK_fast<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const size_t frame_id)
{
	throw tools::runtime_error(__FILE__, __LINE__, __func__, "Unsupported data type.");
}

namespace aff3ct
{
namespace module
{
template <>
void Modem_BPSK_fast<int, float, float>
::_modulate(const int *X_N1, float *X_N2, const size_t frame_id)
{
	auto size = (unsigned int)(this->N);

	const auto vec_loop_size = (size / mipp::nElReg<int>()) * mipp::nElReg<int>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<int>())
	{
		auto x1b = mipp::Reg<int>(&X_N1[i]);
		auto x1r = x1b.cvt<float>();
		auto x2r = one - (x1r + x1r);
		x2r.store(&X_N2[i]);
	}

	for (unsigned i = vec_loop_size; i < size; i++)
		X_N2[i] = (float)((int)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}
}
}

namespace aff3ct
{
namespace module
{
template <>
void Modem_BPSK_fast<short, float, float>
::_modulate(const short *X_N1, float *X_N2, const size_t frame_id)
{
	auto size = (unsigned int)(this->N);

	const auto vec_loop_size = size / mipp::nElReg<short>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i++)
	{
		auto x1b = mipp::Reg<short>(&X_N1[i * mipp::nElReg<short>()]);
		auto x1b_low  = x1b.low ().cvt<int>();
		auto x1b_high = x1b.high().cvt<int>();

		auto x1r_low  = x1b_low .cvt<float>();
		auto x1r_high = x1b_high.cvt<float>();

		auto x2r_low  = one - (x1r_low  + x1r_low);
		auto x2r_high = one - (x1r_high + x1r_high);

		x2r_low .store(&X_N2[i * 2 * mipp::nElReg<float>() + 0 * mipp::nElReg<float>()]);
		x2r_high.store(&X_N2[i * 2 * mipp::nElReg<float>() + 1 * mipp::nElReg<float>()]);
	}

	for (unsigned i = vec_loop_size * mipp::nElReg<short>(); i < size; i++)
		X_N2[i] = (float)((short)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}
}
}

namespace aff3ct
{
namespace module
{
template <>
void Modem_BPSK_fast<signed char, float, float>
::_modulate(const signed char *X_N1, float *X_N2, const size_t frame_id)
{
	auto size = (unsigned int)(this->N);

	const auto vec_loop_size = size / mipp::nElReg<signed char>();
	const mipp::Reg<float> one = 1.f;
	for (unsigned i = 0; i < vec_loop_size; i++)
	{
		auto x1b = mipp::Reg<signed char>(&X_N1[i * mipp::nElReg<signed char>()]);
		auto x1b_low  = x1b.low ().cvt<short>();
		auto x1b_high = x1b.high().cvt<short>();

		auto x1b_low_low   = x1b_low .low ().cvt<int>();
		auto x1b_low_high  = x1b_low .high().cvt<int>();
		auto x1b_high_low  = x1b_high.low ().cvt<int>();
		auto x1b_high_high = x1b_high.high().cvt<int>();

		auto x1r_low_low   = x1b_low_low  .cvt<float>();
		auto x1r_low_high  = x1b_low_high .cvt<float>();
		auto x1r_high_low  = x1b_high_low .cvt<float>();
		auto x1r_high_high = x1b_high_high.cvt<float>();

		auto x2r_low_low   = one - (x1r_low_low   + x1r_low_low  );
		auto x2r_low_high  = one - (x1r_low_high  + x1r_low_high );
		auto x2r_high_low  = one - (x1r_high_low  + x1r_high_low );
		auto x2r_high_high = one - (x1r_high_high + x1r_high_high);

		x2r_low_low  .store(&X_N2[i * 4 * mipp::nElReg<float>() + 0 * mipp::nElReg<float>()]);
		x2r_low_high .store(&X_N2[i * 4 * mipp::nElReg<float>() + 1 * mipp::nElReg<float>()]);
		x2r_high_low .store(&X_N2[i * 4 * mipp::nElReg<float>() + 2 * mipp::nElReg<float>()]);
		x2r_high_high.store(&X_N2[i * 4 * mipp::nElReg<float>() + 3 * mipp::nElReg<float>()]);
	}

	for (unsigned i = vec_loop_size * mipp::nElReg<signed char>(); i < size; i++)
		X_N2[i] = (float)((signed char)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}
}
}

template <typename B,typename R, typename Q>
void Modem_BPSK_fast<B,R,Q>
::_filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_BPSK_fast<B,R,Q>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (disable_sig2)
		std::copy(Y_N1, Y_N1 + this->N, Y_N2);
	else
	{
		if (!std::is_same<R,Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

		if (!std::is_floating_point<Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

		if (*CP != this->last_channel_param)
			this->two_on_square_sigma = (R)2.0 / (*CP * *CP);

		auto size = (unsigned int)(this->N);
		auto vec_loop_size = (size / mipp::nElReg<Q>()) * mipp::nElReg<Q>();
		for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<Q>())
		{
			auto y = mipp::Reg<Q>(&Y_N1[i]) * (Q)this->two_on_square_sigma;
			y.store(&Y_N2[i]);
		}
		for (unsigned i = vec_loop_size; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)this->two_on_square_sigma;
	}

}

template <typename B, typename R, typename Q>
void Modem_BPSK_fast<B,R,Q>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	this->_demodulate(CP, Y_N1, Y_N3, frame_id);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Modem_BPSK_fast<B_8,R_8,R_8>;
template class aff3ct::module::Modem_BPSK_fast<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_BPSK_fast<B_16,R_16,R_16>;
template class aff3ct::module::Modem_BPSK_fast<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_BPSK_fast<B_32,R_32,R_32>;
template class aff3ct::module::Modem_BPSK_fast<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_BPSK_fast<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_BPSK_fast<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
