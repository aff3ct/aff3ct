#ifndef PUNCTURER_NO_HPP_
#define PUNCTURER_NO_HPP_

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_NO : public Puncturer<B,Q>
{
public:
	Puncturer_NO(const int K, const int N, const int n_frames = 1);
	virtual ~Puncturer_NO() = default;

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_NO_HPP_ */
