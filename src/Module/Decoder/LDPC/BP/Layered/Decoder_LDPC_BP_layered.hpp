#ifndef DECODER_LDPC_BP_LAYERED_HPP_
#define DECODER_LDPC_BP_LAYERED_HPP_

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"

#include "../../../Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename R = float>
class Decoder_LDPC_BP_layered : public Decoder_SISO<B,R>
{
private:
	int cur_frame;

protected:
	const int n_ite;     // number of iterations to perform
	const int n_C_nodes; // number of check nodes (= N - K)

	const bool enable_syndrome;

	// reset so C_to_V and V_to_C structures can be cleared only at the begining of the loop in iterative decoding
	bool init_flag;

	const std::vector<std::vector<unsigned int>> &CN_to_VN;

	// data structures for iterative decoding
	std::vector<mipp::vector<R>> var_nodes;
	std::vector<mipp::vector<R>> branches;

public:
	Decoder_LDPC_BP_layered(const int &K, const int &N, const int& n_ite,
	                        const tools::AList_reader &alist_data,
	                        const bool enable_syndrome = true,
	                        const int n_frames = 1,
	                        const std::string name = "Decoder_LDPC_BP_layered");
	virtual ~Decoder_LDPC_BP_layered();

	// unsupported prototype
	void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext);

protected:
	// soft decode
	void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2);

	// hard decoder (load -> decode -> store)
	void load       (const mipp::vector<R>& Y_N);
	void hard_decode(                          );
	void store      (      mipp::vector<B>& V_K) const;

	// BP functions for decoding
	void BP_decode();

	bool check_syndrome();

	virtual void BP_process(mipp::vector<R> &var_nodes, mipp::vector<R> &branches) = 0;
};
}
}

#endif /* DECODER_LDPC_BP_LAYERED_HPP_ */
