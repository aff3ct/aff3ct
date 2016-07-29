/*
*
*
* BCJR Decoding, Memoryless Modulation, and Filtering come for Tarik BENADDI (Toulouse INP)
*
*
*/

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "Modulator_GSM_TBLess.hpp"
#include "../../Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"


//
//  PUBLIC METHODS /////////////////////////////////////////////////////////////////////////////////////////////////////
//

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM_TBLess<B,R,Q,MAX>
::Modulator_GSM_TBLess(int N, const R sigma)
: Modulator_GSM<B,R,Q,MAX>(N)
{
    // Adaptation From Modulator_GSM
    this->L_a_cpm.resize(N);
    this->L_ext_cpm.resize(N);
    this->proba_msg_symb.resize(N *this->m_order);
    this->proba_msg_bits.resize(N *2);
    this->wave_enc.resize(N *this->up_sample_factor *2);
    this->filtered_signal_r.resize(N *this->nb_output_symbs);
    this->symb_apriori_prob.resize(this->m_order *N);
    this->BCJR_gamma.resize(this->nb_states *this->m_order * N);
    this->BCJR_alpha.resize(this->nb_states *N);
    this->BCJR_beta.resize(this->nb_states  *N);
    this->log_bits_probability.resize(2 *N);
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
    return N *this->up_sample_factor *2; 
    // *up_sample_factor because work with waveforms
    // *2: because of complex numbers
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_GSM_TBLess<B,R,Q,MAX>
::get_buffer_size_after_filtering(const int N)
{
    return (N * this->nb_output_symbs); 
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) //const // Changed To allow Coset Approach in IterDEV
{
    assert((X_N1.size() * this->up_sample_factor * 2) == X_N2.size());


    // CPE Encoder Instanciation
    Encoder_RSC3_CPE_sys<B> enc(X_N1.size(), X_N1.size()*2, 1, false);

    // Bit Mapping -> Not done here

    // Rimoldi Phase Tilting -> For GSM only consists in transforming -1 / +1 in 0 / 1

    // CPE Encoding
    enc.encode_sys(X_N1, this->parity_enc);

    // Memoryless Modulation (Attributing Complex Waveforms to Symbols)
    R* wave_enc_r = this->wave_enc.data();
    R* wave_enc_i = this->wave_enc.data() + X_N1.size() * this->up_sample_factor;
    unsigned int ii;
    int          jj;
    for (ii = 0; ii < X_N1.size(); ii++)
    {
        for (jj = 0; jj < this->up_sample_factor; jj++)
        {
            wave_enc_r[ii * this->up_sample_factor + jj] = 
                this->baseband_vector_r[this->parity_enc[ii] + this->nb_output_symbs * jj];
            wave_enc_i[ii * this->up_sample_factor + jj] = 
                this->baseband_vector_i[this->parity_enc[ii] + this->nb_output_symbs * jj];
        }
    }

    X_N2 = this->wave_enc;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM_TBLess<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) //const // Changed To allow Coset Approach in IterDEV
{
        assert( Y_N1.size() == Y_N2.size() * this->nb_output_symbs );

        /* Estimator Processing */ 
        Q estimator = this->sigma * this->sigma;

        this->BCJR_decode(      /* inputs */   this->L_a_cpm,         estimator           ,   Y_N1,
                                /* outputs */  this->proba_msg_symb,  this->proba_msg_bits,   this->L_ext_cpm);

        Y_N2 = this->L_ext_cpm;
}