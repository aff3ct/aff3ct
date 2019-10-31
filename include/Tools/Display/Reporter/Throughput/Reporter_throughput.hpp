/*!
 * \file
 * \brief Class tools::Reporter_throughput.
 */
#ifndef REPORTER_THROUGHPUT_HPP_
#define REPORTER_THROUGHPUT_HPP_

#include <chrono>
#include <cstdint>
#include <functional>
#include <type_traits>

#include "Module/Monitor/MI/Monitor_MI.hpp"
#include "Module/Monitor/BFER/Monitor_BFER.hpp"
#include "Module/Monitor/EXIT/Monitor_EXIT.hpp"
#include "Tools/Display/Reporter/Reporter.hpp"

namespace aff3ct
{
namespace tools
{
template <typename T = uint64_t>
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

	template<typename B>
	explicit Reporter_throughput(const module::Monitor_BFER<B>& m);

	template<typename B, typename R>
	explicit Reporter_throughput(const module::Monitor_MI<B,R>& m);

	template<typename B, typename R>
	explicit Reporter_throughput(const module::Monitor_EXIT<B,R>& m);

	virtual ~Reporter_throughput() = default;

	report_t report(bool final = false);

	void init();
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Display/Reporter/Throughput/Reporter_throughput.hxx"
#endif

#endif /* REPORTER_THROUGHPUT_HPP_ */
