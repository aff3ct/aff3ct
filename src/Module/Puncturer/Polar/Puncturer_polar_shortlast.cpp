#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"
#include "Module/Puncturer/Polar/Puncturer_polar_shortlast.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_polar_shortlast<B,Q>
::Puncturer_polar_shortlast(const int &K,
                            const int &N,
                            const tools::Frozenbits_generator &fb_generator)
: Puncturer<B,Q>(K, N, (int)std::exp2(std::ceil(std::log2(N)))),
  fb_generator(fb_generator)
{
	const std::string name = "Puncturer_polar_shortlast";
	this->set_name(name);

	if (fb_generator.get_K() != K)
	{
		std::stringstream message;
		message << "'fb_generator.get_K()' has to be equal to 'K' ('fb_generator.get_K()' = " << fb_generator.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (fb_generator.get_N() != this->N_cw)
	{
		std::stringstream message;
		message << "'fb_generator.get_N()' has to be equal to 'N_cw' ('fb_generator.get_N()' = "
		        << fb_generator.get_N() << ", 'N_cw' = " << this->N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename B, typename Q>
Puncturer_polar_shortlast<B,Q>* Puncturer_polar_shortlast<B,Q>
::clone() const
{
	auto m = new Puncturer_polar_shortlast(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B, typename Q>
void Puncturer_polar_shortlast<B,Q>
::gen_frozen_bits(std::vector<bool> &frozen_bits)
{
	auto best_channels = fb_generator.get_best_channels();

	int info_bits_placed = 0;

	// initialize all bits to frozen
	std::fill(frozen_bits.begin(), frozen_bits.end(), true);

	auto i = 0;
	while (info_bits_placed < this->K)
	{
		if (best_channels[i] < (uint32_t)this->N) // choose best channels in interval [0 ; N]
		{                                         // interval [0 ; N] are frozen
			frozen_bits[best_channels[i]] = false;
			info_bits_placed++;
		}
		i++;
	}
}

template <typename B, typename Q>
void Puncturer_polar_shortlast<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	std::copy(X_N1, X_N1 + this->N, X_N2);
}

template <typename B, typename Q>
void Puncturer_polar_shortlast<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	std::copy(Y_N1, Y_N1 + this->N, Y_N2);

	// +inf (bit = 0)
	std::fill(Y_N2 + this->N, Y_N2 + this->N_cw, tools::sat_vals<Q>().second);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Puncturer_polar_shortlast<B_8,Q_8>;
template class aff3ct::module::Puncturer_polar_shortlast<B_16,Q_16>;
template class aff3ct::module::Puncturer_polar_shortlast<B_32,Q_32>;
template class aff3ct::module::Puncturer_polar_shortlast<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_polar_shortlast<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
