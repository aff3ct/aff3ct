#include <sstream>
#include <string>
#include <mipp.h>

#include "Tools/Exception/exception.hpp"
#include "Module/Channel/User/Channel_user_bs.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename R>
Channel_user_bs<R>
::Channel_user_bs(const int N, const std::string &filename)
: Channel_user<R>(N, filename, false)
{
	const std::string name = "Channel_user_bs";
	this->set_name(name);
}

template <typename R>
Channel_user_bs<R>* Channel_user_bs<R>
::clone() const
{
	auto m = new Channel_user_bs(*this);
	m->deep_copy(*this);
	return m;
}

template <typename R>
void Channel_user_bs<R>
::_add_noise(const float *CP, const R *X_N, R *Y_N, const size_t frame_id)
{
	const mipp::Reg<E> r_false = (E)false;
	const mipp::Reg<R> r_0     = (R)0.0;
	const mipp::Reg<R> r_1     = (R)1.0;

	this->set_noise(frame_id);

	auto event_draw = (E*)(this->noised_data.data() + this->N * frame_id);

	const auto vec_loop_size = (this->N / mipp::nElReg<R>()) * mipp::nElReg<R>();

	for (auto i = 0; i < vec_loop_size; i += mipp::nElReg<R>())
	{
		const mipp::Reg<R> r_in    = X_N + i;
		const mipp::Reg<E> r_event = event_draw + i;

		const auto m_zero  = r_in == r_0;
		const auto m_event = r_event != r_false;

		const auto r_out   = mipp::blend(r_0, r_1, m_event ^ m_zero);
		r_out.store(Y_N + i);
	}

	for (auto i = vec_loop_size; i < this->N; i++)
		Y_N[i] = event_draw[i] != (X_N[i] == (R)0.0) ? (R)0.0 : (R)1.0;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Channel_user_bs<R_32>;
template class aff3ct::module::Channel_user_bs<R_64>;
#else
template class aff3ct::module::Channel_user_bs<R>;
#endif
// ==================================================================================== explicit template instantiation
