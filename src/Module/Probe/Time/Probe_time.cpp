#include "Module/Probe/Time/Probe_time.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Probe_time<T>
::Probe_time(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: Probe<T>(size, col_name, reporter, n_frames),
  t_start(std::chrono::steady_clock::now())
{
	const std::string name = "Probe_time<" + col_name + ">";
	this->set_name(name);
	this->set_single_wave(true);
}

template <typename T>
void Probe_time<T>
::_probe(const T *in, const size_t frame_id)
{
	auto t_stop = std::chrono::steady_clock::now();
	auto time_duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(t_stop - this->t_start).count();
	auto time_duration_sec = time_duration * 1e-6;

	for (size_t f = 0; f < this->get_n_frames(); f++)
		this->reporter.probe(this->col_name, (void*)&time_duration_sec, frame_id);
}

template <typename T>
std::type_index Probe_time<T>
::get_datatype() const
{
	return typeid(double);
}

template <typename T>
void Probe_time<T>
::reset()
{
	this->t_start = std::chrono::steady_clock::now();
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Probe_time<R_32>;
template class aff3ct::module::Probe_time<R_64>;
template class aff3ct::module::Probe_time<B_8 >;
template class aff3ct::module::Probe_time<B_16>;
template class aff3ct::module::Probe_time<B_32>;
template class aff3ct::module::Probe_time<B_64>;
// ==================================================================================== explicit template instantiation
