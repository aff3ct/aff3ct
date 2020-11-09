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
	this->set_single_wave(true);
}

template <typename T>
void Probe_throughput<T>
::_probe(const T *in, const size_t frame_id)
{
	auto t_stop = std::chrono::steady_clock::now();
	auto time_duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(t_stop - this->t_start).count();
	this->mbps = ((double)(this->size * this->n_frames) / (1024. * 1024.)) / (time_duration * 1e-6);
	this->t_start = t_stop;

	for (size_t f = 0; f < this->get_n_frames(); f++)
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
