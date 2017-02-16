#ifndef ENCODER_LDPC_HPP_
#define ENCODER_LDPC_HPP_

#include <random>

#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_LDPC : public Encoder_sys<B>
{
	const int K, N;

public:
	Encoder_LDPC(const int K, const int N, const int n_frames = 1,
	              const std::string name = "Encoder_LDPC");
	virtual ~Encoder_LDPC();

	void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);
};

#endif /* ENCODER_LDPC_HPP_ */
