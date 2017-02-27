#include <cassert>
#include <vector>
#include <cmath>

#include "Tools/Display/bash_tools.h"

#include "Encoder_CPE_Rimoldi.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;


template <typename SIN, typename SOUT>
Encoder_CPE_Rimoldi<SIN, SOUT>
::Encoder_CPE_Rimoldi(const int N,
                      const CPM_parameters<SIN,SOUT>& cpm,
                      const int n_frames,
                      const std::string name)
: Encoder_CPE<SIN,SOUT>(N, cpm, n_frames, name)
{
}

template<typename SIN, typename SOUT>
SOUT Encoder_CPE_Rimoldi<SIN, SOUT>
::inner_encode(const SIN new_symbol, int &state)
{
	//compute new symbol value
	state |= ((int)new_symbol << (this->cpm.n_bits_p + this->cpm.n_b_per_s * (this->cpm.L -1)));
	SOUT wa_symb = (SOUT)state;

	// extract V_n
	SIN V_n  = state & ((1 << this->cpm.n_bits_p) -1); // because V_n can have p values

	// extract U_(n-L-1)
	SIN U_n_L_1 = (state >> this->cpm.n_bits_p) & ((1 << this->cpm.n_b_per_s) -1);

	// compute V_(n+1)
	SIN V_n_1 = ((U_n_L_1 + V_n)%this->cpm.p);// because V_n can have p values

	// remove V_n and U_(n-L-1)
	state >>= this->cpm.n_bits_p + this->cpm.n_b_per_s;

	// add V_(n+1)
	state <<= this->cpm.n_bits_p;
	state  |= V_n_1;

	return wa_symb;
}

template<typename SIN, typename SOUT>
SIN Encoder_CPE_Rimoldi<SIN, SOUT>
::tail_symb(const int &state)
{
	assert(state < this->cpm.max_st_id);

	// extract V_n
	int val = state & ((1 << this->cpm.n_bits_p)-1);

	// sum V_n to the U_(n-L-1-i)
	for (auto i = 0; i < this->cpm.L -1; i++)
		val += (state >> (i*this->cpm.n_b_per_s+this->cpm.n_bits_p)) & ((1 << this->cpm.n_b_per_s)-1);

	return (SIN)(this->cpm.p - (val)%this->cpm.p) % this->cpm.p;
}

template<typename SIN, typename SOUT>
void Encoder_CPE_Rimoldi<SIN, SOUT>
::generate_mapper(mipp::vector<SIN>& transition_to_binary,
                  mipp::vector<SIN>& binary_to_transition,
                  const std::string& mapping)
{
	assert((int)transition_to_binary.size() == (this->cpm.m_order*this->cpm.n_b_per_s));
	assert((int)binary_to_transition.size() == (this->cpm.m_order                    ));

	if (mapping == "NATURAL")
	{
		for (auto tr = 0; tr < this->cpm.m_order; tr++)
			for (auto b = 0; b < this->cpm.n_b_per_s; b++)
				transition_to_binary[tr * this->cpm.n_b_per_s + this->cpm.n_b_per_s -1 -b] = (tr & (1 << b)) ? 1 : 0;

		for (auto tr = 0; tr < this->cpm.m_order; tr++)
			binary_to_transition[tr] = tr;
	}
	else if (mapping == "GRAY")
	{
		for (auto tr = 0; tr < this->cpm.m_order; tr++)
		{
			int gray_val = (tr >> 1) ^ tr;
			for (auto b = 0; b < this->cpm.n_b_per_s; b++)
				transition_to_binary[tr * this->cpm.n_b_per_s + this->cpm.n_b_per_s -1 -b] = (gray_val & (1 << b)) ? 1 : 0;
		}

		for (auto tr = 0; tr < this->cpm.m_order; tr++)
			binary_to_transition[this->merge_bits(transition_to_binary.data() + tr * this->cpm.n_b_per_s,
			                                      this->cpm.n_b_per_s, true)] = tr;
	}
	else
	{
		std::cerr << bold_red("(EE) Unknown BCJR mapping scheme!") << std::endl;
		exit(-1);
	}

}

template<typename SIN, typename SOUT>
void Encoder_CPE_Rimoldi<SIN, SOUT>
::generate_allowed_states(mipp::vector<int>& allowed_states)
{
	assert((int)allowed_states.size() == this->cpm.n_st);

	int state_index = 0;
	int state_pos   = 0;

	int p_mask = (1 << this->cpm.n_bits_p) - 1;

	while (state_index < this->cpm.max_st_id && state_pos < this->cpm.n_st)
	{
		allowed_states[state_pos] = state_index;

		state_pos++;
		state_index++;

		if ((state_index & p_mask) == this->cpm.p)
		{
			state_index |= p_mask;
			state_index++;
		}
	}
}

template<typename SIN, typename SOUT>
void Encoder_CPE_Rimoldi<SIN, SOUT>
::generate_allowed_wave_forms(mipp::vector<SOUT>& allowed_wave_forms)
{
	assert((int)allowed_wave_forms.size() == this->cpm.n_wa);

	int wa_index = 0;
	int wa_pos = 0;

	int p_mask = (1 << this->cpm.n_bits_p) - 1;

	while (wa_index < this->cpm.max_wa_id && wa_pos < this->cpm.n_wa)
	{
		allowed_wave_forms[wa_pos] = wa_index;

		wa_pos++;
		wa_index++;

		if ((wa_index & p_mask) == this->cpm.p)
		{
			wa_index |= p_mask;
			wa_index++;
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::module::Encoder_CPE_Rimoldi<B_8,  B_8 >;
template class aff3ct::module::Encoder_CPE_Rimoldi<B_16, B_16>;
template class aff3ct::module::Encoder_CPE_Rimoldi<B_32, B_32>;
template class aff3ct::module::Encoder_CPE_Rimoldi<B_64, B_64>;
#else
template class aff3ct::module::Encoder_CPE_Rimoldi<B, B>;
#endif
// ==================================================================================== explicit template instantiation
