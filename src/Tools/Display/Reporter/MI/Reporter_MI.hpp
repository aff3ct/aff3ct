#ifndef REPORTER_MI_HPP_
#define REPORTER_MI_HPP_

#include <string>
#include <chrono>

#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "../Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int, typename R = float>
class Reporter_MI : public Reporter
{
protected:
	const module::Monitor_MI<B,R> &monitor;

	group_t MI_group;

public:
	explicit Reporter_MI(const module::Monitor_MI<B,R> &monitor);

	virtual ~Reporter_MI() = default;

	report_t report(bool final = false);
};
}
}

#endif /* REPORTER_MI_HPP_ */
