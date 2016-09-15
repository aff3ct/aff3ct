#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <vector>
#include "Tools/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B>
class Encoder_interface : public Module // please use Encoder<B> for inheritance (instead of Encoder_interface<B>)
{
protected:
	const int K; // number of information bits in the frame size
	const int N; // frame size

public:
	Encoder_interface(const int K, const int N, const int n_frames = 1, const std::string name = "Encoder_interface") 
	: Module(n_frames, name), K(K), N(N) {};
	virtual ~Encoder_interface() {};

	virtual void encode(const mipp::vector<B>& U_K, mipp::vector<B>& X_N) = 0;

	virtual int tail_length() const { return 0; }
};

#include "SC_Encoder.hpp"

#endif /* ENCODER_HPP_ */
