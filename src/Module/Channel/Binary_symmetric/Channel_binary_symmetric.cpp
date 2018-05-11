#include <type_traits>
#include "Channel_binary_symmetric.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_binary_symmetric<R>
::Channel_binary_symmetric(const int N, const int seed, const tools::Event_probability<R>& noise, const int n_frames)
: Channel_binary_symmetric<R>(N, new tools::Event_generator_std<R>(seed), noise, n_frames)
{
}

template<typename R>
Channel_binary_symmetric<R>
::Channel_binary_symmetric(const int N, tools::Event_generator<R> *event_generator,
              const tools::Event_probability<R> &noise, const int n_frames)
: Channel<R>(N, noise, n_frames), event_generator(event_generator), event_draw(this->N)
{
	const std::string name = "Channel_binary_symmetric";
	this->set_name(name);
}

template <typename R>
void Channel_binary_symmetric<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	const auto event_probability = this->n->get_noise();
	event_generator->generate(this->event_draw.data(), (unsigned)this->N, event_probability);

	for (auto i = 0; i < this->N; i++)
		Y_N[i] = event_draw[i] != (X_N[i] == (R)0.0) ? (R)0.0 : (R)1.0;

//	const mipp::Reg<R> r_zero   = (R)false;
//	const mipp::Reg<R> r_0      = (R)0.0;
//	const mipp::Reg<R> r_1      = (R)1.0;
//
//	const auto vec_loop_size = (this->N / mipp::nElReg<R>()) * mipp::nElReg<R>();
//
//	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
//	{
//		const auto r_in    = mipp::Reg<R>(X_N + i);
//		const auto r_event = mipp::Reg<R>(&this->noise[i]);
//
//		const auto m_zero  = r_in == r_0;
//		const auto m_event = r_event == r_zero;
//
//		const auto r_out   = mipp::blend(r_0, r_1, m_event ^ m_zero);
//		r_out.store(Y_N + i);
//	}
//
//	for (auto i = vec_loop_size; i < this->N; i++)
//		Y_N[i] = this->noise[i] != (X_N[i] == (R)0.0) ? (R)0.0 : (R)1.0;
}

template<typename R>
void Channel_binary_symmetric<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_binary_symmetric<R_32>;
template class aff3ct::module::Channel_binary_symmetric<R_64>;
#else
template class aff3ct::module::Channel_binary_symmetric<R>;
#endif
// ==================================================================================== explicit template instantiation