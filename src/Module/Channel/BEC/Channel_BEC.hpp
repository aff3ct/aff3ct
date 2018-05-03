#ifndef CHANNEL_BEC_HPP_
#define CHANNEL_BEC_HPP_

#include <vector>
#include <mipp.h>

#include <random>

#include "Tools/Algo/PRNG/PRNG_MT19937.hpp"
#include "Tools/Algo/PRNG/PRNG_MT19937_simd.hpp"

#include "../Channel.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float>
class Channel_BEC : public Channel<R>
{
protected:
	// for float type
	tools::PRNG_MT19937      mt19937;      // Mersenne Twister 19937 (scalar)
	tools::PRNG_MT19937_simd mt19937_simd; // Mersenne Twister 19937 (SIMD)

	// for double type
	std::mt19937                      rd_engine; // Mersenne Twister 19937
	std::uniform_real_distribution<R> uni_dist;

public:
	Channel_BEC(const int N, const int seed = 0,
	            const tools::Event_probability<R>& noise = tools::Event_probability<R>(),
	            const int n_frames = 1);
	virtual ~Channel_BEC() = default;

protected:
	void _add_noise(const R *X_N, R *Y_N, const int frame_id = -1);

	inline mipp::Reg<R> get_random_simd();
	inline R            get_random     ();
	virtual void        check_noise    ();
};
}
}

#endif /* CHANNEL_BEC_HPP_ */
