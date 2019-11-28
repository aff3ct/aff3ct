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
	std::vector<std::vector<Task*>> tasks_sequences;
	std::vector<std::vector<std::shared_ptr<Module>>> modules;

public:
	Chain(const Task &first,                   const size_t n_threads = 1);
	Chain(const Task &first, const Task &last, const size_t n_threads = 1);
	virtual ~Chain() = default;

	void exec(std::function<bool(const std::vector<int>&)> stop_condition);
	void exec(std::function<bool(                       )> stop_condition);
	inline size_t get_n_threads() const;

	template <class C = Module>
	std::vector<C*> get_modules() const;
	std::vector<std::vector<const Module*>> get_modules_per_threads() const;
	std::vector<std::vector<const Module*>> get_modules_per_types  () const;

protected:
	void init_recursive(const Task& current_task, const Task *last = nullptr);
	void _exec(std::function<bool(const std::vector<int>&)> &stop_condition,
	                                    std::vector<Task*> &tasks_sequence);
	void _exec_without_statuses(std::function<bool()> &stop_condition,
	                            std::vector<Task*> &tasks_sequence);
	void duplicate();

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Module/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
