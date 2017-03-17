#ifndef ENCODER_LDPC_HPP_
#define ENCODER_LDPC_HPP_

#include <vector>

#include "Tools/Code/LDPC/AList_reader/AList_reader.hpp"
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_LDPC : public Encoder<B>
{
	mipp::vector<B> tG; // the generator matrix

public:
	Encoder_LDPC(const int K, const int N, const tools::AList_reader &alist_G, const int n_frames = 1,
	             const std::string name = "Encoder_LDPC");
	virtual ~Encoder_LDPC();

	void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};

}
}

#endif /* ENCODER_LDPC_HPP_ */
