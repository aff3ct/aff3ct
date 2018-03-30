#include <type_traits>
#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Modem_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_optical<B,R,Q>
::Modem_optical(const int N,
                tools::User_pdf_noise_generator<R> *noise_generator_p0,
                tools::User_pdf_noise_generator<R> *noise_generator_p1,
                const R ROP,
                const int n_frames)
: Modem<B,R,Q>(N, ROP, n_frames),
  noise_generator_p0(noise_generator_p0),
  noise_generator_p1(noise_generator_p1)
{
	const std::string name = "Modem_optical";
	this->set_name(name);

	if (noise_generator_p0 == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator_p0' can't be NULL.");

	if (noise_generator_p1 == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator_p1' can't be NULL.");
}

template <typename B, typename R, typename Q>
Modem_optical<B,R,Q>
::~Modem_optical()
{
}

template <typename B, typename R, typename Q>
void Modem_optical<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	auto size = (unsigned int)(this->N);
	for (unsigned i = 0; i < size; i++)
		X_N2[i] = X_N1[i] ? (R)1 : (R)0;
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

	auto dist0  = noise_generator_p0->get_distribution(this->sigma);
	auto pdf_x0 = dist0->get_pdf_x();
	auto pdf_y0 = dist0->get_pdf_y();

	auto dist1 = noise_generator_p1->get_distribution(this->sigma);
	// auto pdf_x1 = dist1->get_pdf_x(); // shall be same than pdf_x0
	auto pdf_y1 = dist1->get_pdf_y();

	unsigned x0_pos;
	for (auto i = 0; i < this->N_fil; i++)
	{
		// find the position of the first x that is above the receiver val
		auto x0_above = std::lower_bound(pdf_x0.begin(), pdf_x0.end(), Y_N1[i]);

		if (x0_above == pdf_x0.end())
			x0_pos = pdf_x0.size() - 1;
		else if (x0_above == pdf_x0.begin())
			x0_pos = 0;
		else
		{
			x0_pos = std::distance(pdf_x0.begin(), x0_above);

			auto x0_below = x0_above - 1;

			// find which between x_below and x_above is the nearest of Y_N1[i]
			x0_pos = (Y_N1[i] - *x0_below) < (Y_N1[i] - *x0_above) ? x0_pos - 1 : x0_pos;
		}

		// then get the matching probabilities
		auto prob_0 = pdf_y0[x0_pos] == (Q)0 ? min_value : pdf_y0[x0_pos];
		auto prob_1 = pdf_y1[x0_pos] == (Q)0 ? min_value : pdf_y1[x0_pos]; // pdf_x1 shall be same than pdf_x0

		Y_N2[i] = std::log(prob_0/prob_1);
	}
}

template <typename B, typename R, typename Q>
void Modem_optical<B,R,Q>
::set_sigma(const R ROP)
{
	this->sigma = ROP;
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
