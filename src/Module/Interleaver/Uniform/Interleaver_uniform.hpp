#ifndef INTERLEAVER_UNIFORM_HPP
#define	INTERLEAVER_UNIFORM_HPP

#include "../Random/Interleaver_random.hpp"

namespace aff3ct
{
namespace module
{
template <typename T = int>
class Interleaver_uniform : public Interleaver_random<T>
{
public:
	Interleaver_uniform(int size, const int seed = 0, const std::string name = "Interleaver_uniform")
	: Interleaver_random<T>(size, seed, name) {}

	virtual ~Interleaver_uniform() {}

	void regen_lookup_tables()
	{
		this->gen_lookup_tables();
	}
};
}
}

#endif
