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

	static void show(std::vector<const module::Module*> modules, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<const module::Task*> tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<std::vector<const module::Module*>> modules, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<std::vector<const module::Task*>> tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);

private:
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
