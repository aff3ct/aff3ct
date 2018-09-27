#include <type_traits>

#include "Tools/Exception/exception.hpp"

#include "Modem_BPSK.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_BPSK<B,R,Q>
::Modem_BPSK(const int N, const tools::Noise<R>& noise, const bool disable_sig2, const int n_frames)
: Modem<B,R,Q>(N, noise, n_frames),
  disable_sig2(disable_sig2),
  two_on_square_sigma((R)0)
{
	const std::string name = "Modem_BPSK";
	this->set_name(name);

	if (disable_sig2)
		this->set_demodulator(false);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::set_noise(const tools::Noise<R>& noise)
{
	Modem<B,R,Q>::set_noise(noise);

	this->n->is_of_type_throw(tools::Noise_type::SIGMA);

	two_on_square_sigma = (R)2.0 / (this->n->get_noise() * this->n->get_noise());
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	auto size = (unsigned int)(this->N);
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = (R)((B)1 - (X_N1[i] + X_N1[i])); // (X_N[i] == 1) ? -1 : +1
}

template <typename B,typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (disable_sig2)
		std::copy(Y_N1, Y_N1 + this->N, Y_N2);
	else
	{
		if (!std::is_same<R,Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

		if (!std::is_floating_point<Q>::value)
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

		if (!this->n->is_set())
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

		auto size = (unsigned int)(this->N_fil);
		for (unsigned i = 0; i < size; i++)
		{
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma;
		}
	}

}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (disable_sig2)
	{
		auto size = (unsigned int)(this->N_fil);
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)H_N[i];
	}
	else
	{
		if (!this->n->is_set())
			throw tools::runtime_error(__FILE__, __LINE__, __func__, "No noise has been set");

		auto size = (unsigned int)(this->N_fil);
		for (unsigned i = 0; i < size; i++)
			Y_N2[i] = Y_N1[i] * (Q)two_on_square_sigma * (Q)H_N[i];
	}

}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	this->_demodulate(Y_N1,Y_N3, frame_id);
}

template <typename B, typename R, typename Q>
void Modem_BPSK<B,R,Q>
::_tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	this->_demodulate_wg(H_N, Y_N1, Y_N3, frame_id);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_BPSK<B_8,R_8,R_8>;
template class aff3ct::module::Modem_BPSK<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_BPSK<B_16,R_16,R_16>;
template class aff3ct::module::Modem_BPSK<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_BPSK<B_32,R_32,R_32>;
template class aff3ct::module::Modem_BPSK<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_BPSK<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_BPSK<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
