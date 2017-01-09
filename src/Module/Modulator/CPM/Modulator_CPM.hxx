/*
 * Modulator_CPM.hxx
 *
 *  Created on: 14 dec. 2016
 *      Author: ohartmann
 */

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctgmath>

#include "Tools/Math/matrix.h"
#include "../../Encoder/RSC/Encoder_RSC3_CPE_sys.hpp"
#include "Modulator_CPM.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_CPM<B,R,Q,MAX>
::Modulator_CPM(int _N, const modulator_parameters &params, const R sigma, const int n_frames, const std::string name)
:
 	 Modulator<B,R,Q>(_N, 0, 0, n_frames, name), // Warning !!! Won't work if N_mod and N_fil are used since this constructor is not completely done
 	 N		 (_N							  ),
	 sigma   (sigma							  ),
	 cpm     (params.cpm_std,
			  params.cpm_L,
			  params.cpm_k,
			  params.cpm_p,
			  params.bits_per_symbol,
			  params.upsample_factor,
			  "TOTAL",
			  params.wave_shape				  ),
	 cpm_h   ((R)cpm.k/(R)cpm.p               ),
	 T_samp  ((R)1.0  /(R)cpm.Sf              ),
	 baseband(  cpm.max_wa_id *cpm.Sf *2, 0   ),
	 projection(cpm.max_wa_id *cpm.Sf *2, 0   ),
	 Nb_sy   (N/cpm.Nb_b_per_s				  ),
	 Nb_sy_tl(Nb_sy+cpm.tl					  ),
	 cpe     (Nb_sy, cpm, this->n_frames	  ),
	 bcjr    (cpm, Nb_sy_tl					  )
{
	assert(N%params.bits_per_symbol == 0);

	// write the right buffers sizes
	int* N_mod_writable = const_cast<int*>(&this->N_mod     );
	*N_mod_writable 	= get_buffer_size_after_modulation(N);
	int* N_fil_writable = const_cast<int*>(&this->N_fil     );
	*N_fil_writable 	= get_buffer_size_after_filtering(N );

	// initialize CPM
	cpe.generate_allowed_states    (cpm.allowed_states               );
	cpe.generate_allowed_wave_forms(cpm.allowed_wave_forms           );

	cpe.generate_mapper            (cpm.transition_to_binary,
								    cpm.binary_to_transition,
									params.mapping      		     );

	cpe.generate_trellis           (cpm.trellis_next_state,
									cpm.trellis_related_wave_form    );
	cpe.generate_anti_trellis      (cpm.anti_trellis_original_state,
									cpm.anti_trellis_input_transition);

	generate_baseband  			   (                                 );
	generate_projection			   (                                 );

//	std::cout << "allowed_states : " << std::endl;
//
//	for (int st = 0; st < cpm.Nb_st; st++)
//	{
//		std::cout << cpm.allowed_states[st] << " ";
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "allowed_wave_forms : " << std::endl;
//
//	for (int st = 0; st < cpm.Nb_wa; st++)
//	{
//		std::cout << cpm.allowed_wave_forms[st] << " ";
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//
//	std::cout << "transition_to_binary (msb to lsb): " << std::endl;
//
//	for (int tr = 0; tr < cpm.Mo; tr++)
//	{
//		for (int b = 0; b < cpm.Nb_b_per_s; b++)
//		{
//			std::cout << cpm.transition_to_binary[tr * cpm.Nb_b_per_s + b] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "binary_to_transition : " << std::endl;
//
//	for (int tr = 0; tr < cpm.Mo; tr++)
//	{
//		std::cout << cpm.binary_to_transition[tr] << " ";
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "trellis_next_state : " << std::endl;
//
//	for (int S = 0; S < (int)cpm.trellis_next_state.size()/cpm.Mo; ++S)
//	{
//		for (int tr = 0; tr < cpm.Mo; ++tr)
//		{
//			std::cout << cpm.trellis_next_state[S*cpm.Mo + tr] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "trellis_related_wave_form : " << std::endl;
//
//	for (int S = 0; S < (int)cpm.trellis_related_wave_form.size()/cpm.Mo; ++S)
//	{
//		for (int tr = 0; tr < cpm.Mo; ++tr)
//		{
//			std::cout << cpm.trellis_related_wave_form[S*cpm.Mo + tr] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "anti_trellis_original_state : " << std::endl;
//
//	for (int S = 0; S < (int)cpm.anti_trellis_original_state.size()/cpm.Mo; ++S)
//	{
//		for (int tr = 0; tr < cpm.Mo; ++tr)
//		{
//			std::cout << cpm.anti_trellis_original_state[S*cpm.Mo +tr] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout << "anti_trellis_input_transition : " << std::endl;
//
//	for (int S = 0; S < (int)cpm.anti_trellis_input_transition.size()/cpm.Mo; ++S)
//	{
//		for (int tr = 0; tr < cpm.Mo; ++tr)
//		{
//			std::cout << cpm.anti_trellis_input_transition[S*cpm.Mo +tr] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//
//	std::cout<< std::endl << "baseband : " << std::endl;
//	for (int wa = 0; wa < cpm.max_wa_id; ++wa)
//	{
//		for(int s = 0 ; s < cpm.Sf ; s++)
//		{
//			std::cout << baseband[wa*cpm.Sf+s] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//	for (int wa = 0; wa < cpm.max_wa_id; ++wa)
//	{
//		for(int s = 0 ; s < cpm.Sf ; s++)
//		{
//			std::cout << baseband[baseband.size()/2+wa*cpm.Sf+s] << " ";
//		}
//		std::cout <<  std::endl;
//	}
//	std::cout <<  std::endl;
//
//	std::cout<< std::endl << "projection = [... " << std::endl;
//	for(int s = 0 ; s < cpm.Sf ; s++)
//	{
//		for (int wa = 0; wa < cpm.Nb_wa; ++wa)
//		{
//			std::cout << projection[s*cpm.max_wa_id+cpm.allowed_wave_forms[wa]] << " ";
//		}
//		std::cout <<((s+1 == cpm.Sf)?"]..." : ";...")<<  std::endl;
//	}
//	std::cout <<"+1i*[..." << std::endl;
//	for(int s = 0 ; s < cpm.Sf ; s++)
//	{
//		for (int wa = 0; wa < cpm.Nb_wa; ++wa)
//		{
//			std::cout << projection[projection.size()/2+s*cpm.max_wa_id+cpm.allowed_wave_forms[wa]] << " ";
//		}
//		std::cout <<((s+1 == cpm.Sf)?"];" : ";...")<<  std::endl;
//	}
//	std::cout <<  std::endl;

	//exit(-1);
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
Modulator_CPM<B,R,Q,MAX>
::~Modulator_CPM()
{

}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_CPM<B,R,Q,MAX>
::get_buffer_size_after_modulation(const int N)
{
	// N : number of bits to send
	//  / cpm.Nb_b_per_s: because buffer contains symbols and not bits anymore
	// +cpm.tl: tails symbols
	// *cpm.Sf: because work with samples of wave forms
	// *2: because of complex numbers
	return (N/cpm.Nb_b_per_s + cpm.tl) * cpm.Sf * 2;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
int Modulator_CPM<B,R,Q,MAX>
::get_buffer_size_after_filtering(const int N)
{
	// N : number of bits to receive
	// / cpm.Nb_b_per_s: because buffer contains symbols and not bits
	// +cpm.tl: tails symbols
	// *cpm.Nb_wa: because work with waveforms probability
	return (N/cpm.Nb_b_per_s + cpm.tl) * cpm.max_wa_id;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((int)X_N1.size() ==           N*this->n_frames);
	assert((int)X_N2.size() == this->N_mod*this->n_frames);

	if (this->n_frames == 1)
	{
		_modulate(X_N1, X_N2);
	}
	else // more than 1 frame
	{
		mipp::vector<B> X_N1_tmp(N);
		mipp::vector<R> X_N2_tmp(this->N_mod);
		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(X_N1.begin() + f * N,
					  X_N1.begin() + (f +1) * N,
					  X_N1_tmp.begin());

			_modulate(X_N1_tmp, X_N2_tmp);

			std::copy(X_N2_tmp.begin(), X_N2_tmp.end(), X_N2.begin() + f * this->N_mod);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::_modulate(const mipp::vector<B>& X_N1, mipp::vector<R>& X_N2)
{
	assert((int)X_N1.size() == N          );
	assert((int)X_N2.size() == this->N_mod);

	// mapper
	mipp::vector<S_in> mapped_frame(Nb_sy);

	for(int i=0; i < Nb_sy; i++)
		mapped_frame[i] = cpm.binary_to_transition[cpe.merge_bits(X_N1.data()+i*cpm.Nb_b_per_s, cpm.Nb_b_per_s, true)];

	// continuous phase encoder
	mipp::vector<S_in> encoded_frame(Nb_sy_tl);
	cpe.encode(mapped_frame, encoded_frame);

	// memoryless modulation (attributing complex waveforms to symbols)
	const auto off_X_r  = 0;
	const auto off_X_i  = (int)X_N2.size() / 2;
	const auto off_BB_r = 0;
	const auto off_BB_i = (int)baseband.size() / 2;

	for (auto i = 0; i < Nb_sy_tl; i++)
		for (auto s = 0; s < cpm.Sf; s++)
		{
			X_N2[off_X_r + i * cpm.Sf + s] = baseband[off_BB_r + encoded_frame[i] * cpm.Sf + s];
			X_N2[off_X_i + i * cpm.Sf + s] = baseband[off_BB_i + encoded_frame[i] * cpm.Sf + s];
		}

//	std::cout << "mapped_frame : " << std::endl;
//
//	for (int i = 0; i < Nb_sy; i++)
//	{
//		std::cout << mapped_frame[i] << " ";
//	}
//	std::cout <<  std::endl;
//	std::cout <<  std::endl;
//
//	std::cout << "encoded_frame : " << std::endl;
//
//	for (int i = 0; i < Nb_sy_tl; i++)
//	{
//		std::cout << encoded_frame[i] << " ";
//	}
//	std::cout <<  std::endl;
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_mod*this->n_frames);
	assert((int)Y_N2.size() == this->N_fil*this->n_frames);

	if (this->n_frames == 1)
	{
		_filter(Y_N1, Y_N2);
	}
	else // more than 1 frame
	{
		mipp::vector<R> Y_N1_tmp(this->N_mod);
		mipp::vector<R> Y_N2_tmp(this->N_fil);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_mod,
			          Y_N1.begin() + (f +1) * this->N_mod,
			          Y_N1_tmp.begin());

			_filter(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * this->N_fil);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::_filter(const mipp::vector<R>& Y_N1, mipp::vector<R>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_mod);
	assert((int)Y_N2.size() == this->N_fil);

	const R* Y_real = Y_N1.data();
	const R* Y_imag = Y_N1.data() + (Y_N1.size() >> 1);
	const R* p_real = projection.data();
	const R* p_imag = projection.data() + (projection.size() >> 1);


	for(auto i = 0; i < Nb_sy_tl; i++)
	{
		for (auto wa = 0; wa < cpm.Nb_wa; wa++)
		{
			R sum_r = 0;
			for (auto s = 0; s < cpm.Sf; s++)
			{
				sum_r += Y_real[i * cpm.Sf + s] * p_real[s * cpm.max_wa_id + cpm.allowed_wave_forms[wa]]
						-Y_imag[i * cpm.Sf + s] * p_imag[s * cpm.max_wa_id + cpm.allowed_wave_forms[wa]];
			}

			Y_N2[i * cpm.max_wa_id + cpm.allowed_wave_forms[wa]] = sum_r;
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2)
{
	assert((int)Y_N1.size() == this->N_fil*this->n_frames);
	assert((int)Y_N2.size() ==           N*this->n_frames);

	if (this->n_frames == 1)
	{
		bcjr.decode(Y_N1, Y_N2);
	}
	else // more than 1 frame
	{
		mipp::vector<Q> Y_N1_tmp(this->N_fil);
		mipp::vector<Q> Y_N2_tmp(N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_fil,
			          Y_N1.begin() + (f +1) * this->N_fil,
			          Y_N1_tmp.begin());

			bcjr.decode(Y_N1_tmp, Y_N2_tmp);

			std::copy(Y_N2_tmp.begin(), Y_N2_tmp.end(), Y_N2.begin() + f * N);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3)
{
	assert((int)Y_N1.size() == this->N_fil);
	assert((int)Y_N2.size() == N);
	assert((int)Y_N3.size() == N);

	if (this->n_frames == 1)
	{
		bcjr.decode(Y_N1, Y_N2, Y_N3);
	}
	else
	{
		mipp::vector<Q> Y_N1_tmp(this->N_fil);
		mipp::vector<Q> Y_N2_tmp(N);
		mipp::vector<Q> Y_N3_tmp(N);

		for (auto f = 0; f < this->n_frames; f++)
		{
			std::copy(Y_N1.begin() + (f +0) * this->N_fil,
			          Y_N1.begin() + (f +1) * this->N_fil,
			          Y_N1_tmp.begin());

			std::copy(Y_N2.begin() + (f +0) * N,
					  Y_N2.begin() + (f +1) * N,
					  Y_N2_tmp.begin());

			bcjr.decode(Y_N1_tmp, Y_N2_tmp, Y_N3_tmp); // remove tail symb automatically because Y_N3_tmp.size = Y_N1_tmp.size + cpm.tl

			std::copy(Y_N3_tmp.begin(), Y_N3_tmp.end(), Y_N3.begin() + f * N);
		}
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::generate_baseband()
{
	assert((int)baseband.size() == (cpm.max_wa_id*cpm.Sf*2));
	mipp::vector<R> phase_response(cpm.L*cpm.Sf);

	// calculate the different phase responses
	for(int s = 0 ; s < cpm.L*cpm.Sf ; s++)
	{
		phase_response[s] = calculate_phase_response(s*T_samp);
	}


	int p_mask = (1 << cpm.Nb_bits_p ) - 1;
	int L_mask = (1 << cpm.Nb_b_per_s) - 1;

	for(int wa = 0 ; wa < cpm.Nb_wa ; wa++)
	{
		int allowed_wa       = cpm.allowed_wave_forms[wa];
		R tilted_phase_part1 = 2*M_PI*cpm_h*(allowed_wa & p_mask);

		mipp::vector<R> tilted_phase_part2(cpm.Sf, 0);
		mipp::vector<R> tilted_phase_part3(cpm.Sf, 0);

		for(int l = 0 ; l < cpm.L ; l++)
		{
			int U_n = (allowed_wa >> ((cpm.L-l-1)*cpm.Nb_b_per_s+cpm.Nb_bits_p)) & L_mask;

			for(int s = 0 ; s < cpm.Sf ; s++)
			{
				tilted_phase_part2[s] += phase_response[l*cpm.Sf+s] * U_n;
				tilted_phase_part3[s] += phase_response[l*cpm.Sf+s];
			}
		}

		for(int s = 0 ; s < cpm.Sf ; s++)
		{
			R tilted_phase = tilted_phase_part1
							+ M_PI*cpm_h*(4*tilted_phase_part2[s]
										  + (cpm.Mo-1)*(s*T_samp
											  	  	    + (cpm.L-1)
													    - 2*tilted_phase_part3[s]
													   )
										 );

			baseband[allowed_wa*cpm.Sf+s                     ] = cos(tilted_phase);
			baseband[allowed_wa*cpm.Sf+s + baseband.size()/2] = sin(tilted_phase);
		}
	}
}

template<typename R>
class gmsk
{
public:
	gmsk(const R b, const R offset) : B(b), off(offset){ }

	R operator()(const R& t) const
	{
		R x = t + off;

		R factor = (R)2.0*M_PI*B/sqrt((R)2.0*log((R)2.0));
		R minus  = (x-(R)0.5)*factor;
		R plus   = (x+(R)0.5)*factor;

		return (erf(plus)-erf(minus))/(R)4.0;
	}

private:
   const R B;
   const R off;
};

template <typename B, typename R, typename Q, proto_max<Q> MAX>
R Modulator_CPM<B,R,Q,MAX>
::calculate_phase_response(const R t_stamp)
{
	if(cpm.wave_shape == "GMSK")
	{
		gmsk<R> g((R)0.3, -(R)cpm.L/(R)2.0);
		return integral(g, (R)0.0, t_stamp, t_stamp/(R)1e-4);
	}
	else if(cpm.wave_shape == "RCOS")
	{
		return t_stamp/((R)2.0*cpm.L) - sin((R)2.0*M_PI*t_stamp/(R)cpm.L)/(R)4.0/M_PI;
	}
	else if(cpm.wave_shape == "REC")
	{
		return t_stamp/((R)2.0*cpm.L);
	}
	else
	{
		std::cerr<<bold_red("(EE) Unknown CPM wave shape")<<std::endl;
		exit(-1);
	}
}

template <typename B, typename R, typename Q, proto_max<Q> MAX>
void Modulator_CPM<B,R,Q,MAX>
::generate_projection()
{
	assert(projection.size() == baseband.size());

	//R factor = (R)2.0/(R)cpm.Sf;

	//R noise_var = 2*sigma*sigma/cpm.Sf;

	R factor = 1/sigma/sigma;

	if(cpm.filters_type == "TOTAL")
	{
		complex_transpose(cpm.max_wa_id, cpm.Sf, baseband, projection);

		for(size_t i = 0; i < projection.size() ; i++)
		{
			projection[i] *= factor;
		}
	}
//	else if(filters_type == "ORTHO_NORM")
//	{
//	}
	else
	{
		std::cerr<<bold_red("(EE) Unknown CPM filter bank type")<<std::endl;
		exit(-1);
	}
}
