#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <vector>

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder_LDPC.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_LDPC_from_H : public Encoder_LDPC<B>
{
protected:
	mipp::vector<int> swapped;
	mipp::vector<B> G;
	void build_H(const int k, const int n, const std::vector<std::vector<unsigned int>>& positions,
				 std::vector<mipp::vector<B>>& H);
	void triangularization_H(std::vector<mipp::vector<B>>& H, mipp::vector<int>& swapped);
	void identity_H(std::vector<mipp::vector<B>>& H);
	void transformation_H_to_G(std::vector<mipp::vector<B>>& H, mipp::vector<B>& G);

public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames = 1,
	                    const std::string name = "Encoder_LDPC_from_H");
	virtual ~Encoder_LDPC_from_H();

	virtual void get_info_bits_pos(mipp::vector<B>& info_bits_pos);

	virtual void get_G(mipp::vector<B>& G);
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
