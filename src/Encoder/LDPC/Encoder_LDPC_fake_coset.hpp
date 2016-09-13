#ifndef ENCODER_LDPC_FAKE_COSET_HPP_
#define ENCODER_LDPC_FAKE_COSET_HPP_

#include <random>

#include "Encoder_LDPC_fake.hpp"

template <typename B>
class Encoder_LDPC_fake_coset : public Encoder_LDPC_fake<B>
{
private:
	std::mt19937 rd_engine; // Mersenne Twister 19937
	std::uniform_int_distribution<B> uniform_dist;

public:
	Encoder_LDPC_fake_coset(const int K, const int N, const int seed = 0, const int n_frames = 1, const std::string name = "Encoder_LDPC_fake_coset");
	virtual ~Encoder_LDPC_fake_coset();

	void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);
};

#endif /* ENCODER_LDPC_FAKE_COSET_HPP_ */
