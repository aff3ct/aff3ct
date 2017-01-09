/*
 * Encoder_CPE_Rimoldi.hxx
 *
 *  Created on: 19 d�c. 2016
 *      Author: bpetit
 */

#include <cassert>
#include <vector>
#include <cmath>

#include "Encoder_CPE_Rimoldi.hpp"

template <typename S_in, typename S_out>
Encoder_CPE_Rimoldi<S_in, S_out>
::Encoder_CPE_Rimoldi(const int N, const CPM_parameters<S_in,S_out>& cpm, const int n_frames,
					  const std::string name)
: Encoder_CPE<S_in,S_out>(N, cpm, n_frames, name)//, cpm(cpm)
{
}

template<typename S_in, typename S_out>
S_out Encoder_CPE_Rimoldi<S_in, S_out>
::inner_encode(const S_in new_symbol, int &state)
{
	//compute new symbol value
	state |= ((int)new_symbol << (this->cpm.Nb_bits_p+this->cpm.Nb_b_per_s*(this->cpm.L-1)));
	S_out wa_symb = (S_out)state;

	// extract V_n
	S_in V_n 	 = state & ((1 << this->cpm.Nb_bits_p)-1); // because V_n can have p values

	// extract U_(n-L-1)
	S_in U_n_L_1 = (state >> this->cpm.Nb_bits_p) & ((1 << this->cpm.Nb_b_per_s)-1);

	// compute V_(n+1)
	S_in V_n_1   = ((U_n_L_1 + V_n)%this->cpm.p);// because V_n can have p values

	// remove V_n and U_(n-L-1)
	state      >>= this->cpm.Nb_bits_p + this->cpm.Nb_b_per_s;

	// add V_(n+1)
	state      <<= this->cpm.Nb_bits_p;
	state       |= V_n_1;

	return wa_symb;
}

template<typename S_in, typename S_out>
S_in Encoder_CPE_Rimoldi<S_in, S_out>
::tail_symb(const int &state)
{
	assert(state < this->cpm.max_st_id);

	// extract V_n
	int val = state & ((1 << this->cpm.Nb_bits_p)-1);

	// sum V_n to the U_(n-L-1-i)
	for(auto i = 0; i < this->cpm.L-1 ; i++)
	{
		val += (state >> (i*this->cpm.Nb_b_per_s+this->cpm.Nb_bits_p)) & ((1 << this->cpm.Nb_b_per_s)-1);
	}

	return (S_in)(this->cpm.p - (val)%this->cpm.p)%this->cpm.p;
}

template<typename S_in, typename S_out>
void Encoder_CPE_Rimoldi<S_in, S_out>
::generate_mapper(mipp::vector<S_in>& transition_to_binary, mipp::vector<S_in>& binary_to_transition, const std::string& mapping)
{
	assert((int)transition_to_binary.size() == (this->cpm.Mo*this->cpm.Nb_b_per_s));
	assert((int)binary_to_transition.size() == (this->cpm.Mo                     ));

	if(mapping == "NATURAL")
	{
		for (int tr = 0; tr < this->cpm.Mo; tr++)
			for (int b = 0; b < this->cpm.Nb_b_per_s; b++)
				transition_to_binary[tr * this->cpm.Nb_b_per_s + this->cpm.Nb_b_per_s -1 -b] = (tr & (1 << b)) ? 1 : 0 ;

		for (int tr = 0; tr < this->cpm.Mo; tr++)
			binary_to_transition[tr] = tr;
	}
	else if(mapping == "GRAY")
	{
		for (int tr = 0; tr < this->cpm.Mo; tr++)
		{
			int gray_val = (tr >> 1) ^ tr;
			for (int b = 0; b < this->cpm.Nb_b_per_s; b++)
				transition_to_binary[tr * this->cpm.Nb_b_per_s + this->cpm.Nb_b_per_s -1 -b] = (gray_val & (1 << b)) ? 1 : 0;
		}

		for (int tr = 0; tr < this->cpm.Mo; tr++)
			binary_to_transition[this->merge_bits(transition_to_binary.data() + tr * this->cpm.Nb_b_per_s,  this->cpm.Nb_b_per_s, true)] = tr;
	}
	else
	{
		std::cerr<<bold_red("(EE) Unknown BCJR mapping scheme!")<<std::endl;
		exit(-1);
	}

}

template<typename S_in, typename S_out>
void Encoder_CPE_Rimoldi<S_in, S_out>
::generate_allowed_states(mipp::vector<int>& allowed_states)
{
	assert((int)allowed_states.size() == this->cpm.Nb_st);

	int state_index = 0;
	int state_pos = 0;

	int p_mask = (1 << this->cpm.Nb_bits_p) - 1;

	while(state_index < this->cpm.max_st_id && state_pos < this->cpm.Nb_st)
	{
		allowed_states[state_pos] = state_index;

		state_pos ++;
		state_index ++;

		if((state_index & p_mask) == this->cpm.p)
		{
			state_index |= p_mask;
			state_index++;
		}
	}
}

template<typename S_in, typename S_out>
void Encoder_CPE_Rimoldi<S_in, S_out>
::generate_allowed_wave_forms(mipp::vector<S_out>& allowed_wave_forms)
{
	assert((int)allowed_wave_forms.size() == this->cpm.Nb_wa);

	int wa_index = 0;
	int wa_pos = 0;

	int p_mask = (1 << this->cpm.Nb_bits_p) - 1;

	while(wa_index < this->cpm.max_wa_id && wa_pos < this->cpm.Nb_wa)
	{
		allowed_wave_forms[wa_pos] = wa_index;

		wa_pos ++;
		wa_index ++;

		if((wa_index & p_mask) == this->cpm.p)
		{
			wa_index |= p_mask;
			wa_index++;
		}
	}
}

// ================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_CPE_Rimoldi<S_8,  S_8 >;
template class Encoder_CPE_Rimoldi<S_16, S_16>;
template class Encoder_CPE_Rimoldi<S_32, S_32>;
template class Encoder_CPE_Rimoldi<S_64, S_64>;
#else
template class Encoder_CPE_Rimoldi<S_in, S_out>;
#endif
// =================================================== explicit template instantiation
