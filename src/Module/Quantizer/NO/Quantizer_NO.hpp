#ifndef QUANTIZER_NO_HPP_
#define QUANTIZER_NO_HPP_

#include "../Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R = float, typename Q = int>
class Quantizer_NO : public Quantizer<R,Q>
{
public:
	Quantizer_NO(const int N, const int n_frames = 1);
	virtual ~Quantizer_NO();

	void process(const R *Y_N1, Q *Y_N2); using Quantizer<R,Q>::process;
};
}
}

#endif /* QUANTIZER_NO_HPP_ */

