#include <string>
#include <vector>
#include <stdexcept>

#include "Monitor_std.hpp"

using namespace aff3ct::module;

template <typename B, typename R>
Monitor_std<B,R>
::Monitor_std(const int& K, const int& N, const int& N_mod, const unsigned& max_fe, const int& n_frames, const std::string name)
: Monitor<B,R>(K, N, N_mod, n_frames, name.c_str()),
  max_fe(max_fe),
  n_bit_errors(0),
  n_frame_errors(0),
  n_analyzed_frames(0)
{
}

template <typename B, typename R>
bool Monitor_std<B,R>
::fe_limit_achieved()
{
	return (get_n_fe() >= get_fe_limit()) || Monitor<B,R>::interrupt;
}

template <typename B, typename R>
void Monitor_std<B,R>
::_check_and_track_errors(const B *U_K, const B *V_K, const B *X_N, const R *Y_N_mod)
{
	if (__check_errors(U_K, V_K, this->K)) copy_bad_frame(U_K, X_N, Y_N_mod);

	n_analyzed_frames++;
}

template <typename B, typename R>
void Monitor_std<B,R>
::_check_errors(const B *U_K, const B *V_K)
{
	__check_errors(U_K, V_K, this->K);

	n_analyzed_frames++;
}

template <typename B, typename R>
bool Monitor_std<B,R>
::__check_errors(const B* U, const B* V, const int length)
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
::copy_bad_frame(const B* U_K, const B* X_N, const R* Y_N_mod)
{
	buff_src.  push_back(mipp::vector<B>(this->K    ));
	buff_enc.  push_back(mipp::vector<B>(this->N    ));
	buff_noise.push_back(mipp::vector<R>(this->N_mod));

	for (int b = 0; b < this->K; b++)
		buff_src.back()[b] = U_K[b];

	for (int b = 0; b < this->N; b++)
		buff_enc.back()[b] = X_N[b];

	for (int b = 0; b < this->N_mod; b++)
		buff_noise.back()[b] = Y_N_mod[b];// - X_mod[b];
}

template <typename B, typename R>
void Monitor_std<B,R>
::dump_bad_frames(const std::string& base_path, const float snr, mipp::vector<int> itl_pi)
{
	throw std::runtime_error("aff3ct::module::Monitor_std: \"dump_bad_frames\" is not defined in \"Monitor_std\" "
	                         "class, please call this method on \"Monitor_reduction\" instead.");
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
