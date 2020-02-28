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
}

template <typename T>
void Probe_timestamp<T>
::probe(const T *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	auto unix_timestamp = std::chrono::seconds(std::time(NULL));
	int64_t unix_timestamp_count = (int64_t)unix_timestamp.count();

	for (auto f = f_start; f < f_stop; f++)
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
