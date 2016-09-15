#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <string>
#include <vector>
#include "Tools/MIPP/mipp.h"

#include "Module/Module.hpp"

template <typename B, typename R>
class Decoder_interface : public Module // please use Decoder<B,R> for inheritance (instead of Decoder_interface<B,R>)
{
protected:
	const int K; // frame size
	const int N; // number of information bits in the frame size

public:
	Decoder_interface(const int K, const int N, const int n_frames = 1, std::string name = "Decoder_interface") 
	: Module(n_frames, name), K(K), N(N) {}
	virtual ~Decoder_interface() {}

	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

	virtual void store_fast(mipp::vector<B>& V) const { store(V); }
	virtual void unpack    (mipp::vector<B>& V) const {           }
};

#include "SC_Decoder.hpp"

#endif /* DECODER_HPP_ */
