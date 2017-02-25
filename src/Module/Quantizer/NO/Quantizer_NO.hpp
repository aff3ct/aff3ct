#ifndef QUANTIZER_NO_HPP_
#define QUANTIZER_NO_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Quantizer.hpp"

namespace aff3ct
{
namespace module
{
template <typename R, typename Q>
class Quantizer_NO : public Quantizer<R,Q>
{
public:
	Quantizer_NO(const int N, const int n_frames = 1, const std::string name = "Quantizer_NO");
	virtual ~Quantizer_NO();

	void process(const mipp::vector<R>& Y_N1, mipp::vector<Q>& Y_N2);
};
}
}

#endif /* QUANTIZER_NO_HPP_ */

