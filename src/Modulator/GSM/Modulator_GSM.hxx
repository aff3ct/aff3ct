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

#include "Modulator_GSM.hpp"
#include "../../Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"


//
//  BCJR FUNCTIONS FOR DEMODULATION ////////////////////////////////////////////////////////////////////////////////////
//

/* Maths Functions */

// Complete Version of Matricial Product
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::matCProd( /* Inputs */    int rowNb1, int columnNb1, R *mat1_r, R *mat1_i, 
                            int rowNb2, int columnNb2, R *mat2_r, R *mat2_i, 
            /* Outputs */   R *output_r, R *output_i) //const // Changed To allow Coset Approach in IterDEV
{
    // Init
    int 	i, j, k = 0;
    R  		sum_r = 0; 
    R 		sum_i = 0;
 
    // Size Check
    if (columnNb1 != rowNb2)
    {
        std::cout << "Matrices with entered orders can't be multiplied with each other." << std::endl;
        return;
    }
    else
    {
        // Processing...
        for (i = 0; i < rowNb1; i++) 
        {
            for (j = 0; j < columnNb2; j++) 
            {
                for (k = 0; k < rowNb2; k++) 
                {
                    //out_r += r1 * r2 - i1 * i2;
                    sum_r += mat1_r[i * columnNb1 + k] * mat2_r[j * rowNb2 + k] - mat1_i[i * columnNb1 + k] * mat2_i[j * rowNb2 + k];
                    //out_i += i1 * r2 + r1 * i2;
                    sum_i += mat1_i[i * columnNb1 + k] * mat2_r[j * rowNb2 + k] + mat1_r[i * columnNb1 + k] * mat2_i[j * rowNb2 + k];
                }
                output_r[j * rowNb1 + i] = sum_r;
                output_i[j * rowNb1 + i] = sum_i;
                sum_r = 0;
                sum_i = 0;
            }
        }
    }
}

// Real Part Output Only Version of Matricial Product 
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::R_matCProd(  	/* Inputs */    int rowNb1, int columnNb1, R *mat1_r, R *mat1_i, 
                   	            int rowNb2, int columnNb2, R *mat2_r, R *mat2_i, 
                /* Outputs */   mipp::vector<R> &output_r) //const // Changed To allow Coset Approach in IterDEV
{
    // Init
    B   i, j, k = 0;
    R  	sum_r = 0;
 
    // Size Check
    if (columnNb1 != rowNb2)
    {
        std::cout << "Matrices with entered orders can't be multiplied with each other." << std::endl;
        return;
    }
    else
    {
        // Processing...
        for (i = 0; i < rowNb1; i++) 
        {
            for (j = 0; j < columnNb2; j++) 
            {
                for (k = 0; k < rowNb2; k++) 
                {
                    //out_r += r1 * r2 - i1 * i2;
                    sum_r += mat1_r[i * columnNb1 + k] * mat2_r[j * rowNb2 + k] - mat1_i[i * columnNb1 + k] * mat2_i[j * rowNb2 + k];
                }
                output_r[j * rowNb1 + i] = sum_r;
                sum_r = 0;
            }
        }
    }
}


/* BCJR Algorithm Functions */

// Retrieve Log Symbols Probability from LLR
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_LLR2LogSymbProba(const   mipp::vector<Q> &llr, 
                                mipp::vector<Q> &log_proba_msg_symb) //const // Changed To allow Coset Approach in IterDEV
{
    unsigned int depth, ii, jj;

    for (ii = 0; ii < llr.size(); ii++)
    {
        this->log_bits_probability[2 * ii]        = llr[ii] / 2;
        this->log_bits_probability[1 + 2 * ii]    = -llr[ii] / 2;
    }

    for (ii = 0; ii < (unsigned int)m_order; ii++)
    {
        for (jj = 0; jj < log_proba_msg_symb.size() / m_order; jj++)
        {
            log_proba_msg_symb[ii + m_order * jj] = 0;
        }
    }
        
    for (depth = 0; depth < log_proba_msg_symb.size() / m_order; depth++)
    {
        for (ii = 0; ii < (unsigned int)m_order; ii++)
        {
            for (jj = 0; jj < (unsigned int)nb_bits_per_symb; jj++)
            {
                if (binary_symbols[ii + m_order * jj] == 0)
                {
                    log_proba_msg_symb[ii + m_order * depth] = log_proba_msg_symb[ii + m_order * depth] + log_bits_probability[2 * (depth * nb_bits_per_symb + jj)];
                }
                else if (binary_symbols[ii + m_order * jj] == 1)
                {
                    log_proba_msg_symb[ii + m_order * depth] = log_proba_msg_symb[ii + m_order * depth] + log_bits_probability[1 + 2 * (depth * nb_bits_per_symb + jj)];
                }
            }
        }
    }

}

