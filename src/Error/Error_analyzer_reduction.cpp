#include "../Error/Error_analyzer_reduction.hpp"

template <typename B, typename R>
Error_analyzer_reduction<B,R>
::Error_analyzer_reduction(const int& K, const int& N, const int& max_fe, 
                           std::vector<Error_analyzer<B,R>*>& error_analyzers, const int& n_frames,
                           const std::string name)
: Error_analyzer<B,R>(K, N, max_fe, n_frames, name),
  error_analyzers(error_analyzers),
  prev_n_be(error_analyzers.size()),
  prev_n_fe(error_analyzers.size()),
  prev_n_analyzed_frames(error_analyzers.size())
{
	std::fill(prev_n_be             .begin(), prev_n_be             .end(), 0);
	std::fill(prev_n_fe             .begin(), prev_n_fe             .end(), 0);
	std::fill(prev_n_analyzed_frames.begin(), prev_n_analyzed_frames.end(), 0);

	this->reduce();
}

template <typename B, typename R>
Error_analyzer_reduction<B,R>
::~Error_analyzer_reduction()
{
}

template <typename B, typename R>
void Error_analyzer_reduction<B,R>
::reduce()
{
	for (unsigned i = 0; i < error_analyzers.size(); i++)
	{
		if (error_analyzers[i]->get_n_analyzed_frames())
		{
			auto cur_n_be              = error_analyzers[i]->get_n_be();
			auto cur_n_fe              = error_analyzers[i]->get_n_fe();
			auto cur_n_analyzed_frames = error_analyzers[i]->get_n_analyzed_frames();
			
			this->n_bit_errors      += cur_n_be              - prev_n_be[i];
			// this is made in "increment_frame_errors" subroutine
			// this->n_frame_errors    += cur_n_fe              - prev_n_fe[i];
			this->n_analyzed_frames += cur_n_analyzed_frames - prev_n_analyzed_frames[i];

			prev_n_be             [i] = cur_n_be;
			prev_n_fe             [i] = cur_n_fe;
			prev_n_analyzed_frames[i] = cur_n_analyzed_frames;
		}
	}
}

template <typename B, typename R>
void Error_analyzer_reduction<B,R>
::increment_frame_errors(const int n_frames)
{
	this->mutex_total_frame_errors.lock();
	this->n_frame_errors += n_frames;
	this->mutex_total_frame_errors.unlock();
}

// ==================================================================================== explicit template instantiation 
#include "../Tools/types.h"
#ifdef MULTI_PREC
template class Error_analyzer_reduction<B_8,R_8>;
template class Error_analyzer_reduction<B_16,R_16>;
template class Error_analyzer_reduction<B_32,R_32>;
template class Error_analyzer_reduction<B_64,R_64>;
#else
template class Error_analyzer_reduction<B,R>;
#endif
// ==================================================================================== explicit template instantiation