/*!
 * \file
 * \brief Class tools::Encoder_CPE_Rimoldi.
 */
#ifndef ENCODER_CPE_RIMOLDI_HPP_
#define ENCODER_CPE_RIMOLDI_HPP_

#include <vector>
#include <string>

#include "Tools/Code/CPM/CPM_parameters.hpp"
#include "Tools/Code/CPM/CPE/Encoder_CPE.hpp"

namespace aff3ct
{
namespace tools
{
template <typename SIN = int, typename SOUT = int>
class Encoder_CPE_Rimoldi : public Encoder_CPE<SIN, SOUT>
{
protected:
	std::vector<SIN> tail_symb_transition;

public:
	Encoder_CPE_Rimoldi(const int N, const CPM_parameters<SIN,SOUT>& cpm);
	virtual ~Encoder_CPE_Rimoldi() = default;

	SOUT inner_encode(const SIN new_symbol, int &state);
	SIN  tail_symb   (const int &state                );

	void generate_mapper(std::vector<SIN>& transition_to_binary,
	                     std::vector<SIN>& binary_to_transition,
	                     const std::string& mapping);

	void generate_allowed_states      (std::vector<int>& allowed_states     );
	void generate_allowed_wave_forms  (std::vector<SOUT>& allowed_wave_forms);
	void generate_tail_symb_transition(                                     );
};
}
}

#endif /* ENCODER_CPE_RIMOLDI_HPP_ */
