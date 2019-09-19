#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include "Tools/Code/CPM/CPM_parameters.hpp"

namespace aff3ct
{
namespace tools
{
template <typename SIN, typename SOUT>
CPM_parameters<SIN,SOUT>
::CPM_parameters(const int L,
                 const int k,
                 const int p,
                 const int n_b_per_s,
                 const int s_factor,
                 const std::string& filters_type,
                 const std::string& wave_shape)
: L           (L                                                              ),
  k           (k                                                              ),
  p           (p                                                              ),
  n_b_per_s   (n_b_per_s                                                      ),
  s_factor    (s_factor                                                       ),
  filters_type(filters_type                                                   ),
  wave_shape  (wave_shape                                                     ),
  m_order     ((int)1 << n_b_per_s                                            ),
  n_bits_p    ((int)std::ceil(std::log2((double)p))                           ),
  tl          ((int)(std::ceil((float)(p - 1) / (float)(m_order - 1))) + L - 1),
  // TODO: warning: from here parameters are working for Rimoldi decomposition only!
  n_wa        ((int)(p * std::pow(m_order, L))                                ),
  n_bits_wa   ((int)std::ceil(std::log2(n_wa))                                ),
  max_wa_id   (((int)1 << n_bits_wa)                                          ),
  n_st        ((int)(p * std::pow(m_order, L-1))                              ),
  n_bits_st   ((int)std::ceil(std::log2(n_st))                                ),
  max_st_id   (((int)1 << n_bits_st)                                          ),

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
}
}
