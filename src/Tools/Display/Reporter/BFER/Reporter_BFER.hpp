#ifndef Reporter_BFER_HPP_
#define Reporter_BFER_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/BFER/Monitor_BFER.hpp"

#include "../Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int>
class Reporter_BFER : public Reporter
{
protected:
	const module::Monitor_BFER<B> &monitor;
	const bool display_throughput;
	std::chrono::time_point<std::chrono::steady_clock> t_report;

	group_t BFER_group;
	group_t throughput_group;

public:
	explicit Reporter_BFER(const module::Monitor_BFER<B> &monitor, const bool display_throughput = false);

	virtual ~Reporter_BFER() = default;

	void report(std::ostream &stream = std::cout, bool final = false);
};
}
}

#endif /* Reporter_BFER_HPP_ */
