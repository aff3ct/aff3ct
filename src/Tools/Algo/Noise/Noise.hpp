#ifndef NOISE_HPP_
#define NOISE_HPP_

#include <vector>

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Noise
{
public:
	Noise()
	{
	}

	virtual ~Noise()
	{
	}

	template <class A = std::allocator<R>>
	void generate(std::vector<R,A> &noise, const R sigma)
	{
		this->generate(noise.data(), (unsigned)noise.size(), sigma);
	}

	virtual void set_seed(const int seed) = 0;
	virtual void generate(R *noise, const unsigned length, const R sigma) = 0;

};
}
}

#endif /* CHANNEL_HPP_ */
