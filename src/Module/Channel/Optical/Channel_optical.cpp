#include <algorithm>
#include <string>

#include "Tools/Noise/Noise.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Standard/User_pdf_noise_generator_std.hpp"
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/Fast/User_pdf_noise_generator_fast.hpp"
#ifdef AFF3CT_CHANNEL_GSL
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/GSL/User_pdf_noise_generator_GSL.hpp"
#endif
#ifdef AFF3CT_CHANNEL_MKL
#include "Tools/Algo/Draw_generator/User_pdf_noise_generator/MKL/User_pdf_noise_generator_MKL.hpp"
#endif
#include "Tools/Exception/exception.hpp"
#include "Module/Channel/Optical/Channel_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_optical<R>
::Channel_optical(const int N,
                  const tools::User_pdf_noise_generator<R>& pdf_noise_generator)
: Channel<R>(N),
  pdf_noise_generator(pdf_noise_generator.clone())
{
	const std::string name = "Channel_optical";
	this->set_name(name);
}

template <typename R>
tools::User_pdf_noise_generator<R>* create_user_pdf_noise_generator(const tools::Distributions<R>& dist,
                                                                    const tools::User_pdf_noise_generator_implem implem,
                                                                    const int seed)
{
	switch (implem)
	{
		case tools::User_pdf_noise_generator_implem::STD:
			return new tools::User_pdf_noise_generator_std<R>(dist, seed);
			break;
		case tools::User_pdf_noise_generator_implem::FAST:
			return new tools::User_pdf_noise_generator_fast<R>(dist, seed);
			break;
#ifdef AFF3CT_CHANNEL_GSL
		case tools::User_pdf_noise_generator_implem::GSL:
			return new tools::User_pdf_noise_generator_GSL<R>(dist, seed);
			break;
#endif
#ifdef AFF3CT_CHANNEL_MKL
		case tools::User_pdf_noise_generator_implem::MKL:
			return new tools::User_pdf_noise_generator_MKL<R>(dist, seed);
			break;
#endif
		default:
			std::stringstream message;
			message << "Unsupported 'implem' ('implem' = " << (int)implem << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	};
}

template <typename R>
Channel_optical<R>
::Channel_optical(const int N,
                  const tools::Distributions<R>& dist,
                  const tools::User_pdf_noise_generator_implem implem,
                  const int seed)
: Channel<R>(N),
  pdf_noise_generator(create_user_pdf_noise_generator<R>(dist, implem, seed))
{
	const std::string name = "Channel_optical";
	this->set_name(name);
}

template <typename R>
Channel_optical<R>* Channel_optical<R>
::clone() const
{
	auto m = new Channel_optical(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_optical<R>
::deep_copy(const Channel_optical<R> &m)
{
	Module::deep_copy(m);
	if (m.pdf_noise_generator != nullptr) this->pdf_noise_generator.reset(m.pdf_noise_generator->clone());
}

template <typename R>
void Channel_optical<R>
::set_seed(const int seed)
{
	this->pdf_noise_generator->set_seed(seed);
}

template <typename R>
void Channel_optical<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	pdf_noise_generator->generate(X_N, Y_N, this->N, (R)*CP);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_optical<R_32>;
template class aff3ct::module::Channel_optical<R_64>;
#else
template class aff3ct::module::Channel_optical<R>;
#endif
// ==================================================================================== explicit template instantiation
