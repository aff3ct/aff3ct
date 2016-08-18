#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Decoder_interface // please use Decoder<B> for inheritance (instead of Decoder_interface<B>)
{
protected:
	const int K; // frame size
	const int N; // number of information bits in the frame size

public:
	Decoder_interface(const int K, const int N, std::string name = "Decoder_interface") : K(K), N(N) {};
	virtual ~Decoder_interface() {};

	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

	virtual void store_fast(mipp::vector<B>& V) const { store(V); }
	virtual void unpack    (mipp::vector<B>& V) const {           }

	virtual int get_n_frames() const { return 1; }
};

#include "SC_Decoder.hpp"

#endif /* DECODER_HPP_ */