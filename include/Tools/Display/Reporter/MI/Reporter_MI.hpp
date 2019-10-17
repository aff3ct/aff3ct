/*!
 * \file
 * \brief Class tools::Reporter_MI.
 */
#ifndef REPORTER_MI_HPP_
#define REPORTER_MI_HPP_

#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Tools/Display/Reporter/Reporter_monitor.hpp"

namespace aff3ct
{
namespace tools
{
template <typename B = int32_t, typename R = float>
class Reporter_MI : public Reporter_monitor<module::Monitor_MI<B,R>>
{
public:
	using Rm = Reporter_monitor<module::Monitor_MI<B,R>>;
	using typename Rm::M;
	using typename Rm::report_t;

public:
	explicit Reporter_MI(const M &monitor);

	virtual ~Reporter_MI() = default;

	report_t report(bool final = false);

private:
	void create_groups();
};
}
}

#endif /* REPORTER_MI_HPP_ */
