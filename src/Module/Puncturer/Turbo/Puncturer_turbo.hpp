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
	      std::vector<std::vector<bool>> pattern_bits;
	const bool                           buff_enc;
	const int                            tail_bits;

public:
	Puncturer_turbo(const int &K,
	                const int &N,
	                const int &tail_bits,
	                const std::string &pattern,
	                const bool buff_enc = true,
	                const int n_frames = 1,
	                const std::string name = "Puncturer_turbo");
	virtual ~Puncturer_turbo();

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_TURBO_HPP_ */