// Compute Gamma, Alpha and Beta, heart of the processing
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_compute_alpha_beta_gamma(
                                const B               *trellis,                const B   *anti_trellis, 
                                const mipp::vector<Q> &symbol_apriori_prob,    const int M, 
                                const int             cpm_symb_message_length, 
                                const int             nb_output_symbs,         const Q   noise_var, 
                                const mipp::vector<Q> &signal, 

                                      mipp::vector<Q> &gamma,                 mipp::vector<Q> &alpha, 
                                      mipp::vector<Q> &beta) //const // Changed To allow Coset Approach in IterDEV
{
    //
    //  VARIABLES DECLARATION
    //

    Q      log_lambda, beta_final_equi_proba, sum_proba_alpha, sum_proba_beta;
    int    ii, jj, kk, symb_ctr, depth_ctr, state_ctr, branch_ctr;
    B      output_symbol;
    B     *ptr_prev_states;
    B     *ptr_prev_symb;
    B     *input_symb, *original_state;

    //
    //  ALPHA, BETA, AND GAMMA INITIALIZATION
    //

    for (ii = 0; ii < nb_states; ii++)
    {
        for (jj = 0; jj < cpm_symb_message_length; jj++)
        {
            alpha[ ii + nb_states * jj ] = -std::numeric_limits<Q>::infinity();
            beta[ ii + nb_states * jj ]  = -std::numeric_limits<Q>::infinity();
        }
    }
    alpha[0] = 0;
    beta_final_equi_proba = 1./ nb_states;
    for (ii = 0; ii < nb_states; ii++)
    {
        beta[ ii + nb_states * (cpm_symb_message_length - 1) ] = log(beta_final_equi_proba);
    }
    
    //
    //  PROCESSING STARTS HERE
    //      

    /* Compute Gamma */

    // For all Symbols
    for (ii = 0; ii < cpm_symb_message_length; ii++)
    {
        // For all States
        for (jj = 0; jj < nb_states; jj++)
        {
            // For all Values of the alphabet (M is the Modulation Order)
            for (kk = 0; kk < M; kk++)
            {
                output_symbol       = trellis[ jj + nb_states * (kk + M) ];
                log_lambda          = 2 * signal[ ii + cpm_symb_message_length * output_symbol ] / noise_var; 
                gamma[ ii + cpm_symb_message_length * ( jj + nb_states * kk ) ] = log_lambda + symbol_apriori_prob[ kk + M * ii ];
            }
        }
    }

    /* Compute alpha and beta */
    for (depth_ctr = 1; depth_ctr < cpm_symb_message_length; depth_ctr++)
    {
        for (state_ctr = 0; state_ctr < nb_states; state_ctr++)
        {

            /* compute alpha */
            /**/
            ptr_prev_states = (B*) anti_trellis + state_ctr * nb_prev_branches;

            if (ptr_prev_states != NULL) //A tester
            {

                ptr_prev_symb   = (B*) anti_trellis + state_ctr * nb_prev_branches + nb_states * nb_prev_branches;
                original_state  = ptr_prev_states;
                input_symb      = ptr_prev_symb;

                for (branch_ctr = 0; branch_ctr < nb_prev_branches; branch_ctr++)
                {

                    alpha[ state_ctr + nb_states * depth_ctr ] = 
                        MAX(
                                    alpha[ state_ctr + nb_states * depth_ctr ], 
                                    gamma[ depth_ctr - 1 + cpm_symb_message_length * (original_state[branch_ctr] + nb_states * input_symb[branch_ctr] ) ]
                                    + alpha[original_state[branch_ctr] + nb_states * (depth_ctr - 1)]
                                   );
                }

            }

            /* Compute beta */
            /**/
            for (symb_ctr = 0; symb_ctr < M; symb_ctr++)
            {
                beta[state_ctr + nb_states * (cpm_symb_message_length - (depth_ctr + 1))] = 
                    MAX(
                                beta[ state_ctr + nb_states * (cpm_symb_message_length - (depth_ctr + 1)) ],
                                gamma[ cpm_symb_message_length - depth_ctr + cpm_symb_message_length * (state_ctr + nb_states * symb_ctr) ]
                                + beta[ trellis[ state_ctr + nb_states * symb_ctr ] + nb_states * (cpm_symb_message_length - depth_ctr) ]
                               );
            }
        }
        
        /* Normalize vectors alpha and beta*/
        if (depth_ctr != 0)
        {
            sum_proba_alpha = -std::numeric_limits<Q>::infinity();
            sum_proba_beta  = -std::numeric_limits<Q>::infinity();
            for (ii = 0; ii < nb_states; ii++)
            {
                sum_proba_alpha = MAX(sum_proba_alpha, alpha[ ii + nb_states * depth_ctr ]);
                sum_proba_beta  = MAX(sum_proba_beta,   beta[ ii + nb_states * (cpm_symb_message_length - depth_ctr - 1) ]);
            }
            /**/
            for (ii = 0; ii < nb_states; ii++)
            {
                alpha[ ii + nb_states * depth_ctr ]                                 -= sum_proba_alpha;
                 beta[ ii + nb_states * (cpm_symb_message_length - depth_ctr - 1) ] -= sum_proba_beta;
            }
            /**/
        }
    }
}

