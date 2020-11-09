#include <ctime>
#include <chrono>

#include "Module/Probe/Timestamp/Probe_timestamp.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Probe_timestamp<T>
::Probe_timestamp(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: Probe<T>(size, col_name, reporter, n_frames)
{
	const std::string name = "Probe_timestamp<" + col_name + ">";
	this->set_name(name);
	this->set_single_wave(true);
}

template <typename T>
void Probe_timestamp<T>
::_probe(const T *in, const size_t frame_id)
{
	auto unix_timestamp = std::chrono::seconds(std::time(NULL));
	int64_t unix_timestamp_count = (int64_t)unix_timestamp.count();

	for (size_t f = 0; f < this->get_n_frames(); f++)
		this->reporter.probe(this->col_name, (void*)&unix_timestamp_count, frame_id);
}

template <typename T>
std::type_index Probe_timestamp<T>
::get_datatype() const
{
	return typeid(int64_t);
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Probe_timestamp<R_32>;
template class aff3ct::module::Probe_timestamp<R_64>;
template class aff3ct::module::Probe_timestamp<B_8 >;
template class aff3ct::module::Probe_timestamp<B_16>;
template class aff3ct::module::Probe_timestamp<B_32>;
template class aff3ct::module::Probe_timestamp<B_64>;
// ==================================================================================== explicit template instantiation
