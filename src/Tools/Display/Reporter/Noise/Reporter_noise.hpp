#ifndef REPORTER_NOISE_HPP_
#define REPORTER_NOISE_HPP_

#include <string>
#include <chrono>

#include "Tools/Noise/noise_utils.h"

#include "../Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename R = float>
class Reporter_noise : public Reporter
{
protected:
	Noise<R>* const& noise;
	const Noise_type saved_noise_type;

	group_t noise_group;

public:
	explicit Reporter_noise(Noise<R>* const& noise);
	virtual ~Reporter_noise() = default;

	void report(std::ostream &stream = std::cout, bool final = false);
};
}
}

#endif /* REPORTER_NOISE_HPP_ */
