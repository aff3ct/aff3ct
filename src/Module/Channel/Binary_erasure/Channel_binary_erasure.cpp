#include <type_traits>
#include <algorithm>
#include <string>

#include "Tools/Algo/Draw_generator/Event_generator/Standard/Event_generator_std.hpp"
#include "Tools/Algo/Draw_generator/Event_generator/Fast/Event_generator_fast.hpp"
#ifdef AFF3CT_CHANNEL_GSL
#include "Tools/Algo/Draw_generator/Event_generator/GSL/Event_generator_GSL.hpp"
#endif
#ifdef AFF3CT_CHANNEL_MKL
#include "Tools/Algo/Draw_generator/Event_generator/MKL/Event_generator_MKL.hpp"
#endif
#include "Tools/Noise/Noise.hpp"
#include "Tools/Noise/noise_utils.h"
#include "Module/Channel/Binary_erasure/Channel_binary_erasure.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template<typename R>
Channel_binary_erasure<R>
::Channel_binary_erasure(const int N,
                         const tools::Event_generator<R> &event_generator)
: Channel<R>(N),
  event_generator(event_generator.clone())
{
	const std::string name = "Channel_binary_erasure";
	this->set_name(name);
}

template <typename R>
tools::Event_generator<R>* create_event_generator(const tools::Event_generator_implem implem, const int seed)
{
	switch (implem)
	{
		case tools::Event_generator_implem::STD:
			return new tools::Event_generator_std<R>(seed);
			break;
		case tools::Event_generator_implem::FAST:
			return new tools::Event_generator_fast<R>(seed);
			break;
#ifdef AFF3CT_CHANNEL_GSL
		case tools::Event_generator_implem::GSL:
			return new tools::Event_generator_GSL<R>(seed);
			break;
#endif
#ifdef AFF3CT_CHANNEL_MKL
		case tools::Event_generator_implem::MKL:
			return new tools::Event_generator_MKL<R>(seed);
			break;
#endif
		default:
			std::stringstream message;
			message << "Unsupported 'implem' ('implem' = " << (int)implem << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	};
}

template <typename R>
Channel_binary_erasure<R>
::Channel_binary_erasure(const int N,
                         const tools::Event_generator_implem implem,
                         const int seed)
: Channel<R>(N),
  event_generator(create_event_generator<R>(implem, seed))
{
	const std::string name = "Channel_binary_erasure";
	this->set_name(name);
}

template <typename R>
Channel_binary_erasure<R>* Channel_binary_erasure<R>
::clone() const
{
	auto m = new Channel_binary_erasure(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_binary_erasure<R>
::deep_copy(const Channel_binary_erasure<R> &m)
{
	Module::deep_copy(m);
	this->event_generator.reset(m.event_generator->clone());
}

template <typename R>
void Channel_binary_erasure<R>
::set_seed(const int seed)
{
	this->event_generator->set_seed(seed);
}

template <typename R>
void Channel_binary_erasure<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	auto event_draw = (E*)(this->noised_data.data() + this->N * frame_id);

	const auto event_probability = (R)*CP;
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

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_binary_erasure<R_32>;
template class aff3ct::module::Channel_binary_erasure<R_64>;
#else
template class aff3ct::module::Channel_binary_erasure<R>;
#endif
// ==================================================================================== explicit template instantiation
