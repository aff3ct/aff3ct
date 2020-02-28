#include "Module/Probe/Throughput/Probe_throughput.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Probe_throughput<T>
::Probe_throughput(const int size, const std::string &col_name, tools::Reporter_probe& reporter, const int n_frames)
: Probe<T>(size, col_name, reporter, n_frames),
  t_start(std::chrono::steady_clock::now()),
  mbps(0.)
{
	const std::string name = "Probe_throughput<" + col_name + ">";
	this->set_name(name);
}

template <typename T>
void Probe_throughput<T>
::probe(const T *in, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	auto t_stop = std::chrono::steady_clock::now();
	auto time_duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(t_stop - this->t_start).count();
	this->mbps = ((double)(this->size * this->n_frames) / (1024. * 1024.)) / (time_duration * 1e-6);
	this->t_start = t_stop;

	for (auto f = f_start; f < f_stop; f++)
		this->reporter.probe(this->col_name, (void*)&mbps, frame_id);
}

template <typename T>
std::type_index Probe_throughput<T>
::get_datatype() const
{
	return typeid(double);
}

template <typename T>
void Probe_throughput<T>
::reset()
{
	this->t_start = std::chrono::steady_clock::now();
	this->mbps = 0.;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Probe_throughput<R_32>;
template class aff3ct::module::Probe_throughput<R_64>;
template class aff3ct::module::Probe_throughput<B_8 >;
template class aff3ct::module::Probe_throughput<B_16>;
template class aff3ct::module::Probe_throughput<B_32>;
template class aff3ct::module::Probe_throughput<B_64>;
// ==================================================================================== explicit template instantiation
