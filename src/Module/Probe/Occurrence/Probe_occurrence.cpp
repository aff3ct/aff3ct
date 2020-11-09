#include "Module/Probe/Occurrence/Probe_occurrence.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Probe_occurrence<T>
::Probe_occurrence(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: Probe<T>(size, col_name, reporter, n_frames),
  occurrences(0)
{
	const std::string name = "Probe_occurrence<" + col_name + ">";
	this->set_name(name);
	this->set_single_wave(true);
}

template <typename T>
void Probe_occurrence<T>
::_probe(const T *in, const size_t frame_id)
{
	for (size_t f = 0; f < this->get_n_frames(); f++)
	{
		this->reporter.probe(this->col_name, (void*)&occurrences, frame_id);
		this->occurrences++;
	}
}

template <typename T>
std::type_index Probe_occurrence<T>
::get_datatype() const
{
	return typeid(int64_t);
}

template <typename T>
void Probe_occurrence<T>
::reset()
{
	this->occurrences = 0;
}

template <typename T>
int64_t Probe_occurrence<T>
::get_occurrences() const
{
	return this->occurrences;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Probe_occurrence<R_32>;
template class aff3ct::module::Probe_occurrence<R_64>;
template class aff3ct::module::Probe_occurrence<B_8 >;
template class aff3ct::module::Probe_occurrence<B_16>;
template class aff3ct::module::Probe_occurrence<B_32>;
template class aff3ct::module::Probe_occurrence<B_64>;
// ==================================================================================== explicit template instantiation
