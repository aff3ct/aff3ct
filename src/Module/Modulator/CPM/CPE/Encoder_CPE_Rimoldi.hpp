#ifndef ENCODER_CPE_RIMOLDI_HPP_
#define ENCODER_CPE_RIMOLDI_HPP_

#include "Tools/Perf/MIPP/mipp.h"
#include "Encoder_CPE.hpp"

template<typename SIN, typename SOUT>
class Encoder_CPE_Rimoldi : public Encoder_CPE<SIN, SOUT>
{
public:
	Encoder_CPE_Rimoldi(const int N,
	                    const CPM_parameters<SIN,SOUT>& cpm,
	                    const int n_frames = 1,
	                    const std::string name = "Encoder_CPE_Rimoldi");
	virtual ~Encoder_CPE_Rimoldi() {}

	SOUT inner_encode(const SIN new_symbol, int &state);
	SIN  tail_symb   (const int &state                );

	void generate_mapper(mipp::vector<SIN>& transition_to_binary,
	                     mipp::vector<SIN>& binary_to_transition,
	                     const std::string& mapping);

	void generate_allowed_states    (mipp::vector<int>& allowed_states     );
	void generate_allowed_wave_forms(mipp::vector<SOUT>& allowed_wave_forms);
};

#include "Encoder_CPE_Rimoldi.hxx"

#endif /* ENCODER_CPE_RIMOLDI_HPP_ */
