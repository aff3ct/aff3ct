/*
 * Encoder_CPE.hxx
 *
 *  Created on: 19 d�c. 2016
 *      Author: ohartmann
 */

#include "Encoder_CPE.hpp"

template<typename S_in, typename S_out>
Encoder_CPE<S_in, S_out>
::Encoder_CPE(const int N, const CPM_parameters<S_in,S_out>& cpm, const int n_frames,
                  const std::string name)
: Encoder<S_in>(N, 2*N + cpm.tl, n_frames, name), N(N), cpm(cpm), n_frames(n_frames)
{
	assert((int)sizeof(S_in )*256 >= cpm.Mo);  // because U_n can have Mo values
	assert((int)sizeof(S_in )*256 >= cpm.p);   // because V_n can have p values
	assert((int)sizeof(S_out)*256 >= cpm.Nb_wa);// because X_n can have Nb_xa values
}

template<typename S_in, typename S_out>
int Encoder_CPE<S_in, S_out>
::get_n_ff() const
{
	return cpm.L;
}

template<typename S_in, typename S_out>
int Encoder_CPE<S_in, S_out>
::tail_length() const
{
	return cpm.tl;
}

template<typename S_in, typename S_out>
void Encoder_CPE<S_in, S_out>
::encode(const mipp::vector<S_in>& U_N, mipp::vector<S_out>& X_N)
{
	assert((int)U_N.size() == (N * n_frames));
	assert((int)X_N.size() == ((N+cpm.tl) * n_frames));

	for (auto f = 0; f <n_frames; f++)
		frame_encode(U_N.data() + f*N, X_N.data() + f*(N+cpm.tl));
}

template<typename S_in, typename S_out>
void Encoder_CPE<S_in, S_out>
::frame_encode(const S_in* U_N, S_out* X_N)
{
	auto state = 0; // initial (and final) state 0
	auto i = 0;

	// standard frame encoding process
	for (; i <N; i++)
	{
		X_N[i] = inner_encode((int)U_N[i], state); // encoding block
	}

	// tail symbols for BCJR initialization conditions (value of state have to be 0)
	for (; i <N+tail_length(); i++)
	{
		X_N[i] = inner_encode(tail_symb(state), state); // encoding block
	}

	assert(state == 0);
}

template<typename S_in, typename S_out>
void Encoder_CPE<S_in, S_out>
::generate_trellis(mipp::vector<int>& trellis_next_state, mipp::vector<S_out>& trellis_related_wave_form)
{
	assert((int)trellis_next_state       .size() == (cpm.max_st_id*cpm.Mo));
	assert((int)trellis_related_wave_form.size() == (cpm.max_st_id*cpm.Mo));

	int state;
	S_out wa_symb;

	for (int st = 0; st <cpm.Nb_st; ++st)
		for (S_in tr = 0; tr <cpm.Mo; ++tr)
		{
			state = cpm.allowed_states[st];
			wa_symb = inner_encode(tr, state); // warning state change here!

			trellis_next_state       [cpm.allowed_states[st]*cpm.Mo + tr] = state;
			trellis_related_wave_form[cpm.allowed_states[st]*cpm.Mo + tr] = wa_symb;
		}
}

template<typename S_in, typename S_out>
void Encoder_CPE<S_in, S_out>
::generate_anti_trellis(mipp::vector<int>& anti_trellis_original_state, mipp::vector<S_in>& anti_trellis_input_transition)
{
	assert((int)anti_trellis_original_state  .size() == (cpm.max_st_id*cpm.Mo));
	assert((int)anti_trellis_input_transition.size() == (cpm.max_st_id*cpm.Mo));

	mipp::vector<int> counter(cpm.max_st_id, 0);

	int next_state;

	for(int st = 0; st <cpm.Nb_st ; st++)
	{
		for(S_in tr = 0 ; tr <cpm.Mo ; tr++)
		{
			next_state = cpm.trellis_next_state[cpm.allowed_states[st]*cpm.Mo + tr];
			assert(next_state <cpm.max_st_id);
			assert(counter[next_state] <cpm.Mo);
			anti_trellis_original_state  [next_state*cpm.Mo + counter[next_state]] = cpm.allowed_states[st];
			anti_trellis_input_transition[next_state*cpm.Mo + counter[next_state]] = tr;

			counter[next_state]++;
		}
	}
}

template<typename S_in, typename S_out>
S_in Encoder_CPE<S_in, S_out>
::merge_bits(const S_in* in_bit, const int number_of_bits, const bool msb_to_lsb)
{
	S_in res = 0;

	for(int i = 0; i <number_of_bits ; i++)
	{
		res |= (*(in_bit+i)) <<((msb_to_lsb)?(number_of_bits-1-i):(i));
	}

	return res;
}
// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_CPE<S_8,  S_8 >;
template class Encoder_CPE<S_16, S_16>;
template class Encoder_CPE<S_32, S_32>;
template class Encoder_CPE<S_64, S_64>;
#else
template class Encoder_CPE<S_in, S_out>;
#endif
// ==================================================================================== explicit template instantiation
