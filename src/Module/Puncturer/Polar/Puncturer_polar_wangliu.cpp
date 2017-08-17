#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Math/utils.h"

#include "Puncturer_polar_wangliu.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B, typename Q>
Puncturer_polar_wangliu<B,Q>
::Puncturer_polar_wangliu(const int &K,
                          const int &N,
                          const tools::Frozenbits_generator &fb_generator,
                          const int n_frames,
                          const std::string name)
: Puncturer<B,Q>(K, N, (int)std::exp2(std::ceil(std::log2(N))), n_frames, name),
  fb_generator(fb_generator)
{
	if (fb_generator.get_K() != K)
	{
		std::stringstream message;
		message << "'fb_generator.get_K()' has to be equal to 'K' ('fb_generator.get_K()' = " << fb_generator.get_K()
		        << ", 'K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (fb_generator.get_N() != this->N_code)
	{
		std::stringstream message;
		message << "'fb_generator.get_N()' has to be equal to 'N_code' ('fb_generator.get_N()' = "
		        << fb_generator.get_N() << ", 'N_code' = " << this->N_code << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
}

template <typename B, typename Q>
Puncturer_polar_wangliu<B,Q>
::~Puncturer_polar_wangliu()
{
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
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
		{                               // interval [0 ; N] are frozen
			frozen_bits[best_channels[i]] = false;
			info_bits_placed++;
		}
		i++;
	}
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
::_puncture(const B *X_N1, B *X_N2, const int frame_id) const
{
	std::copy(X_N1, X_N1 + this->N, X_N2);
}

template <typename B, typename Q>
void Puncturer_polar_wangliu<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const
{
	std::copy(Y_N1, Y_N1 + this->N, Y_N2);

	// +inf (bit = 0)
	std::fill(Y_N2 + this->N, Y_N2 + this->N_code, tools::sat_vals<Q>().second);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Puncturer_polar_wangliu<B_8,Q_8>;
template class aff3ct::module::Puncturer_polar_wangliu<B_16,Q_16>;
template class aff3ct::module::Puncturer_polar_wangliu<B_32,Q_32>;
template class aff3ct::module::Puncturer_polar_wangliu<B_64,Q_64>;
#else
template class aff3ct::module::Puncturer_polar_wangliu<B,Q>;
#endif
// ==================================================================================== explicit template instantiation
