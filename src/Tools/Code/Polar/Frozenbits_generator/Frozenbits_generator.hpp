#ifndef FROZENBITS_GENERATOR_HPP_
#define FROZENBITS_GENERATOR_HPP_

#include <cassert>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

template <typename B>
class Frozenbits_generator
{
protected:
	const int K;
	const int N;
	float sigma;

	std::vector<int> best_channels;

public:
	Frozenbits_generator(const int K, const int N, const float sigma = 0.f) 
	: K(K), N(N), sigma(sigma), best_channels(N) {}
	
	virtual ~Frozenbits_generator() {}

	void set_sigma(const float sigma) { this->sigma = sigma; }

	void generate(mipp::vector<B> &frozen_bits)
	{
		assert(frozen_bits.size() == (unsigned)N);

		this->evaluate();

		// init frozen_bits vector, true means frozen bits, false means information bits
		std::fill(frozen_bits.begin(), frozen_bits.end(), (B)1);
		for (auto i = 0; i < K; i++) 
			frozen_bits[best_channels[i]] = (B)0;
	}

	const std::vector<int>& get_best_channels() const { return best_channels; }

protected:
	virtual void evaluate() = 0;
};

#endif /* FROZENBITS_GENERATOR_HPP_ */
