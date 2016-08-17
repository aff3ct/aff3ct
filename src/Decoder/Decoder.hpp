#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <string>
#include <vector>
#include "../Tools/MIPP/mipp.h"

template <typename B, typename R>
class Decoder
{
public:
	Decoder(std::string name = "Decoder") {};
	virtual ~Decoder() {};

	virtual void load  (const mipp::vector<R>& Y_N)       = 0;
	virtual void decode(                          )       = 0;
	virtual void store (      mipp::vector<B>& V_K) const = 0;

	virtual void store_fast(mipp::vector<B>& V) const { store(V); }
	virtual void unpack    (mipp::vector<B>& V) const {           }

	virtual int get_n_frames() const { return 1; }
};

#endif /* DECODER_HPP_ */