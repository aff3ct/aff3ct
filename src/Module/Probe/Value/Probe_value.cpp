#include "Module/Probe/Value/Probe_value.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Probe_value<T>
::Probe_value(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: Probe<T>(size, col_name, reporter, n_frames)
{
	const std::string name = "Probe_value<" + col_name + ">";
	this->set_name(name);
}

template <typename T>
void Probe_value<T>
::_probe(const T *in, const size_t frame_id)
{
	this->reporter.probe(this->col_name, (void*)in, frame_id);
}

template <typename T>
std::type_index Probe_value<T>
::get_datatype() const
{
	return typeid(T);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Probe_value<R_32>;
template class aff3ct::module::Probe_value<R_64>;
template class aff3ct::module::Probe_value<B_8 >;
template class aff3ct::module::Probe_value<B_16>;
template class aff3ct::module::Probe_value<B_32>;
template class aff3ct::module::Probe_value<B_64>;
// ==================================================================================== explicit template instantiation
