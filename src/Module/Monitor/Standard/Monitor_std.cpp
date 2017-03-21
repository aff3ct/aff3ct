#include <string>
#include <vector>
#include <cassert>

#include "Tools/Display/bash_tools.h"

#include "Monitor_std.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;

template <typename B, typename R>
Monitor_std<B,R>
::Monitor_std(const int& K, const int& N, const unsigned& max_fe, const int& n_frames, const std::string name)
: Monitor<B,R>(K, N, n_frames, name.c_str()),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0)
{
	assert(n_frames > 0);
}

template <typename B, typename R>
bool Monitor_std<B,R>
::fe_limit_achieved()
{
	return (get_n_fe() >= get_fe_limit()) || Monitor<B,R>::interrupt;
}

template <typename B, typename R>
void Monitor_std<B,R>
::update_n_analyzed_frames()
{
	n_analyzed_frames += (unsigned long long)this->n_frames;
}

template <typename B, typename R>
void Monitor_std<B,R>
::check_and_track_errors(const mipp::vector<B>& U,
                         const mipp::vector<B>& V,
                         const mipp::vector<B>& X,
                         const mipp::vector<R>& X_mod,
                         const mipp::vector<R>& Y)
{
	assert(this->K * this->n_frames == (int)U    .size());
	assert(this->K * this->n_frames == (int)V    .size());
	assert(this->N * this->n_frames == (int)X    .size());
	assert(Y.size()                 ==      X_mod.size());

	const int Y_size = (int)X_mod.size() / this->n_frames;

	for (auto i = 0; i < this->n_frames; i++)
		if (check_errors(U.data() + i * this->K, V.data() + i * this->K, this->K))
			copy_bad_frame(U    .data() + i * this->K,
			               X    .data() + i * this->N,
			               X_mod.data() + i * Y_size,
			               Y    .data() + i * Y_size,
			               Y_size);

	this->update_n_analyzed_frames();
}

template <typename B, typename R>
bool Monitor_std<B,R>
::check_errors(const B* U, const B* V, const int length)
{
	auto bit_errors_count = 0;
	for (auto b = 0; b < length; b++)
		bit_errors_count += U[b] != V[b];

	if (bit_errors_count)
	{
		n_bit_errors += bit_errors_count;
		n_frame_errors++;

		return true;
	}

	return false;
}

template <typename B, typename R>
void Monitor_std<B,R>
::check_errors(const mipp::vector<B>& U, const mipp::vector<B>& V)
{
	assert(U.size() == V.size());
	auto n = (int)U.size() / this->n_frames;

	for (auto i = 0; i < this->n_frames; i++)
		check_errors(U.data() + i * n, V.data() + i * n, n);

	this->update_n_analyzed_frames();
}

template <typename B, typename R>
unsigned Monitor_std<B,R>
::get_fe_limit() const
{
	return max_fe;
}

template <typename B, typename R>
unsigned long long Monitor_std<B,R>
::get_n_analyzed_fra() const
{
	return n_analyzed_frames;
}

template <typename B, typename R>
unsigned long long Monitor_std<B,R>
::get_n_fe() const
{
	return n_frame_errors;
}

template <typename B, typename R>
unsigned long long Monitor_std<B,R>
::get_n_be() const
{
	return n_bit_errors;
}

template <typename B, typename R>
float Monitor_std<B,R>
::get_fer() const
{
	auto t_fer = (float)get_n_fe() / (float)get_n_analyzed_fra();
	return t_fer;
}

template <typename B, typename R>
float Monitor_std<B,R>
::get_ber() const
{
	auto t_ber = (float)get_n_be() / (float)get_n_analyzed_fra() / (float)this->K;
	return t_ber;
}

template <typename B, typename R>
void Monitor_std<B,R>
::copy_bad_frame(const B* U, const B* X, const R* X_mod, const R* Y, const int Y_size)
{
	buff_src.  push_back(mipp::vector<B>(this->K));
	buff_enc.  push_back(mipp::vector<B>(this->N));
	buff_noise.push_back(mipp::vector<R>(Y_size ));

	for (int b = 0; b < this->K; b++)
		buff_src.back()[b] = U[b];

	for (int b = 0; b < this->N; b++)
		buff_enc.back()[b] = X[b];

	for (int b = 0; b < Y_size; b++)
		buff_noise.back()[b] = Y[b];// - X_mod[b];
}

template <typename B, typename R>
void Monitor_std<B,R>
::dump_bad_frames(const std::string& base_path, const float snr, mipp::vector<int> itl_pi)
{
	std::cerr << bold_red("\"dump_bad_frames\" is not defined in \"Monitor_std\", please call this method on ")
	          << bold_red("\"Monitor_reduction\" instead.")
	          << std::endl;

	std::exit(-1);
}

template <typename B, typename R>
const std::vector<mipp::vector<B>> Monitor_std<B,R>
::get_buff_src() const
{
	return buff_src;
};

template <typename B, typename R>
const std::vector<mipp::vector<B>> Monitor_std<B,R>
::get_buff_enc() const
{
	return buff_enc;
};

template <typename B, typename R>
const std::vector<mipp::vector<R>> Monitor_std<B,R>
::get_buff_noise() const
{
	return buff_noise;
};

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Monitor_std<B_8 ,R_8>;
template class aff3ct::module::Monitor_std<B_16,R_16>;
template class aff3ct::module::Monitor_std<B_32,R_32>;
template class aff3ct::module::Monitor_std<B_64,R_64>;
#else
template class aff3ct::module::Monitor_std<B,R>;
#endif
// ==================================================================================== explicit template instantiation
