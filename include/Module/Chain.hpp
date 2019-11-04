/*!
 * \file
 * \brief Class module::Chain.
 */
#ifndef CHAIN_HPP_
#define CHAIN_HPP_

#include <functional>
#include <vector>

namespace aff3ct
{
namespace module
{
class Task;

class Chain
{
protected:
	size_t n_threads;
	std::vector<Task*> tasks_sequence;
	std::vector<std::vector<Task*>> tasks_sequences;

public:
	Chain(Task &bootstrap, const size_t n_threads = 1);
	virtual ~Chain();

	void exec(std::function<bool(const std::vector<int>&)> &stop_condition);
	inline size_t get_n_threads() const;

protected:
	void init_recursive(Task& current_task);
	void _exec(std::function<bool(const std::vector<int>&)> &stop_condition, std::vector<Task*> &tasks_sequence);
	void duplicate();

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
