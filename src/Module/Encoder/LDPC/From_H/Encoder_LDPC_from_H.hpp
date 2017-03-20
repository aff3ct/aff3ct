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
public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames = 1,
	                    const std::string name = "Encoder_LDPC_from_H");
	virtual ~Encoder_LDPC_from_H();

	virtual void get_info_bits_pos(mipp::vector<B>& info_bits_pos);
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
