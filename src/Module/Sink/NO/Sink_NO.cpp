#include <string>

#include "Module/Sink/NO/Sink_NO.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename B>
Sink_NO<B>
::Sink_NO(const int K, const int n_frames)
: Sink<B>(K, n_frames)
{
	const std::string name = "Sink_NO";
	this->set_name(name);
}

template <typename B>
void Sink_NO<B>
::_send(const B *V, const int frame_id)
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
