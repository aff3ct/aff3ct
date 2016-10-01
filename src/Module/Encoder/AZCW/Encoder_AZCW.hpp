#ifndef ENCODER_AZCW_HPP_
#define ENCODER_AZCW_HPP_

#include <random>

#include "../Encoder_sys.hpp"

template <typename B>
class Encoder_AZCW : public Encoder_sys<B>
{
public:
	Encoder_AZCW(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_AZCW");
	virtual ~Encoder_AZCW();

	void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
	void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);
};

#endif /* ENCODER_AZCW_HPP_ */
