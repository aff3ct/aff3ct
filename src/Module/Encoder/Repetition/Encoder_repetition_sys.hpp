#ifndef ENCODER_REPETITION_SYS_HPP_
#define ENCODER_REPETITION_SYS_HPP_

#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "../Encoder_sys.hpp"

namespace aff3ct
{
template <typename B>
class Encoder_repetition_sys : public Encoder_sys<B>
{
protected:
	const int rep_count; // number of repetition

	const bool buffered_encoding;
	
public:
	Encoder_repetition_sys(const int& K, const int& N, const bool buffered_encoding = true,
	                       const std::string name = "Encoder_repetition_sys");
	virtual ~Encoder_repetition_sys() {}

	virtual void encode_sys(const mipp::vector<B>& U_K, mipp::vector<B>& par);
	virtual void encode    (const mipp::vector<B>& U_K, mipp::vector<B>& X_N);
};
}

#endif // ENCODER_REPETITION_SYS_HPP_
