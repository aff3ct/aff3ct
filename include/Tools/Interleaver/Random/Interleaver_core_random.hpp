/*!
 * \file
 * \brief Class tools::Interleaver_core_random.
 */
#ifndef INTERLEAVER_CORE_RANDOM_HPP
#define INTERLEAVER_CORE_RANDOM_HPP

#include <cstdint>
#include <random>

#include "Tools/Interleaver/Interleaver_core.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint32_t>
class Interleaver_core_random : public Interleaver_core<T>
{
private:
	int          seed;
	std::mt19937 rd_engine;

public:
	Interleaver_core_random(const int size, const int seed = 0, const bool uniform = false);
	virtual ~Interleaver_core_random() = default;

	virtual Interleaver_core_random<T>* clone() const;

	virtual void set_seed(const int seed);

	virtual void reinitialize();

protected:
	void gen_lut(T *lut, const size_t frame_id);
};
}
}

#endif	/* INTERLEAVER_CORE_RANDOM_HPP */

