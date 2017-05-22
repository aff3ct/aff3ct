#ifndef ENCODER_LDPC_DVBS2_HPP_
#define ENCODER_LDPC_DVBS2_HPP_

#include <vector>

#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder_LDPC.hpp"

#include "Encoder_LDPC_DVBS2_constants.hpp"

namespace aff3ct
{
namespace module
{

template <typename B = int>
class Encoder_LDPC_DVBS2 : public Encoder_LDPC<B>
{
	const dvbs2_values* dvbs2 = nullptr;

public:
	Encoder_LDPC_DVBS2(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_LDPC_DVBS2");
	virtual ~Encoder_LDPC_DVBS2();

protected:
	void _encode(const B *U_K, B *X_N, const int frame_id);

private:
	void build_dvbs2();
};

}
}

#endif /* ENCODER_LDPC_DVBS2_HPP_ */
