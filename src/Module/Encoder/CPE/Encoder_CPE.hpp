/*
 * Encoder_CPE.hpp
 *
 *  Created on: 19 dec. 2016
 *      Author: ohartmann
 */

#ifndef ENCODER_CPE_HPP_
#define ENCODER_CPE_HPP_

#include <cassert>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"
#include "Module/Modulator/CPM/CPM_parameters.hpp"

template<typename S_in, typename S_out>
class Encoder_CPE : public Encoder<S_in>
{
public:
	Encoder_CPE(const int N, const CPM_parameters<S_in,S_out>& cpm, const int n_frames,
	                const std::string name = "Encoder_CPE");
	virtual ~Encoder_CPE() {}

	const int N; // number of symbol to send for one frame
	const CPM_parameters<S_in,S_out>& cpm;
	const int n_frames;

	int get_n_ff   () const;
	int tail_length() const;
	void encode    (const mipp::vector<S_in>& U_N, mipp::vector<S_out>& X_N);

	void generate_trellis	  (mipp::vector<int>& trellis_next_state,          mipp::vector<S_out>& trellis_related_wave_form   );
	void generate_anti_trellis(mipp::vector<int>& anti_trellis_original_state, mipp::vector<S_in>& anti_trellis_input_transition);

	// transition_to_binary :  value = 0 or 1 ; bit 0 is lsb, bit cpm.Nb_b_per_s-1 is msb
	virtual void generate_mapper(mipp::vector<S_in>& transition_to_binary, mipp::vector<S_in>& binary_to_transition, const std::string& mapping_layout) = 0;

	virtual void generate_allowed_states	(mipp::vector<int>& allowed_states      ) = 0;
	virtual void generate_allowed_wave_forms(mipp::vector<S_out>& allowed_wave_forms) = 0;

	/* merge given bit on separate memory addresses to one variable
	 * takes number_of_bits from the in_bits array by increasing the memory address
	 * msb_to_lsb=true means that the first read bit is the msb and the number_of_bits one is the lsb. msb_to_lsb = false means the opposite
	 * in_bit must point on 0 or 1 values else result will be wrong
	 */
	S_in merge_bits(const S_in* in_bit, const int number_of_bits, const bool msb_to_lsb = true);

protected:
	void frame_encode(const S_in* U_N, S_out* X_N);

	virtual S_out inner_encode(const S_in bit_sys, int &state) = 0;
	virtual S_in  tail_symb   (const int &state              ) = 0;
};

#include "Encoder_CPE.hxx"

#endif /* ENCODER_CPE_HPP_ */
