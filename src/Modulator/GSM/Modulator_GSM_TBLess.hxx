/*
* BCJR Decoding, Memoryless Modulation, and Filtering come from Tarik BENADDI (Toulouse INP)
*/
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "../../Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"

#include "Modulator_GSM_TBLess.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM_TBLess<B,R,Q,MAX>
::Modulator_GSM_TBLess(int N, const R sigma, const bool disable_sig2, const std::string name)
: Modulator_GSM<B,R,Q,MAX>(N -6, sigma, disable_sig2, name)
{
	// resize buffers from the modulation/demodulation without tail bits
	this->parity_enc.resize(N +6);
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
	return N *this->up_sample_factor *2; 
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
    
	// bit mapping -> not done here

	// Rimoldi phase tilting -> for GSM only consists in transforming -1/+1 in 0/1

	// CPE encoding
	Encoder_RSC3_CPE_sys<B> enc(X_N1.size(), X_N1.size() * 2, 1, false);
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
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert(Y_N1.size() == Y_N2.size() * this->n_output_symbs);

	this->BCJR.decode(Y_N1, Y_N2);
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert(Y_N1.size() == Y_N3.size() * this->n_output_symbs);
	assert(Y_N2.size() == Y_N3.size());

	this->BCJR.decode(Y_N1, Y_N2, Y_N3);
}
