/*!
 * \file
 * \brief Class module::Chain.
 */
#ifndef CHAIN_HPP_
#define CHAIN_HPP_

#include <functional>
#include <memory>
#include <vector>

namespace aff3ct
{
namespace module
{
class Task;
class Module;

class Chain
{
protected:
	size_t n_threads;
	std::vector<const Task*> tasks_sequence;
	std::vector<std::vector<std::shared_ptr<Task>>> tasks_sequences;
	std::vector<std::vector<std::shared_ptr<Module>>> modules;

public:
	Chain(const Task &first,                   const size_t n_threads = 1);
	Chain(const Task &first, const Task &last, const size_t n_threads = 1);
	virtual ~Chain() = default;

	void exec(std::function<bool(const std::vector<int>&)> &stop_condition);
	inline size_t get_n_threads() const;

	template <class C = Module>
	std::vector<C*> get_modules() const;

protected:
	void init_recursive(const Task& current_task, const Task *last = nullptr);
	void _exec(std::function<bool(const std::vector<int>&)> &stop_condition,
	                                    std::vector<std::shared_ptr<Task>> &tasks_sequence);
	void duplicate();

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
