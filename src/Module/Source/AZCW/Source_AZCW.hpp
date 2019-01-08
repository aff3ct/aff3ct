#ifndef SOURCE_AZCW_HPP_
#define SOURCE_AZCW_HPP_

#include <random>
#include <vector>
#include <mipp.h>

#include "../Source.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int>
class Source_AZCW : public Source<B>
{
public:
	Source_AZCW(const int K, const int n_frames = 1);

	virtual ~Source_AZCW() = default;

protected:
	void _generate(B *U_K, const int frame_id);
};
}
}

#endif /* SOURCE_AZCW_HPP_ */
