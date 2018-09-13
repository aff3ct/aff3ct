#ifndef PUNCTURER_LDPC_HPP_
#define PUNCTURER_LDPC_HPP_

#include <vector>

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_LDPC : public Puncturer<B,Q>
{
protected:
	std::vector<bool> pattern_bits;
	int               Z;

public:
	Puncturer_LDPC(const int &K,
	               const int &N,
	               const int &N_cw,
	               const std::vector<bool> &pattern,
	               const int n_frames = 1);
	virtual ~Puncturer_LDPC() = default;

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_LDPC_HPP_ */
