#include <type_traits>
#include <algorithm>
#include <sstream>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"

#include "Modem_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_optical<B,R,Q>
::Modem_optical(const int N,
                const tools::Distributions<R>& dist,
                const R ROP,
                const int n_frames)
: Modem<B,R,Q>(N, (R)-1, n_frames),
  dist(dist)
{
	const std::string name = "Modem_optical";
	this->set_name(name);

	try
	{
		this->set_noise(0,0,ROP);
	}
	catch(tools::invalid_argument&)
	{

	}
}

template <typename B, typename R, typename Q>
Modem_optical<B,R,Q>
::~Modem_optical()
{
}

template <typename B, typename R, typename Q>
void Modem_optical<B,R,Q>
::set_noise(const R sigma, const R esn0, const R ebn0)
{
	this->sigma = sigma;
	this->esn0  = esn0;
	this->ebn0  = ebn0;

	this->rop = this->ebn0;
	this->current_dist = dist.get_distribution(this->rop);

	if (this->current_dist == nullptr)
	{
		std::stringstream message;
		message << "Undefined noise power 'this->rop' in the given distributions ('this->rop' = " << this->rop << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename R, typename Q>
void Modem_optical<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	for (int i = 0; i < this->N; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}


namespace aff3ct
{
namespace module
{
template <>
void Modem_optical<int,float,float>
::_modulate(const int *X_N1, float *X_N2, const int frame_id)
{
	using B = int;
	using R = float;

	unsigned size = (unsigned int)(this->N);

	const auto vec_loop_size = (size / mipp::nElReg<B>()) * mipp::nElReg<B>();
	const mipp::Reg<R> Rone  = (R)1.0;
	const mipp::Reg<R> Rzero = (R)0.0;
	const mipp::Reg<B> Bzero = (B)0;

	for (unsigned i = 0; i < vec_loop_size; i += mipp::nElReg<B>())
	{
		const auto x1b = mipp::Reg<B>(&X_N1[i]);
		const auto x2r = mipp::blend(Rone, Rzero, x1b != Bzero);
		x2r.store(&X_N2[i]);
	}

	for (unsigned i = vec_loop_size; i < size; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
}
}
}



template <typename B,typename R, typename Q>
void Modem_optical<B,R,Q>
::_filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	std::copy(Y_N1, Y_N1 + this->N_fil, Y_N2);
}

template <typename B, typename R, typename Q>
void Modem_optical<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	const Q min_value = 1e-10; // when prob_1 ou prob_0 = 0;

	if (current_dist == nullptr)
		throw tools::runtime_error(__FILE__, __LINE__, __func__, "No valid noise has been set.");


	const auto& pdf_x  = current_dist->get_pdf_x();
	const auto& pdf_y0 = current_dist->get_pdf_y()[0];
	const auto& pdf_y1 = current_dist->get_pdf_y()[1];

	unsigned x_pos;
	for (auto i = 0; i < this->N_fil; i++)
	{
		// find the position of the first x that is above the receiver val
		auto x_above = std::lower_bound(pdf_x.begin(), pdf_x.end(), Y_N1[i]);

		if (x_above == pdf_x.end()) // if last
			x_pos = pdf_x.size() - 1;
		else if (x_above == pdf_x.begin()) // if first
			x_pos = 0;
		else
		{
			x_pos = std::distance(pdf_x.begin(), x_above);

			auto x_below = x_above - 1;

			// find which between x_below and x_above is the nearest of Y_N1[i]
			x_pos = (Y_N1[i] - *x_below) < (Y_N1[i] - *x_above) ? x_pos - 1 : x_pos;
		}

		// then get the matching probabilities
		auto prob_0 = pdf_y0[x_pos] == (Q)0 ? min_value : pdf_y0[x_pos];
		auto prob_1 = pdf_y1[x_pos] == (Q)0 ? min_value : pdf_y1[x_pos];

		Y_N2[i] = std::log(prob_0/prob_1);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Modem_optical<B_8,R_8,R_8>;
template class aff3ct::module::Modem_optical<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_optical<B_16,R_16,R_16>;
template class aff3ct::module::Modem_optical<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_optical<B_32,R_32,R_32>;
template class aff3ct::module::Modem_optical<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_optical<B,R,Q>;
#if !defined(PREC_32_BIT) && !defined(PREC_64_BIT)
template class aff3ct::module::Modem_optical<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
