/*!
 * \file
 * \brief Class tools::Chain.
 */
#ifndef CHAIN_HPP_
#define CHAIN_HPP_

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>

#include "Tools/Interface/Interface_clone.hpp"

namespace aff3ct
{
namespace module
{
class Task;
class Module;
}
namespace tools
{

class Chain : Interface_clone
{
protected:
	size_t n_threads;
	std::vector<std::vector<module::Task*>> tasks_sequences;
	std::vector<std::vector<std::shared_ptr<module::Module>>> modules;

	std::shared_ptr<std::mutex> mtx_exception;
	std::vector<std::string> prev_exception_messages;
	std::vector<std::string> prev_exception_messages_to_display;
	std::shared_ptr<std::atomic<bool>> force_exit_loop;

public:
	Chain(const module::Task &first,                           const size_t n_threads = 1);
	Chain(const module::Task &first, const module::Task &last, const size_t n_threads = 1);
	virtual ~Chain() = default;
	virtual Chain* clone() const;

	void exec(std::function<bool(const std::vector<int>&)> stop_condition);
	void exec(std::function<bool(                       )> stop_condition);
	int  exec(const int tid = 0                                          );
	inline size_t get_n_threads() const;

	template <class C = module::Module>
	std::vector<C*> get_modules(const bool subchain_modules = true) const;
	std::vector<std::vector<const module::Module*>> get_modules_per_threads() const;
	std::vector<std::vector<const module::Module*>> get_modules_per_types  () const;
	inline const std::vector<std::vector<module::Task*>>& get_tasks_sequences() const;
	inline const std::vector<module::Task*>& get_tasks_sequence(const int tid = 0) const;

protected:
	static void init_recursive(std::vector<const module::Task*> &tasks_sequence,
	                           const module::Task& first,
	                           const module::Task& current_task,
	                           const module::Task *last = nullptr);
	void _exec(std::function<bool(const std::vector<int>&)> &stop_condition,
	                                    std::vector<module::Task*> &tasks_sequence);
	void _exec_without_statuses(std::function<bool()> &stop_condition,
	                            std::vector<module::Task*> &tasks_sequence);
	void duplicate(const std::vector<const module::Task*> &tasks_sequence);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Chain/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
