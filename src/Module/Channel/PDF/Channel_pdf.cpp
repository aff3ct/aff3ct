#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Channel_pdf.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_pdf<R>
::Channel_pdf(const int N, tools::User_pdf_noise_generator<R> *draw_generator, const bool add_users,
              const tools::Noise<R>& noise, const int n_frames)
: Channel<R>(N, noise, n_frames),
  add_users(add_users),
  draw_generator(draw_generator)
{
	const std::string name = "Channel_pdf";
	this->set_name(name);

	if (draw_generator == nullptr)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "'draw_generator' can't be NULL.");
}

template <typename R>
Channel_pdf<R>
::~Channel_pdf()
{
	if (draw_generator != nullptr) delete draw_generator;
}

template <typename R>
void Channel_pdf<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	if (add_users && this->n_frames > 1)
	{
		if (frame_id != -1)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to -1 ('frame_id' = " << frame_id << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		draw_generator->generate(X_N, this->noise.data(), this->N, this->n->get_noise());

		std::fill(Y_N, Y_N + this->N, (R)0);
		for (auto f = 0; f < this->n_frames; f++)
			for (auto i = 0; i < this->N; i++)
				Y_N[i] += X_N[f * this->N +i];

		for (auto i = 0; i < this->N; i++)
			Y_N[i] += this->noise[i];
	}
	else
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (frame_id < 0)
			draw_generator->generate(this->noise, this->n->get_noise());
		else
			draw_generator->generate(X_N, this->noise.data() + f_start * this->N, this->N, this->n->get_noise());

		for (auto f = f_start; f < f_stop; f++)
			for (auto n = 0; n < this->N; n++)
				Y_N[f * this->N +n] = X_N[f * this->N +n] + this->noise[f * this->N +n];
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_pdf<R_32>;
template class aff3ct::module::Channel_pdf<R_64>;
#else
template class aff3ct::module::Channel_pdf<R>;
#endif
// ==================================================================================== explicit template instantiation