// From Alpha, Beta, and Gamma computes new Symbol Probability
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_symboles_probas(const B               *trellis,                   const mipp::vector<Q> &alpha, 
                       const mipp::vector<Q> &beta,                      const mipp::vector<Q> &gamma, 
                       const int             cpm_symb_message_length,    const int M,
                             mipp::vector<Q> &proba_msg_symb) //const // Changed To allow Coset Approach in IterDEV
{
    int ii, jj, depth_ctr, symb_ctr, state_ctr;
    
    /* Initialize proba_msg_symb */
    for (ii = 0; ii < M; ii++)
    {
        for (jj = 0; jj < cpm_symb_message_length; jj++)
        {
            proba_msg_symb[ii + M * jj] = -std::numeric_limits<Q>::infinity();
        }
    }

    for (depth_ctr = 0; depth_ctr < cpm_symb_message_length; depth_ctr++)
    {
        for (symb_ctr = 0; symb_ctr < M; symb_ctr++)
        {
            for (state_ctr = 0; state_ctr < nb_states; state_ctr++)
            {
                proba_msg_symb[symb_ctr + M * depth_ctr] = MAX(
                                        proba_msg_symb[symb_ctr + M * depth_ctr],
                                        alpha[state_ctr + nb_states * depth_ctr]
                                        + gamma[depth_ctr + cpm_symb_message_length*(state_ctr + nb_states*symb_ctr)]
                                        + beta[trellis[state_ctr + nb_states*symb_ctr] + nb_states * depth_ctr]);
            }
        }
    }
}

