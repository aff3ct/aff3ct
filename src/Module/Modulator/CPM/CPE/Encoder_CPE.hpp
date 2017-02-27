#ifndef ENCODER_CPE_HPP_
#define ENCODER_CPE_HPP_

#include <cassert>
#include "Tools/Perf/MIPP/mipp.h"

#include "../CPM_parameters.hpp"

namespace aff3ct
{
namespace module
{
template <typename SIN = int, typename SOUT = int>
class Encoder_CPE
{
public:
	Encoder_CPE(const int N, const CPM_parameters<SIN,SOUT>& cpm, const int n_frames,
	                const std::string name = "Encoder_CPE");
	virtual ~Encoder_CPE() {}

	const int N; // number of symbol to send for one frame
	const CPM_parameters<SIN,SOUT>& cpm;
	const int n_frames;

	int get_n_ff   () const;
	int tail_length() const;
	void encode    (const mipp::vector<SIN>& U_N, mipp::vector<SOUT>& X_N);

	void generate_trellis     (mipp::vector<int>& trellis_next_state,
	                           mipp::vector<SOUT>& trellis_related_wave_form);
	void generate_anti_trellis(mipp::vector<int>& anti_trellis_original_state,
	                           mipp::vector<SIN>& anti_trellis_input_transition);

	// transition_to_binary :  value = 0 or 1 ; bit 0 is lsb, bit cpm.n_b_per_s-1 is msb
	virtual void generate_mapper(mipp::vector<SIN>& transition_to_binary,
	                             mipp::vector<SIN>& binary_to_transition,
	                             const std::string& mapping_layout) = 0;

	virtual void generate_allowed_states    (mipp::vector<int>& allowed_states     ) = 0;
	virtual void generate_allowed_wave_forms(mipp::vector<SOUT>& allowed_wave_forms) = 0;

	// merge given bit on separate memory addresses to one variable
	// takes number_of_bits from the in_bits array by increasing the memory address
	// msb_to_lsb=true means that the first read bit is the msb and the number_of_bits one is the lsb.
	// msb_to_lsb = false means the opposite
	// in_bit must point on 0 or 1 values else result will be wrong
	SIN merge_bits(const SIN* in_bit, const int number_of_bits, const bool msb_to_lsb = true);

protected:
	void frame_encode(const SIN* U_N, SOUT* X_N);

	virtual SOUT inner_encode(const SIN bit_sys, int &state) = 0;
	virtual SIN  tail_symb   (const int &state             ) = 0;
};
}
}

#endif /* ENCODER_CPE_HPP_ */
