#ifndef REPORTER_THROUGHPUT_HPP_
#define REPORTER_THROUGHPUT_HPP_

#include <string>
#include <chrono>
#include <functional>
#include <cassert>
#include <type_traits>

#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "../Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T>
class Reporter_throughput : public Reporter
{
	static_assert(std::is_convertible<T, double>::value, "T type must be convertible to a double.");

protected:
	std::function<T(void)> progress_function;
	std::function<T(void)> get_nbits_function;

	const T progress_limit;
	const T nbits_factor;

	std::chrono::time_point<std::chrono::steady_clock> t_report;

	group_t throughput_group;

public:
	explicit Reporter_throughput(std::function<T(void)> progress_function, const T progress_limit = 0,
	                             std::function<T(void)> get_nbits_function = nullptr, const T nbits_factor = 1);

	virtual ~Reporter_throughput() = default;

	report_t report(bool final = false);

	void init();
};
}
}

#include "Reporter_throughput.hxx"

#endif /* REPORTER_THROUGHPUT_HPP_ */
