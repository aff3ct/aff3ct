#include <type_traits>
#include "Channel_BEC.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_BEC<R>
::Channel_BEC(const int N, const int seed, const tools::Event_probability<R>& noise, const int n_frames)
: Channel_BEC<R>(N, new tools::Event_generator_std<R>(seed), noise, n_frames)
{
}

template<typename R>
Channel_BEC<R>::Channel_BEC(const int N, tools::Event_generator<R> *event_generator,
                            const tools::Event_probability<R> &noise, const int n_frames)
: Channel<R>(N, noise, n_frames), event_generator(event_generator), event_draw(this->N)
{
	const std::string name = "Channel_BEC";
	this->set_name(name);
}

template <typename R>
void Channel_BEC<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	const auto event_probability = this->n->get_noise();
	event_generator->generate(this->event_draw.data(), (unsigned)this->N, event_probability);

	for (auto i = 0; i < this->N; i++)
		Y_N[i] = event_draw[i] ? tools::erased_symbol_val<R>() : X_N[i];
//
//	const mipp::Reg<R> r_erased = tools::erased_symbol_val<R>();
//	const mipp::Reg<R> r_zero   = (R)false;
//
//	const auto vec_loop_size = (this->N / mipp::nElReg<R>()) * mipp::nElReg<R>();
//
//	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
//	{
//		const auto r_in    = mipp::Reg<R>(X_N + i);
//		const auto r_event = mipp::Reg<R>(&this->noise[i]);
//		const auto r_out   = mipp::blend(r_in, r_erased, r_event == r_zero);
//		r_out.store(Y_N + i);
//	}
//
//	for (auto i = vec_loop_size; i < this->N; i++)
//		Y_N[i] = this->noise[i] ? tools::erased_symbol_val<R>() : X_N[i];
}

template<typename R>
void Channel_BEC<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Channel_BEC<R_32>;
template class aff3ct::module::Channel_BEC<R_64>;
#else
template class aff3ct::module::Channel_BEC<R>;
#endif
// ==================================================================================== explicit template instantiation
