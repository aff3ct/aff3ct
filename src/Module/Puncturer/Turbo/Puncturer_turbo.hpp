#ifndef PUNCTURER_TURBO_HPP_
#define PUNCTURER_TURBO_HPP_

#include <vector>

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_turbo : public Puncturer<B,Q>
{
protected:
	const std::vector<std::vector<bool>> pattern_bits;
	const bool                           buff_enc;
	const int                            tail_bits;

public:
	Puncturer_turbo(const int &K,
	                const int &N,
	                const int &tail_bits,
	                const std::vector<std::vector<bool>> &pattern_bits,
	                const bool buff_enc = true,
	                const int n_frames = 1);
	virtual ~Puncturer_turbo() = default;

	static unsigned    get_period     (const std::vector<std::vector<bool>>& pattern_bits);
	static unsigned    get_bit_count  (const std::vector<std::vector<bool>>& pattern_bits);
	static std::string display_pattern(const std::vector<std::vector<bool>>& pattern_bits);

	static void check_pattern(const int K,                      const std::vector<std::vector<bool>>& pattern_bits);
	static int  compute_N    (const int K, const int tail_bits, const std::vector<std::vector<bool>>& pattern_bits);

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_TURBO_HPP_ */
