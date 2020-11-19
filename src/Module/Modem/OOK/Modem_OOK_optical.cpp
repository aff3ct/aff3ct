#include <type_traits>
#include <sstream>
#include <cmath>
#include <string>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/general_utils.h"
#include "Module/Modem/OOK/Modem_OOK_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename R, typename Q>
Modem_OOK_optical<B,R,Q>
::Modem_OOK_optical(const int N, const tools::Distributions<R>& dist)
: Modem_OOK<B,R,Q>(N), dist(dist), current_dist(nullptr)
{
	const std::string name = "Modem_OOK_optical";
	this->set_name(name);
}

template <typename B, typename R, typename Q>
Modem_OOK_optical<B,R,Q>* Modem_OOK_optical<B,R,Q>
::clone() const
{
	auto m = new Modem_OOK_optical(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename R, typename Q>
void Modem_OOK_optical<B,R,Q>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	if (!std::is_same<R,Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'R' and 'Q' have to be the same.");

	if (!std::is_floating_point<Q>::value)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "Type 'Q' has to be float or double.");

	if (*CP != this->last_channel_param)
	{
		this->current_dist = &dist.get_distribution(*CP);
		if (this->current_dist == nullptr)
		{
			std::stringstream message;
			message << "Undefined noise power 'CP' in the given distributions ('CP' = " << *CP << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	const auto& pdf_x  = this->current_dist->get_pdf_x();
	const auto& pdf_y0 = this->current_dist->get_pdf_y()[0];
	const auto& pdf_y1 = this->current_dist->get_pdf_y()[1];

	const Q min_value = (Q)1e-10; // when prob_1 ou prob_0 = 0;

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
template class aff3ct::module::Modem_OOK_optical<B_8,R_8,R_8>;
template class aff3ct::module::Modem_OOK_optical<B_8,R_8,Q_8>;
template class aff3ct::module::Modem_OOK_optical<B_16,R_16,R_16>;
template class aff3ct::module::Modem_OOK_optical<B_16,R_16,Q_16>;
template class aff3ct::module::Modem_OOK_optical<B_32,R_32,R_32>;
template class aff3ct::module::Modem_OOK_optical<B_64,R_64,R_64>;
#else
template class aff3ct::module::Modem_OOK_optical<B,R,Q>;
#if !defined(AFF3CT_32BIT_PREC) && !defined(AFF3CT_64BIT_PREC)
template class aff3ct::module::Modem_OOK_optical<B,R,R>;
#endif
#endif
// ==================================================================================== explicit template instantiation
