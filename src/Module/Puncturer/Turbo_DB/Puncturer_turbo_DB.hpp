#ifndef PUNCTURER_TURBO_DB_HPP_
#define PUNCTURER_TURBO_DB_HPP_

#include <string>

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Puncturer_turbo_DB : public Puncturer<B,Q>
{
protected:
	std::string coderate;

public:
	Puncturer_turbo_DB(const int &K, const int &N, const int n_frames = 1);
	virtual ~Puncturer_turbo_DB() = default;

protected:
	void   _puncture(const B *X_N1, B *X_N2, const int frame_id) const;
	void _depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const;
};
}
}

#endif /* PUNCTURER_TURBO_DB_HPP_ */
