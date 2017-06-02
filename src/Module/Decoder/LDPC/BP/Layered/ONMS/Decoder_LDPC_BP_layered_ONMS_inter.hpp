#ifndef DECODER_LDPC_BP_LAYERED_ONMS_INTER_HPP_
#define DECODER_LDPC_BP_LAYERED_ONMS_INTER_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../../../../Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_layered_ONMS_inter : public Decoder_SISO<B,R>
{
private:
	int cur_wave;
	const float normalize_factor;
	const R offset;
	mipp::vector<mipp::Reg<R>> contributions;

protected:
	const R saturation;
	const int n_ite;     // number of iterations to perform
	const int n_C_nodes; // number of check nodes (= N - K)

	const bool enable_syndrome;
	const int  syndrome_depth;

	// reset so C_to_V and V_to_C structures can be cleared only at the beginning of the loop in iterative decoding
	bool init_flag;

	const mipp::vector<B> &info_bits_pos;

	const std::vector<std::vector<unsigned int>> &CN_to_VN;

	// data structures for iterative decoding
	std::vector<mipp::vector<mipp::Reg<R>>> var_nodes;
	std::vector<mipp::vector<mipp::Reg<R>>> branches;

	mipp::vector<mipp::Reg<R>> Y_N_reorderered;
	mipp::vector<mipp::Reg<B>> V_K_reorderered;

public:
	Decoder_LDPC_BP_layered_ONMS_inter(const int &K, const int &N, const int& n_ite,
	                                   const tools::AList_reader &alist_data,
	                                   const mipp::vector<B> &info_bits_pos,
	                                   const float normalize_factor = 1.f,
	                                   const R offset = (R)0,
	                                   const bool enable_syndrome = true,
	                                   const int syndrome_depth = 1,
	                                   const int n_frames = 1,
	                                   const std::string name = "Decoder_LDPC_BP_layered_ONMS_inter");
	virtual ~Decoder_LDPC_BP_layered_ONMS_inter();

protected:
	// soft decode
	void _soft_decode(const R *Y_N1, R *Y_N2);

	// hard decoder (load -> decode -> store)
	void _load       (const R *Y_N);
	void _hard_decode(const R *Y_N, B *V_K, const int frame_id);
	void _store      (B *V_K);

	// BP functions for decoding
	template <int F = 1>
	void BP_decode();

	bool check_syndrome();

	template <int F = 1>
	void BP_process(mipp::vector<mipp::Reg<R>> &var_nodes, mipp::vector<mipp::Reg<R>> &branches);
};
}
}

#endif /* DECODER_LDPC_BP_LAYERED_ONMS_INTER_HPP_ */
