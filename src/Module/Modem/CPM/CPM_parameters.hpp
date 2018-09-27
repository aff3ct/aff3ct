#ifndef CPM_PARAMETERS_HPP_
#define CPM_PARAMETERS_HPP_

#include <fstream>
#include <string>
#include <vector>
#include <cmath>

namespace aff3ct
{
namespace module
{
template <typename SIN = int, typename SOUT = int>
class CPM_parameters
{
public :
	CPM_parameters(const int L,
	               const int k,
	               const int p,
	               const int n_b_per_s,
	               const int s_factor,
	               const std::string& filters_type,
	               const std::string& wave_shape)
	: L           (L                                   ),
	  k           (k                                   ),
	  p           (p                                   ),
	  n_b_per_s   (n_b_per_s                           ),
	  s_factor    (s_factor                            ),
	  filters_type(filters_type                        ),
	  wave_shape  (wave_shape                          ),
	  m_order     ((int)1 << n_b_per_s                 ),
	  n_bits_p    ((int)std::ceil(std::log2((double)p))),
	  tl          ((int)(std::ceil((float)(p - 1) / (float)(m_order - 1))) + L - 1),

	  // TODO: warning: from here parameters are working for Rimoldi decomposition only!
	  n_wa        ((int)(p * std::pow(m_order, L))     ),
	  n_bits_wa   ((int)std::ceil(std::log2(n_wa))     ),
	  max_wa_id   (((int)1 << n_bits_wa)               ),

	  n_st        ((int)(p * std::pow(m_order, L-1))   ),
	  n_bits_st   ((int)std::ceil(std::log2(n_st))     ),
	  max_st_id   (((int)1 << n_bits_st)               ),

	  transition_to_binary         (n_b_per_s     * m_order, -1),
	  binary_to_transition         (                m_order, -1),
	  allowed_states               (n_st                   , -1),
	  allowed_wave_forms           (n_wa                   , -1),
	  trellis_next_state           (max_st_id     * m_order, -1),
	  trellis_related_wave_form    (max_st_id     * m_order, -1),
	  anti_trellis_original_state  (max_st_id     * m_order, -1),
	  anti_trellis_input_transition(max_st_id     * m_order, -1)
	{
	}

	virtual ~CPM_parameters() = default;

	int L;                          // CPM pulse width or CPm memory
	int k;                          // modulation index numerator
	int p;                          // modulation index denumerator
	int n_b_per_s;                  // number of bits per symbol
	int s_factor;                   // sampling factor

	const std::string filters_type; // Choose receiver filters bank: {TOTAL}
	const std::string wave_shape;   // Choose wave shape : {GMSK, REC, RCOS}

	int m_order;                    // modulation order of the cpm
	int n_bits_p;                   // number of bits used to code p
	int tl;                         // tail length for the modulation

	int n_wa;                       // number of wave forms used by the CPM
	int n_bits_wa;                  // number of bits used to code n_wa
	int max_wa_id;                  // maximum possible index of a trellis state

	int n_st;                       // number of states in the trellis
	int n_bits_st;                  // number of bits used to code n_st
	int max_st_id;                  // maximum possible index of a trellis state

	std::vector<SIN>  transition_to_binary;          // gives the binary code for a transition.
	                                                 // For each transition there are n_b_per_s bits separately given
	                                                 // (separate memory cases), in msb to lsb order

	std::vector<SIN>  binary_to_transition;          // gives the transition related to the binary code

	std::vector<int>  allowed_states;                // possible used state indexes
	                                                 // (there are jumps in the indexes because of p)

	std::vector<SOUT> allowed_wave_forms;            // possible used wave forms indexes
	                                                 // (there are jumps in the indexes because of p)

	std::vector<int>  trellis_next_state;            // from a given state and transition, gives the next state

	std::vector<SOUT> trellis_related_wave_form;     // from a given state and transition, gives the related wave form

	std::vector<int>  anti_trellis_original_state;   // from a given state and transition index,
	                                                 // gives the original state from where comes the transition

	std::vector<SIN>  anti_trellis_input_transition; // from a given state and transition index,
	                                                 // gives the related transition value
};
}
}

#endif /* CPM_PARAMETERS_HPP_ */
