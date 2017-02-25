#include "Monitor_reduction.hpp"

using namespace aff3ct::module;

template <typename B>
Monitor_reduction<B>
::Monitor_reduction(const int& K, const int& N, const int& max_fe,
                    std::vector<Monitor<B>*>& error_analyzers, const int& n_frames,
                    const std::string name)
: Monitor_std<B>(K, N, max_fe, n_frames, name), error_analyzers(error_analyzers)
{
}

template <typename B>
Monitor_reduction<B>
::~Monitor_reduction()
{
}

template <typename B>
unsigned long long Monitor_reduction<B>
::get_n_analyzed_fra() const
{
	unsigned long long cur_fra = this->n_analyzed_frames;
	for (unsigned i = 0; i < error_analyzers.size(); i++)
		cur_fra += error_analyzers[i]->get_n_analyzed_fra();

	return cur_fra;
}

template <typename B>
int Monitor_reduction<B>
::get_n_fe() const
{
	auto cur_fe = this->n_frame_errors;
	for (unsigned i = 0; i < error_analyzers.size(); i++)
		cur_fe += error_analyzers[i]->get_n_fe();

	return cur_fe;
}

template <typename B>
int Monitor_reduction<B>
::get_n_be() const
{
	auto cur_be = this->n_bit_errors;
	for (unsigned i = 0; i < error_analyzers.size(); i++)
		cur_be += error_analyzers[i]->get_n_be();

	return cur_be;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Monitor_reduction<B_8>;
template class Monitor_reduction<B_16>;
template class Monitor_reduction<B_32>;
template class Monitor_reduction<B_64>;
#else
template class Monitor_reduction<B>;
#endif
// ==================================================================================== explicit template instantiation
