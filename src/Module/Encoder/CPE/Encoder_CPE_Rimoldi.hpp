/*
 * Encoder_CPE_Rimoldi.hpp
 *
 *  Created on: 19 d�c. 2016
 *      Author: ohartmann
 */

#ifndef ENCODER_CPE_RIMOLDI_HPP_
#define ENCODER_CPE_RIMOLDI_HPP_

#include "Tools/Perf/MIPP/mipp.h"
#include "Encoder_CPE.hpp"

template<typename S_in, typename S_out>
class Encoder_CPE_Rimoldi : public Encoder_CPE<S_in, S_out>
{
public:
	Encoder_CPE_Rimoldi(const int N, const CPM_parameters<S_in,S_out>& cpm, const int n_frames = 1,
	                     const std::string name = "Encoder_CPE_Rimoldi");
	virtual ~Encoder_CPE_Rimoldi() {}

	S_out inner_encode(const S_in new_symbol, int &state);
	S_in  tail_symb   (const int &state                 );

	void generate_mapper(mipp::vector<S_in>& transition_to_binary, mipp::vector<S_in>& binary_to_transition, const std::string& mapping);

	void generate_allowed_states    (mipp::vector<int>& allowed_states      );
	void generate_allowed_wave_forms(mipp::vector<S_out>& allowed_wave_forms);
};


#include "Encoder_CPE_Rimoldi.hxx"

#endif /* ENCODER_CPE_RIMOLDI_HPP_ */