// From Symbol Probabilities, computes bit probabilities
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_bits_probas( const mipp::vector<Q> &proba_msg_symb,            const B       *binary_symbols, 
                    const int             M,                          const int     cpm_bits_message_length, 
                    const int             cpm_symb_message_length,    const int     nb_bits_per_input_symb, 
                          mipp::vector<Q> &proba_msg_bits) //const // Changed To allow Coset Approach in IterDEV
{

    int depth_ctr, index_start, bit_ctr, jj, symb_ctr;

    /* Initialize proba_msg_bits */
    for (jj = 0; jj < 2 * cpm_bits_message_length; jj++)
    {
        proba_msg_bits[jj] = -std::numeric_limits<Q>::infinity();
    }

    for (depth_ctr = 0; depth_ctr < cpm_symb_message_length; depth_ctr++)
    {
        index_start = depth_ctr * nb_bits_per_input_symb;

        for (bit_ctr = 0; bit_ctr < nb_bits_per_input_symb; bit_ctr++)
        {
            for (symb_ctr = 0; symb_ctr < M; symb_ctr++)
            {
                // Bit 0
                if ( binary_symbols[symb_ctr + M * bit_ctr] == 0 )
                {
                    proba_msg_bits[2*(index_start + bit_ctr)] = 
                                MAX(
                                            proba_msg_bits[2*(index_start + bit_ctr)], 
                                            proba_msg_symb[symb_ctr +  M * depth_ctr]
                                           );
                }
                // Bit 1
                else if ( binary_symbols[symb_ctr + M * bit_ctr] == 1 )
                {
                    proba_msg_bits[1 + 2*(index_start + bit_ctr)] = 
                                MAX(
                                            proba_msg_bits[1 + 2*(index_start + bit_ctr)],
                                            proba_msg_symb[symb_ctr + M * depth_ctr]
                                           );
                }
            }
        }
    }
}

// Extrinsic information processing from bit probabilities and CPM A Priori LLR
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_compute_ext( const mipp::vector<Q> &proba_msg_bits, const mipp::vector<Q> &llr,
                          mipp::vector<Q> &L_ext) //const // Changed To allow Coset Approach in IterDEV
{
    unsigned int ii;
    for (ii = 0; ii < L_ext.size() * 2; ii += 2)
    {
        // Processing aposteriori and substracting a priori to directly obtain extrinsic
        L_ext[ii / 2] = proba_msg_bits[ii] - proba_msg_bits[ii + 1] - llr[ii / 2];
    }
}

// High Level function to use in code
template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::BCJR_decode(const mipp::vector<Q> &llr,            const Q noise_var,                     
              const mipp::vector<Q> &signal,
                    mipp::vector<Q> &proba_msg_symb, mipp::vector<Q> &proba_msg_bits, 
                    mipp::vector<Q> &L_ext) //const // Changed To allow Coset Approach in IterDEV
{

    BCJR_LLR2LogSymbProba(  /* inputs */    llr,                    
                            /* outputs */   this->symb_apriori_prob);

    BCJR_compute_alpha_beta_gamma(  (B*) trellis,   (B*) anti_trellis,  this->symb_apriori_prob,  m_order,            proba_msg_symb.size() / m_order,
                                    nb_output_symbs, noise_var, signal, this->BCJR_gamma,   this->BCJR_alpha,   this->BCJR_beta);

    BCJR_symboles_probas((B*) trellis, this->BCJR_alpha, this->BCJR_beta, this->BCJR_gamma, proba_msg_symb.size() / m_order, m_order, proba_msg_symb);

    BCJR_bits_probas(proba_msg_symb, (B*) binary_symbols, m_order, proba_msg_bits.size() / 2, proba_msg_symb.size() / m_order, nb_bits_per_symb, proba_msg_bits);

    BCJR_compute_ext(   /* Inputs */    proba_msg_bits, llr,
                        /* Outputs */   L_ext);

}

//
//  OTHER PUBLIC METHODS ///////////////////////////////////////////////////////////////////////////////////////////////
//


