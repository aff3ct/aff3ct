#include <type_traits>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <cmath>

#include "Tools/Noise/Received_optical_power.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Module/Modem/OOK/Modem_OOK_optical_rop_estimate.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

//#define ROP_KNOWN_BITS_MEAN0

template <typename B, typename R, typename Q>
Modem_OOK_optical_rop_estimate<B,R,Q>
::Modem_OOK_optical_rop_estimate(const int N, const std::vector<size_t> &ROP_known_bits,
                                 const tools::Distributions<R>& dist)
: module::Modem_OOK<B,R,Q>(N),
  ROP_known_bits(ROP_known_bits),
  dist(dist)
{
	const std::string name = "Modem_OOK_optical_rop_estimate";
	this->set_name(name);

	init();
}

template <typename B, typename R, typename Q>
Modem_OOK_optical_rop_estimate<B,R,Q>
::Modem_OOK_optical_rop_estimate(const int N, int n_known_bits, const tools::Distributions<R>& dist)
: module::Modem_OOK<B,R,Q>(N),
  ROP_known_bits(n_known_bits),
  dist(dist)
{
	const std::string name = "Modem_OOK_optical_rop_estimate";
	this->set_name(name);

	std::iota(ROP_known_bits.begin(), ROP_known_bits.end(), 0);

	init();
}

template <typename B, typename R, typename Q>
Modem_OOK_optical_rop_estimate<B,R,Q>* Modem_OOK_optical_rop_estimate<B,R,Q>
::clone() const
{
	auto m = new Modem_OOK_optical_rop_estimate(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical_rop_estimate<B,R,Q>
::init()
{
	if (ROP_known_bits.size() > (size_t)this->N)
	{
		std::stringstream message;
		message << "'ROP_known_bits.size()' has to be smaller than 'N' ('ROP_known_bits.size()' = "
		        << ROP_known_bits.size() << ", 'N' = " << this->N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	dist_ROP_range = dist.get_noise_range();
	dist_meanvoltage_p0.resize(dist_ROP_range.size(), 0);
	dist_meanvoltage_p1.resize(dist_ROP_range.size(), 0);

	// compute meanvoltage of distributions
	for (unsigned i = 0; i < this->dist_ROP_range.size(); i++)
	{
		const auto& d = dist.get_distribution(this->dist_ROP_range[i]);

		const auto& pdf_x  = d.get_pdf_x();
		const auto& pdf_y0 = d.get_pdf_y()[0];
		const auto& pdf_y1 = d.get_pdf_y()[1];

		for (unsigned j = 0; j < pdf_x.size(); j++)
		{
			this->dist_meanvoltage_p0[i] += pdf_x[j] * pdf_y0[j];
			this->dist_meanvoltage_p1[i] += pdf_x[j] * pdf_y1[j];
		}
	}
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical_rop_estimate<B,R,Q>
::_demodulate_wg(const float *CP, const R *X_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	// ROP estimation
	Q mean1 = 0;
	unsigned n_val1 = 0;
#ifdef ROP_KNOWN_BITS_MEAN0
	Q mean0 = 0;
	unsigned n_val0 = 0;
#endif

	for (unsigned i = 0; i < ROP_known_bits.size(); i++)
	{
		auto pos = ROP_known_bits[i];

		if (X_N[pos] == (R)1.0)
		{
			mean1 += Y_N1[pos];
			n_val1 ++;
		}
#ifdef ROP_KNOWN_BITS_MEAN0
		else
		{
			mean0 += Y_N1[pos];
			n_val0 ++;
		}
#endif
	}

	mean1 /= n_val1;
	auto idx_ROP1 = tools::get_closest_index(dist_meanvoltage_p1.begin(), dist_meanvoltage_p1.end(), mean1);

#ifdef ROP_KNOWN_BITS_MEAN0
	mean0 /= n_val0;
	auto idx_ROP0 = tools::get_closest_index(dist_meanvoltage_p0.begin(), dist_meanvoltage_p0.end(), mean0);
#endif

	const auto estimated_ROP = this->dist_ROP_range[idx_ROP1];

	// LLR computation
	const auto& d = dist.get_distribution(estimated_ROP);
	const auto& pdf_x  = d.get_pdf_x();
	const auto& pdf_y0 = d.get_pdf_y()[0];
	const auto& pdf_y1 = d.get_pdf_y()[1];

	const auto min_value = (Q)1e-10; // when prob_1 ou prob_0 = 0;

	for (auto i = 0; i < this->N_fil; i++)
	{
		auto x_pos = tools::get_closest_index(pdf_x.begin(), pdf_x.end(), Y_N1[i]);

		auto prob_0 = pdf_y0[x_pos] == (Q)0 ? min_value : pdf_y0[x_pos];
		auto prob_1 = pdf_y1[x_pos] == (Q)0 ? min_value : pdf_y1[x_pos];

		Y_N2[i] = (Q)std::log(prob_0/prob_1);
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_8, R_8, Q_8>;
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_8, R_8, R_8>;
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_32,R_32,Q_32>;
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B_64,R_64,Q_64>;
#else
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_OOK_optical_rop_estimate<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
