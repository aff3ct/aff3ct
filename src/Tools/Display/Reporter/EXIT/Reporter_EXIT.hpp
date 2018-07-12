#ifndef Reporter_EXIT_HPP_
#define Reporter_EXIT_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Tools/Noise/Noise.hpp"

#include "../Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Reporter_EXIT : public Reporter
{
protected:
	const module::Monitor_EXIT<B,R> &monitor;
	const Noise<R>& noise_a;

	group_t EXIT_group;

public:
	explicit Reporter_EXIT(const module::Monitor_EXIT<B,R> &monitor, const Noise<R>& noise_a);

	virtual ~Reporter_EXIT() = default;

	report_t report(bool final = false);
};
}
}

#endif /* Reporter_EXIT_HPP_ */
