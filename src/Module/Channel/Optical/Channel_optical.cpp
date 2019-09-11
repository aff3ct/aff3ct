#include "Tools/Exception/exception.hpp"

#include "Module/Channel/Optical/Channel_optical.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_optical<R>
::Channel_optical(const int N, std::unique_ptr<tools::User_pdf_noise_generator<R>>&& _ng,
                  const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  noise_generator(std::move(_ng))
{
	const std::string name = "Channel_optical";
	this->set_name(name);

	if (noise_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'noise_generator' can't be NULL.");
}

template <typename R>
void Channel_optical<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	noise_generator->generate(X_N, Y_N, this->N, this->n->get_noise());
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
