#ifndef ENCODER_SYS_HPP_
#define ENCODER_SYS_HPP_

#include "Encoder.hpp"

template <typename B>
class Encoder_sys : public Encoder<B>
{
public:
	Encoder_sys(const int n_frames = 1, const std::string name = "Encoder_sys") : Encoder<B>(n_frames) {};
	virtual ~Encoder_sys() {};

	// par bits : [par | tail bit sys | tail bits par]
	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par) = 0;
};

#endif /* ENCODER_SYS_HPP_ */
