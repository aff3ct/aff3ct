#include <algorithm>
#include <string>

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
                  tools::User_pdf_noise_generator<R>& pdf_noise_generator,
                  const tools::Received_optical_power<R> *noise,
                  const int n_frames)
: Channel<R>(N, noise, n_frames),
  pdf_noise_generator(&pdf_noise_generator),
  is_autoalloc_pdf_gen(false)
{
	const std::string name = "Channel_optical";
	this->set_name(name);
}

template <typename R>
Channel_optical<R>
::Channel_optical(const int N,
                  const tools::Distributions<R>& dist,
                  const tools::Received_optical_power<R> *noise,
                  const tools::User_pdf_noise_generator_implem implem,
                  const int seed,
                  const int n_frames)
: Channel<R>(N, noise, n_frames),
  pdf_noise_generator(nullptr),
  is_autoalloc_pdf_gen(true)
{
	const std::string name = "Channel_optical";
	this->set_name(name);

	switch (implem)
	{
		case tools::User_pdf_noise_generator_implem::STD:
			this->pdf_noise_generator = new tools::User_pdf_noise_generator_std<R>(dist, seed);
			break;
		case tools::User_pdf_noise_generator_implem::FAST:
			this->pdf_noise_generator = new tools::User_pdf_noise_generator_fast<R>(dist, seed);
			break;
#ifdef AFF3CT_CHANNEL_GSL
		case tools::User_pdf_noise_generator_implem::GSL:
			this->pdf_noise_generator = new tools::User_pdf_noise_generator_GSL<R>(dist, seed);
			break;
#endif
#ifdef AFF3CT_CHANNEL_MKL
		case tools::User_pdf_noise_generator_implem::MKL:
			this->pdf_noise_generator = new tools::User_pdf_noise_generator_MKL<R>(dist, seed);
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
::~Channel_optical()
{
	if (this->is_autoalloc_pdf_gen)
		delete pdf_noise_generator;
}

template <typename R>
void Channel_optical<R>
::set_seed(const int seed)
{
	this->pdf_noise_generator->set_seed(seed);
}

template <typename R>
void Channel_optical<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	pdf_noise_generator->generate(X_N, Y_N, this->N, this->n->get_noise());
}

template<typename R>
void Channel_optical<R>
::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::ROP);
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
