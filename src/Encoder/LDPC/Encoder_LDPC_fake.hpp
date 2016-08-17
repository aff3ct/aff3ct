#ifndef ENCODER_LDPC_FAKE_HPP_
#define ENCODER_LDPC_FAKE_HPP_

#include "Encoder_LDPC_sys.hpp"

template <typename B>
class Encoder_LDPC_fake : public Encoder_LDPC_sys<B>
{
protected:
	const int K;
	const int N;

public:
	Encoder_LDPC_fake(const int K, const int N, const int n_frames = 1);
	virtual ~Encoder_LDPC_fake();

	std::vector<unsigned char> get_n_variables_per_parity ();
	std::vector<unsigned char> get_n_parities_per_variable();
	std::vector<unsigned int > get_transpose              ();

	void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);
};

#endif /* ENCODER_LDPC_FAKE_HPP_ */
