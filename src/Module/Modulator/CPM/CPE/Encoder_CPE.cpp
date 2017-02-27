#include "Encoder_CPE.hpp"

using namespace aff3ct::module;

template<typename SIN, typename SOUT>
Encoder_CPE<SIN, SOUT>
::Encoder_CPE(const int N, const CPM_parameters<SIN,SOUT>& cpm, const int n_frames,
                  const std::string name)
: N(N), cpm(cpm), n_frames(n_frames)
{
	assert((int)sizeof(SIN )*256 >= cpm.m_order); // because U_n can have Mo values
	assert((int)sizeof(SIN )*256 >= cpm.p      ); // because V_n can have p values
	assert((int)sizeof(SOUT)*256 >= cpm.n_wa   ); // because X_n can have Nb_xa values
}

template<typename SIN, typename SOUT>
int Encoder_CPE<SIN, SOUT>
::get_n_ff() const
{
	return cpm.L;
}

template<typename SIN, typename SOUT>
int Encoder_CPE<SIN, SOUT>
::tail_length() const
{
	return cpm.tl;
}

template<typename SIN, typename SOUT>
void Encoder_CPE<SIN, SOUT>
::encode(const mipp::vector<SIN>& U_N, mipp::vector<SOUT>& X_N)
{
	assert((int)U_N.size() == (N * n_frames));
	assert((int)X_N.size() == ((N + cpm.tl) * n_frames));

	for (auto f = 0; f <n_frames; f++)
		frame_encode(U_N.data() + f*N, X_N.data() + f*(N+cpm.tl));
}

template<typename SIN, typename SOUT>
void Encoder_CPE<SIN, SOUT>
::frame_encode(const SIN* U_N, SOUT* X_N)
{
	auto state = 0; // initial (and final) state 0
	auto i = 0;

	// standard frame encoding process
	for (; i < N; i++)
		X_N[i] = inner_encode((int)U_N[i], state); // encoding block

	// tail symbols for BCJR initialization conditions (value of state have to be 0)
	for (; i < N + tail_length(); i++)
		X_N[i] = inner_encode(tail_symb(state), state); // encoding block

	assert(state == 0);
}

template<typename SIN, typename SOUT>
void Encoder_CPE<SIN, SOUT>
::generate_trellis(mipp::vector<int>& trellis_next_state, mipp::vector<SOUT>& trellis_related_wave_form)
{
	assert((int)trellis_next_state       .size() == (cpm.max_st_id*cpm.m_order));
	assert((int)trellis_related_wave_form.size() == (cpm.max_st_id*cpm.m_order));

	int state;
	SOUT wa_symb;

	for (auto st = 0; st < cpm.n_st; ++st)
		for (auto tr = 0; tr < cpm.m_order; ++tr)
		{
			state = cpm.allowed_states[st];
			wa_symb = inner_encode(tr, state); // warning state change here!

			trellis_next_state       [cpm.allowed_states[st]*cpm.m_order + tr] = state;
			trellis_related_wave_form[cpm.allowed_states[st]*cpm.m_order + tr] = wa_symb;
		}
}

template<typename SIN, typename SOUT>
void Encoder_CPE<SIN, SOUT>
::generate_anti_trellis(mipp::vector<int>& anti_trellis_original_state,
                        mipp::vector<SIN>& anti_trellis_input_transition)
{
	assert((int)anti_trellis_original_state  .size() == (cpm.max_st_id*cpm.m_order));
	assert((int)anti_trellis_input_transition.size() == (cpm.max_st_id*cpm.m_order));

	mipp::vector<int> counter(cpm.max_st_id, 0);

	int next_state;

	for (auto st = 0; st < cpm.n_st; st++)
	{
		for (auto tr = 0; tr < cpm.m_order; tr++)
		{
			next_state = cpm.trellis_next_state[cpm.allowed_states[st]*cpm.m_order + tr];
			assert(next_state < cpm.max_st_id);
			assert(counter[next_state] < cpm.m_order);
			anti_trellis_original_state  [next_state*cpm.m_order + counter[next_state]] = cpm.allowed_states[st];
			anti_trellis_input_transition[next_state*cpm.m_order + counter[next_state]] = tr;

			counter[next_state]++;
		}
	}
}

template<typename SIN, typename SOUT>
SIN Encoder_CPE<SIN, SOUT>
::merge_bits(const SIN* in_bit, const int number_of_bits, const bool msb_to_lsb)
{
	SIN res = 0;

	for (auto i = 0; i < number_of_bits; i++)
		res |= (*(in_bit +i)) << ((msb_to_lsb) ? (number_of_bits-1-i) : (i));

	return res;
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Encoder_CPE<B_8,  B_8 >;
template class Encoder_CPE<B_16, B_16>;
template class Encoder_CPE<B_32, B_32>;
template class Encoder_CPE<B_64, B_64>;
#else
template class Encoder_CPE<B, B>;
#endif
// ==================================================================================== explicit template instantiation
