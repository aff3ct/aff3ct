/*!
 * \file
 * \brief Class tools::Statistics.
 */
#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <chrono>
#include <string>

#include "Module/Module.hpp"
#include "Module/Task.hpp"

namespace aff3ct
{
namespace tools
{
class Statistics
{
protected:
	Statistics() = default;

public:
	virtual ~Statistics() = default;

	template <class MODULE_OR_TASK>
	static void show(std::vector<MODULE_OR_TASK*> modules_or_tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	template <class MODULE_OR_TASK>
	static void show(std::vector<std::vector<MODULE_OR_TASK*>> modules_or_tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);

private:
	template <class MODULE = const module::Module>
	static void show_modules(std::vector<MODULE*> modules, const bool ordered = false,
	                         std::ostream &stream = std::cout);
	template <class TASK = const module::Task>
	static void show_tasks(std::vector<TASK*> tasks, const bool ordered = false,
	                       std::ostream &stream = std::cout);
	template <class MODULE = const module::Module>
	static void show_modules(std::vector<std::vector<MODULE*>> modules, const bool ordered = false,
	                         std::ostream &stream = std::cout);
	template <class TASK = const module::Task>
	static void show_tasks(std::vector<std::vector<TASK*>> tasks, const bool ordered = false,
	                       std::ostream &stream = std::cout);

	static void separation1(std::ostream &stream = std::cout);

	static void separation2(std::ostream &stream = std::cout);

	static void show_header(std::ostream &stream = std::cout);

	static void show_task(const float                    total_sec,
	                      const std::string&             module_sname,
	                      const std::string&             task_name,
	                      const size_t                   task_n_elmts,
	                      const uint32_t                 task_n_calls,
	                      const std::chrono::nanoseconds task_tot_duration,
	                      const std::chrono::nanoseconds task_min_duration,
	                      const std::chrono::nanoseconds task_max_duration,
	                            std::ostream             &stream = std::cout);

	static void show_timer(const float                    total_sec,
	                       const uint32_t                 task_n_calls,
	                       const size_t                   timer_n_elmts,
	                       const std::string&             timer_name,
	                       const uint32_t                 timer_n_calls,
	                       const std::chrono::nanoseconds timer_tot_duration,
	                       const std::chrono::nanoseconds timer_min_duration,
	                       const std::chrono::nanoseconds timer_max_duration,
	                             std::ostream             &stream = std::cout);
};

using Stats = Statistics;
}
}

#endif /* STATISTICS_HPP_ */
