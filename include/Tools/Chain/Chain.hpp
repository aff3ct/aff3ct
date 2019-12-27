/*!
 * \file
 * \brief Class tools::Chain.
 */
#ifndef CHAIN_HPP_
#define CHAIN_HPP_

#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>

#include "Tools/Interface/Interface_clone.hpp"
#include "Tools/Algo/Tree/Generic/Generic_node.hpp"

namespace aff3ct
{
namespace module
{
class Task;
class Module;
}
namespace tools
{

enum class subseq_t : size_t { STD, LOOP, ROUTER };

class Sub_sequence_const
{
public:
	subseq_t type;
	std::vector<const module::Task*> tasks;
	size_t id;

	explicit Sub_sequence_const() : type(subseq_t::STD), id(0) {}
	virtual ~Sub_sequence_const() = default;
};

class Sub_sequence
{
public:
	subseq_t type;
	std::vector<module::Task*> tasks;
	size_t id;

	explicit Sub_sequence() : type(subseq_t::STD), id(0) {}
	virtual ~Sub_sequence() = default;
};

class Chain : Interface_clone
{
protected:
	size_t n_threads;
	std::vector<Generic_node<Sub_sequence>*> sequences;

	std::vector<std::vector<std::vector<module::Task*>>> tasks_sequences;
	std::vector<std::vector<std::shared_ptr<module::Module>>> modules;
	std::vector<subseq_t> subseq_types;

	std::shared_ptr<std::mutex> mtx_exception;
	std::vector<std::string> prev_exception_messages;
	std::vector<std::string> prev_exception_messages_to_display;
	std::shared_ptr<std::atomic<bool>> force_exit_loop;

	size_t n_tasks;
	std::vector<std::vector<size_t>> task_id;

public:
	Chain(const module::Task &first,                           const size_t n_threads = 1);
	Chain(const module::Task &first, const module::Task &last, const size_t n_threads = 1);
	virtual ~Chain();
	virtual Chain* clone() const;

	void exec(std::function<bool(const std::vector<int>&)> stop_condition);
	void exec(std::function<bool(                       )> stop_condition);
	int  exec(const size_t tid = 0                                       );
	inline size_t get_n_threads() const;

	template <class C = module::Module>
	std::vector<C*> get_modules(const bool subchain_modules = true) const;
	std::vector<std::vector<const module::Module*>> get_modules_per_threads() const;
	std::vector<std::vector<const module::Module*>> get_modules_per_types  () const;
	inline const std::vector<std::vector<std::vector<module::Task*>>>& get_tasks_sequences() const;
	inline const std::vector<std::vector<module::Task*>>& get_tasks_sequence(const int tid = 0) const;

	void export_dot(std::ostream &stream = std::cout) const;

protected:
	void export_dot_subsequence(const std::vector<module::Task*> &subseq,
	                            const subseq_t &subseq_type,
	                            const std::string &subseq_name,
	                            const std::string &tab,
	                            std::ostream &stream) const;
	static void init_recursive(std::vector<std::vector<const module::Task*>> &tasks_sequence,
	                           const size_t ssid,
	                           std::vector<subseq_t> &subseq_type,
	                           std::vector<const module::Task*> &loops,
	                           const module::Task& first,
	                           const module::Task& current_task,
	                           const module::Task *last = nullptr);
	void _exec(std::function<bool(const std::vector<int>&)> &stop_condition,
	                                    std::vector<std::vector<module::Task*>> &tasks_sequence);
	void _exec_without_statuses(std::function<bool()> &stop_condition,
	                            std::vector<std::vector<module::Task*>> &tasks_sequence);
	void duplicate(const std::vector<std::vector<const module::Task*>> &tasks_sequence);

	// new ------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------

	template <class SS>
	void delete_tree(Generic_node<SS> *node);

	static void init_recursive_new(Generic_node<Sub_sequence_const> *cur_subseq,
	                               size_t &ssid,
	                               std::vector<const module::Task*> &loops,
	                               const module::Task& first,
	                               const module::Task& current_task,
	                               const module::Task *last = nullptr);

	void export_dot_new_subsequence(const std::vector<module::Task*> &subseq,
	                                const subseq_t &subseq_type,
	                                const std::string &subseq_name,
	                                const std::string &tab,
	                                      std::ostream &stream = std::cout) const;

	void export_dot_new_connections(const std::vector<module::Task*> &subseq,
	                                const std::string &tab,
	                                      std::ostream &stream = std::cout) const;

	void export_dot_new(Generic_node<Sub_sequence>* root, std::ostream &stream = std::cout) const;

	void duplicate_new(const Generic_node<Sub_sequence_const> *sequence);

	void _exec_without_statuses_new(std::function<bool()> &stop_condition, Generic_node<Sub_sequence>* sequence);

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Chain/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
