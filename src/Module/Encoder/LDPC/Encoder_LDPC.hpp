#ifndef ENCODER_LDPC_HPP_
#define ENCODER_LDPC_HPP_

#include <vector>
#include "Tools/Algo/Sparse_matrix/Sparse_matrix.hpp"

#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC : public Encoder<B>
{
protected:
	mipp::vector<B> tG; // the generator matrix

protected:
	Encoder_LDPC(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_LDPC");

public:
	Encoder_LDPC(const int K, const int N, const tools::Sparse_matrix &G, const int n_frames = 1,
	             const std::string name = "Encoder_LDPC");
	virtual ~Encoder_LDPC();

	virtual void get_info_bits_pos(mipp::vector<B>& info_bits_pos);

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id);
};

}
}

#endif /* ENCODER_LDPC_HPP_ */
