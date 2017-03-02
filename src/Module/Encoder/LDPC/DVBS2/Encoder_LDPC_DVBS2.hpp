#ifndef ENCODER_LDPC_DVBS2_HPP_
#define ENCODER_LDPC_DVBS2_HPP_

#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "../../Encoder_sys.hpp"
#include "Encoder_LDPC_DVBS2_constants.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
class Encoder_LDPC_DVBS2 : public Encoder_sys<B>
{
	const int K, N;
	const dvbs2_values* dvbs2 = nullptr;

public:
	Encoder_LDPC_DVBS2(const int K, const int N, const int n_frames = 1,
	                   const std::string name = "Encoder_LDPC");
	virtual ~Encoder_LDPC_DVBS2();


	void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);

private:
	void build_dvbs2();
};

}
}

#endif /* ENCODER_LDPC_DVBS2_HPP_ */