template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM<B,R,Q,MAX>
::Modulator_GSM(int N, const R sigma)
: Modulator<B,R,Q>(),
  sigma(sigma),
  parity_enc(N + 6),
  wave_enc((N +6) * UP_SAMPLE_FACTOR * 2),
  L_a_cpm(N +6, 0),
  L_ext_cpm(N +6, 0),
  proba_msg_symb((N +6) *M_ORDER, 0),
  proba_msg_bits((N +6) *2, 0),
  filtered_signal_r((N +6) * NB_OUTPUT_SYMBS),
  symb_apriori_prob(M_ORDER *(N +6)),
  BCJR_gamma(NB_STATES *M_ORDER * (N +6)),
  BCJR_alpha(NB_STATES *(N +6)),
  BCJR_beta(NB_STATES  *(N +6)),
  log_bits_probability(2 *(N +6))
{
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_GSM<B,R,Q,MAX>
::~Modulator_GSM()
{
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_GSM<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
    return (N +6) *up_sample_factor *2;
    // +6: tails bit
    // *up_sample_factor because work with waveforms
    // *2: because of complex numbers
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_GSM<B,R,Q,MAX>
::get_buffer_size_after_filtering(const int N)
{
    return (N +6) * nb_output_symbs;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2) //const // Changed To allow Coset Approach in IterDEV
{
	assert(((X_N1.size() +6) *up_sample_factor *2) == X_N2.size());


	// CPE Encoder Instanciation
	Encoder_RSC3_CPE_sys<B> enc(X_N1.size(), X_N1.size()*2, 1, false);

	// Bit Mapping -> Not done here

	// Rimoldi Phase Tilting -> For GSM only consists in transforming -1 / +1 in 0 / 1

	// CPE Encoding
	enc.encode_sys(X_N1, this->parity_enc);

	// Memoryless Modulation (Attributing Complex Waveforms to Symbols)
	R* wave_enc_r = this->wave_enc.data();
	R* wave_enc_i = this->wave_enc.data() + (X_N1.size() +6) * up_sample_factor;
	unsigned int ii;
    int          jj;
    for (ii = 0; ii < X_N1.size() +6; ii++)
	{
		for (jj = 0; jj < up_sample_factor; jj++)
		{
			wave_enc_r[ii * up_sample_factor + jj] = baseband_vector_r[this->parity_enc[ii] + nb_output_symbs * jj];
			wave_enc_i[ii * up_sample_factor + jj] = baseband_vector_i[this->parity_enc[ii] + nb_output_symbs * jj];
		}
	}

	X_N2 = this->wave_enc;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) //const // Changed To allow Coset Approach in IterDEV
{
    	assert( Y_N1.size() == (Y_N2.size() +6) * nb_output_symbs );

        /* Estimator Processing */ 
        Q   estimator  = this->sigma * this->sigma;


        BCJR_decode(    /* inputs */   this->L_a_cpm,         estimator /*Estimator*/,      this->filtered_signal_r,
                        /* outputs */  this->proba_msg_symb,  this->proba_msg_bits,   this->L_ext_cpm);

        for(unsigned int i = 0; i < Y_N2.size(); i++)
            Y_N2[i] = L_ext_cpm[i];
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_GSM<B,R,Q,MAX>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2) //const // Changed To allow Coset Approach in IterDEV
{

    assert( Y_N1.size() / up_sample_factor == 2 * (Y_N2.size() / 16) ); 
    // /16 Because 16 Modulated Symbols in GSM
    // *2  Because we only keep real part here

    // Incoming Data
    R* signal_r = (R*) Y_N1.data();
    R* signal_i = (R*) Y_N1.data() + (Y_N1.size() / 2);

    unsigned int size_signal = (Y_N1.size() / 2);

    // // Canal Values Testing:
    // std::cout << "================ CANAL ================" << std::endl << std::endl;
    // std::cout << std::setprecision(8);
    // for(unsigned int rr = 0; rr < Y_N1.size() /2; rr++)
    // {
    //  std::cout << signal_r[rr] << " + i " << signal_i[rr] << "," << std::endl;
    // }
    // std::cout << std::endl;

    // Filtering
    this->R_matCProd(   
                        /*IN*/
                        size_signal / up_sample_factor,  up_sample_factor, 
                        signal_r,                        signal_i, 
                        up_sample_factor,                nb_output_symbs,  
                        (R*) proj_r,                     (R*) proj_i,
                        /*OUT*/
                        Y_N2
                    );

    // // Filter Values Testing:
    // std::cout << "================ FILTER ================" << std::endl << std::endl;
    // std::cout << std::setprecision(8);
    // for(unsigned int rr = 0; rr < this->filtered_signal_r.size(); rr++)
    // {
    //  std::cout << this->filtered_signal_r[rr] << ", " << std::endl;
    // }
    // std::cout << std::endl;
}