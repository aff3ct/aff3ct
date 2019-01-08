#include <type_traits>
#include "Channel_binary_erasure.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_binary_erasure<R>
::Channel_binary_erasure(const int N, const int seed, const tools::Event_probability<R>& noise, const int n_frames)
: Channel_binary_erasure<R>(N, std::unique_ptr<tools::Event_generator_std<R>>(new tools::Event_generator_std<R>(seed)),
  noise, n_frames)
{
}

template<typename R>
Channel_binary_erasure<R>
::Channel_binary_erasure(const int N, std::unique_ptr<tools::Event_generator<R>>&& event_generator,
                         const tools::Event_probability<R> &noise, const int n_frames)
: Channel<R>(N, noise, n_frames), event_generator(std::move(event_generator))
{
	const std::string name = "Channel_binary_erasure";
	this->set_name(name);
}

template <typename R>
void Channel_binary_erasure<R>
::_add_noise(const R *X_N, R *Y_N, const int frame_id)
{
	this->check_noise();

	auto event_draw = (E*)(this->noise.data() + this->N * frame_id);

	const auto event_probability = this->n->get_noise();
	event_generator->generate(event_draw, (unsigned)this->N, event_probability);

	const mipp::Reg<R> r_erased = tools::unknown_symbol_val<R>();
	const mipp::Reg<E> r_false  = (E)false;

	const auto vec_loop_size = (this->N / mipp::nElReg<R>()) * mipp::nElReg<R>();

	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const mipp::Reg<R> r_in    = X_N + i;
		const mipp::Reg<E> r_event = event_draw + i;
		const auto r_out   = mipp::blend(r_in, r_erased, r_event == r_false);
		r_out.store(Y_N + i);
	}

	for (auto i = vec_loop_size; i < this->N; i++)
		Y_N[i] = event_draw[i] ? tools::unknown_symbol_val<R>() : X_N[i];
}

template<typename R>
void Channel_binary_erasure<R>::check_noise()
{
	Channel<R>::check_noise();

	this->n->is_of_type_throw(tools::Noise_type::EP);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_binary_erasure<R_32>;
template class aff3ct::module::Channel_binary_erasure<R_64>;
#else
template class aff3ct::module::Channel_binary_erasure<R>;
#endif
// ==================================================================================== explicit template instantiation
