#include <string>

#include "Module/Sink/NO/Sink_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Sink_NO<B>
::Sink_NO(const int K)
: Sink<B>(K)
{
	const std::string name = "Sink_NO";
	this->set_name(name);
}

template <typename B>
Sink_NO<B>* Sink_NO<B>
::clone() const
{
	auto m = new Sink_NO(*this);
	m->deep_copy(*this);
	return m;
}

template <typename B>
void Sink_NO<B>
::_send(const B *V, const size_t frame_id)
{
	// do nothing, best module ever :-D
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::module::Sink_NO<B_8>;
template class aff3ct::module::Sink_NO<B_16>;
template class aff3ct::module::Sink_NO<B_32>;
template class aff3ct::module::Sink_NO<B_64>;
#else
template class aff3ct::module::Sink_NO<B>;
#endif
// ==================================================================================== explicit template instantiation
