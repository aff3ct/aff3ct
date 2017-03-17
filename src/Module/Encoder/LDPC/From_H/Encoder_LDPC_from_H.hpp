#ifndef ENCODER_LDPC_FROM_H_HPP_
#define ENCODER_LDPC_FROM_H_HPP_

#include <vector>

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"
#include "Tools/Perf/MIPP/mipp.h"

#include "../../Encoder.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_LDPC_from_H : public Encoder<B>
{
	mipp::vector<B> tG; // the generator matrix

public:
	Encoder_LDPC_from_H(const int K, const int N, const tools::AList_reader &alist_H, const int n_frames = 1,
	                    const std::string name = "Encoder_LDPC_from_H");
	virtual ~Encoder_LDPC_from_H();

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

}
}

#endif /* ENCODER_LDPC_FROM_H_HPP_ */
