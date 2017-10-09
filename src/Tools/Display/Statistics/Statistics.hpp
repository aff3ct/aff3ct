#ifndef STATISTICS_HPP_
#define STATISTICS_HPP_

#include "Module/Module.hpp"
#include "Module/Task.hpp"

#include <vector>
#include <iostream>

namespace aff3ct
{
namespace tools
{
class Statistics
{
protected:
	Statistics();

public:
	virtual ~Statistics();

	static void show(std::vector<module::Module*> modules, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<module::Task*> tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<std::vector<module::Module*>> modules, const bool ordered = false,
	                 std::ostream &stream = std::cout);
	static void show(std::vector<std::vector<module::Task*>> tasks, const bool ordered = false,
	                 std::ostream &stream = std::cout);

private:
	static void separation1(std::ostream &stream = std::cout);

	static void separation2(std::ostream &stream = std::cout);

	static void show_header(std::ostream &stream = std::cout);

	static void show_task(const float                    total_sec,
	                      const std::string              module_sname,
	                      const std::string              task_name,
	                      const size_t                   task_n_elmts,
	                      const uint32_t                 task_n_calls,
	                      const std::chrono::nanoseconds task_tot_duration,
	                      const std::chrono::nanoseconds task_min_duration,
	                      const std::chrono::nanoseconds task_max_duration,
	                            std::ostream             &stream = std::cout);

	static void show_sub_task(const float                    total_sec,
	                          const std::string              task_name,
	                          const uint32_t                 task_n_calls,
	                          const size_t                   subtask_n_elmts,
	                          const std::string              subtask_name,
	                          const uint32_t                 subtask_n_calls,
	                          const std::chrono::nanoseconds subtask_tot_duration,
	                          const std::chrono::nanoseconds subtask_min_duration,
	                          const std::chrono::nanoseconds subtask_max_duration,
	                                std::ostream             &stream = std::cout);
};

using Stats = Statistics;
}
}

#endif /* STATISTICS_HPP_ */
