#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "../../Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"

#include "Modulator_GSM_TBLess.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM_TBLess<B,R,Q,MAX>
::Modulator_GSM_TBLess(int N, const R sigma, const bool disable_sig2,const std::string name)
: Modulator_GSM<B,R,Q,MAX>(N, sigma, disable_sig2, name, true)
{
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM_TBLess<B,R,Q,MAX>
::~Modulator_GSM_TBLess()
{
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_GSM_TBLess<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
	// *up_sample_factor because work with waveforms
	// *2: because of complex numbers
	return N * this->up_sample_factor *2; 
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_GSM_TBLess<B,R,Q,MAX>
::get_buffer_size_after_filtering(const int N)
{
	return (N * this->n_output_symbs); 
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((X_N1.size() * this->up_sample_factor * 2) == X_N2.size());

	if (this->n_frames == 1)
	{
		_modulate(X_N1, X_N2);
	}
	else // more than 1 frame
	{
		mipp::vector<B> X_N1_tmp(this->N);
		mipp::vector<R> X_N2_tmp(this->N * this->up_sample_factor * 2);
		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(X_N1.begin() + f * this->N, X_N1.begin() + (f +1) * this->N, X_N1_tmp.begin());

			_modulate(X_N1_tmp, X_N2_tmp);

			std::copy(X_N2_tmp.begin(), X_N2_tmp.end(), X_N2.begin() + f * (this->N * this->up_sample_factor * 2));
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::_modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((X_N1.size() * this->up_sample_factor * 2) == X_N2.size());

	// bit mapping -> not done here

	// Rimoldi phase tilting -> for GSM only consists in transforming -1/+1 in 0/1

	// CPE encoding
	Encoder_RSC3_CPE_sys<B> enc((int)X_N1.size(), (int)X_N1.size() * 2, 1, false);
	enc.encode_sys(X_N1, this->parity_enc);

	// memoryless modulation (attributing complex waveforms to symbols)
	const auto off_X_real  = 0;
	const auto off_X_imag  = X_N1.size() * this->up_sample_factor;
	const auto off_BB_real = 0;
	const auto off_BB_imag = (int)(this->baseband.size() / 2);

	const auto loop_size = (int)X_N1.size();
	for (auto i = 0; i < loop_size; i++)
		for (auto j = 0; j < this->up_sample_factor; j++)
		{
			X_N2[off_X_real + i * this->up_sample_factor +j] = this->baseband[off_BB_real + this->n_output_symbs * j + this->parity_enc[i]];
			X_N2[off_X_imag + i * this->up_sample_factor +j] = this->baseband[off_BB_imag + this->n_output_symbs * j + this->parity_enc[i]];
		}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	// /16 because 16 modulated symbols in GSM
	// *2  because we only keep real part here
	assert(Y_N1.size() / this->up_sample_factor == 2 * (Y_N2.size() / 16));

	if (this->n_frames == 1)
	{
		this->_filter(Y_N1, Y_N2);
	}
	else // mote than 1 frame
	{
		mipp::vector<R> Y_N1_tmp(this->N * this->up_sample_factor * 2);
		mipp::vector<R> Y_N2_tmp(this->N * this->n_output_symbs      );

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N * this->up_sample_factor * 2, 
			          Y_N1.begin() + (f +1) * this->N * this->up_sample_factor * 2,
			          Y_N1_tmp.begin());

			this->_filter(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * this->N * this->n_output_symbs);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size() * this->n_output_symbs);

	if (this->n_frames == 1)
	{
		this->BCJR.decode(Y_N1, Y_N2);
	}
	else // more than 1 frame
	{
		mipp::vector<Q> Y_N1_tmp(this->N * this->n_output_symbs);
		mipp::vector<Q> Y_N2_tmp(this->N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N * this->n_output_symbs,
			          Y_N1.begin() + (f +1) * this->N * this->n_output_symbs,
			          Y_N1_tmp.begin());

			this->BCJR.decode(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * this->N);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert(Y_N1.size() == Y_N3.size() * this->n_output_symbs);
	assert(Y_N2.size() == Y_N3.size());

	if (this->n_frames == 1)
	{
		this->BCJR.decode(Y_N1, Y_N2, Y_N3);
	}
	else
	{
		mipp::vector<Q> Y_N1_tmp(this->N * this->n_output_symbs);
		mipp::vector<Q> Y_N2_tmp(this->N);
		mipp::vector<Q> Y_N3_tmp(this->N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N * this->n_output_symbs,
			          Y_N1.begin() + (f +1) * this->N * this->n_output_symbs,
			          Y_N1_tmp.begin());
			std::copy(Y_N2.begin() + (f +0) * this->N, Y_N2.begin() + (f +1) * this->N, Y_N2_tmp.begin());

			this->BCJR.decode(Y_N1_tmp, Y_N2_tmp, Y_N3_tmp);

			std::copy(Y_N3_tmp.begin(), Y_N3_tmp.end(), Y_N3.begin() + f * this->N);
		}
	}
}
