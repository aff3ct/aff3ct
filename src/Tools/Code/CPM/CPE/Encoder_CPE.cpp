#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Code/CPM/CPE/Encoder_CPE.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

template<typename SIN, typename SOUT>
Encoder_CPE<SIN, SOUT>
::Encoder_CPE(const int N, const CPM_parameters<SIN,SOUT>& cpm)
: N(N), cpm(cpm)
{
	// because U_n can have Mo values
	if ((int)sizeof(SIN) * 256 < cpm.m_order)
	{
		std::stringstream message;
		message << "'cpm.m_order' has to be equal or smaller than 'sizeof(SIN)' * 256 ('cpm.m_order' = "
		        << cpm.m_order << ", 'sizeof(SIN)' = " << sizeof(SIN) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// because V_n can have p values
	if ((int)sizeof(SIN) * 256 < cpm.p)
	{
		std::stringstream message;
		message << "'cpm.p' has to be equal or smaller than 'sizeof(SIN)' * 256 ('cpm.p' = " << cpm.p
		        << ", 'sizeof(SIN)' = " << sizeof(SIN) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// because X_n can have Nb_xa values
	if ((int)sizeof(SOUT) * 256 < cpm.n_wa)
	{
		std::stringstream message;
		message << "'cpm.n_wa' has to be equal or smaller than 'sizeof(SOUT)' * 256 ('cpm.n_wa' = " << cpm.n_wa
		        << ", 'sizeof(SOUT)' = " << sizeof(SOUT) << ").";
		throw invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}
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
::encode(const std::vector<SIN>& U_N, std::vector<SOUT>& X_N)
{
	if ((int)U_N.size() != N)
	{
		std::stringstream message;
		message << "'U_N.size()' has to be equal to 'N' ('U_N.size()' = " << U_N.size() << ", 'N' = " << N << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)X_N.size() != (N + cpm.tl))
	{
		std::stringstream message;
		message << "'X_N.size()' has to be equal to 'N' + 'cpm.tl' ('X_N.size()' = " << X_N.size()
		        << ", 'N' = " << N << ", 'cpm.tl' = " << cpm.tl << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto state = 0; // initial (and final) state 0
	auto i = 0;

	// standard frame encoding process
	for (; i < N; i++)
		X_N[i] = inner_encode((int)U_N[i], state); // encoding block

	// tail symbols for BCJR initialization conditions (value of state have to be 0)
	for (; i < N + tail_length(); i++)
		X_N[i] = inner_encode(tail_symb(state), state); // encoding block

	if (state != 0)
	{
		std::stringstream message;
		message << "'state' should be equal to 0 ('state' = " << state << ").";
		throw runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

template<typename SIN, typename SOUT>
void Encoder_CPE<SIN, SOUT>
::generate_trellis(std::vector<int>& trellis_next_state, std::vector<SOUT>& trellis_related_wave_form)
{
	if ((int)trellis_next_state.size() != cpm.max_st_id * cpm.m_order)
	{
		std::stringstream message;
		message << "'trellis_next_state.size()' has to be equal to 'cpm.max_st_id' * 'cpm.m_order' "
		        << "('trellis_next_state.size()' = " << trellis_next_state.size()
		        << ", 'cpm.max_st_id' = " << cpm.max_st_id
		        << ", 'cpm.m_order' = " << cpm.m_order << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)trellis_related_wave_form.size() != cpm.max_st_id * cpm.m_order)
	{
		std::stringstream message;
		message << "'trellis_related_wave_form.size()' has to be equal to 'cpm.max_st_id' * 'cpm.m_order' "
		        << " ('trellis_related_wave_form.size()' = " << trellis_related_wave_form.size()
		        << ", 'cpm.max_st_id' = " << cpm.max_st_id
		        << ", 'cpm.m_order' = " << cpm.m_order << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

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
::generate_anti_trellis(std::vector<int>& anti_trellis_original_state,
                        std::vector<SIN>& anti_trellis_input_transition)
{
	if ((int)anti_trellis_original_state.size() != cpm.max_st_id * cpm.m_order)
	{
		std::stringstream message;
		message << "'anti_trellis_original_state.size()' has to be equal to 'cpm.max_st_id' * 'cpm.m_order' "
		        << "('anti_trellis_original_state.size()' = " << anti_trellis_original_state.size()
		        << ", 'cpm.max_st_id' = " << cpm.max_st_id
		        << ", 'cpm.m_order' = " << cpm.m_order << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((int)anti_trellis_input_transition.size() != cpm.max_st_id * cpm.m_order)
	{
		std::stringstream message;
		message << "'anti_trellis_input_transition.size()' has to be equal to 'cpm.max_st_id' * 'cpm.m_order' "
		        << "('anti_trellis_input_transition.size()' = " << anti_trellis_input_transition.size()
		        << ", 'cpm.max_st_id' = " << cpm.max_st_id
		        << ", 'cpm.m_order' = " << cpm.m_order << ").";
		throw length_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::vector<int> counter(cpm.max_st_id, 0);

	int next_state;

	for (auto st = 0; st < cpm.n_st; st++)
	{
		for (auto tr = 0; tr < cpm.m_order; tr++)
		{
			next_state = cpm.trellis_next_state[cpm.allowed_states[st]*cpm.m_order + tr];

			if (next_state >= cpm.max_st_id)
			{
				std::stringstream message;
				message << "'next_state' should be smaller than 'cpm.max_st_id' ('next_state' = " << next_state
				        << ", 'cpm.max_st_id' = " << cpm.max_st_id << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

			if (counter[next_state] >= cpm.m_order)
			{
				std::stringstream message;
				message << "'counter[next_state]' should be smaller than 'cpm.m_order' "
				        << "('next_state' = " << next_state << ", 'counter[next_state]' = " << counter[next_state]
				        << ", 'cpm.m_order' = " << cpm.m_order << ").";
				throw runtime_error(__FILE__, __LINE__, __func__, message.str());
			}

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
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::tools::Encoder_CPE<B_8,  B_8 >;
template class aff3ct::tools::Encoder_CPE<B_16, B_16>;
template class aff3ct::tools::Encoder_CPE<B_32, B_32>;
template class aff3ct::tools::Encoder_CPE<B_64, B_64>;
#else
template class aff3ct::tools::Encoder_CPE<B, B>;
#endif
// ==================================================================================== explicit template instantiation
