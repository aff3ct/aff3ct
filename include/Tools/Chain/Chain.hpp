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

template <class VTA = std::vector<module::Task*>>
class Sub_sequence_generic
{
public:
	subseq_t type;
	VTA tasks;
	std::vector<size_t> tasks_id;
	size_t id;

	explicit Sub_sequence_generic() : type(subseq_t::STD), id(0) {}
	virtual ~Sub_sequence_generic() = default;
};

using Sub_sequence       = Sub_sequence_generic<std::vector<      module::Task*>>;
using Sub_sequence_const = Sub_sequence_generic<std::vector<const module::Task*>>;

class Chain : Interface_clone
{
protected:
	size_t n_threads;
	std::vector<Generic_node<Sub_sequence>*> sequences;
	std::vector<module::Task*> first_tasks;
	std::vector<module::Task*> last_tasks;
	std::vector<std::vector<std::shared_ptr<module::Module>>> modules;
	std::vector<std::vector<module::Module*>> all_modules;

	std::shared_ptr<std::mutex> mtx_exception;
	std::vector<std::string> prev_exception_messages;
	std::vector<std::string> prev_exception_messages_to_display;
	std::shared_ptr<std::atomic<bool>> force_exit_loop;

	size_t n_tasks;
	bool tasks_inplace;
	bool thread_pinning;
	std::vector<size_t> puids;

public:
	Chain(const module::Task &first,
	      const size_t n_threads = 1,
	      const bool thread_pinning = false,
	      const std::vector<size_t> &puids = {} );
	Chain(const module::Task &first,
	      const module::Task &last,
	      const size_t n_threads = 1,
	      const bool thread_pinning = false,
	      const std::vector<size_t> &puids = {});
	Chain(module::Task &first,
	      const size_t n_threads = 1,
	      const bool thread_pinning = false,
	      const std::vector<size_t> &puids = {},
	      const bool tasks_inplace = true);
	Chain(module::Task &first,
	      module::Task &last,
	      const size_t n_threads = 1,
	      const bool thread_pinning = false,
	      const std::vector<size_t> &puids = {},
	      const bool tasks_inplace = true);

	virtual ~Chain();
	template <class SS, class TA>
	void init(TA &first, TA *last = nullptr);
	template <class SS>
	inline void _init(Generic_node<SS> *root);
	virtual Chain* clone() const;

	void set_thread_pinning(const bool thread_pinning);
	void set_thread_pinning(const bool thread_pinning, const std::vector<size_t> &puids = {});
	bool is_thread_pinning();

	void exec(std::function<bool(const std::vector<int>&)> stop_condition);
	void exec(std::function<bool(                       )> stop_condition);
	int  exec(const size_t tid = 0                                       );
	inline size_t get_n_threads() const;

	template <class C = module::Module>
	std::vector<C*> get_modules(const bool subchain_modules = true) const;
	std::vector<std::vector<module::Module*>> get_modules_per_threads() const;
	std::vector<std::vector<module::Module*>> get_modules_per_types  () const;

	std::vector<std::vector<module::Task*>> get_tasks_per_threads() const;
	std::vector<std::vector<module::Task*>> get_tasks_per_types  () const;

	inline const std::vector<module::Task*>& get_first_tasks() const;
	inline const std::vector<module::Task*>& get_last_tasks() const;

	void export_dot(std::ostream &stream = std::cout) const;

protected:
	template <class SS>
	void delete_tree(Generic_node<SS> *node);

	template <class SS, class TA>
	const module::Task& init_recursive(Generic_node<SS> *cur_subseq,
	                                   size_t &ssid,
	                                   size_t &taid,
	                                   std::vector<TA*> &loops,
	                                   TA& first,
	                                   TA& current_task,
	                                   TA* last = nullptr);

	template <class VTA>
	void export_dot_subsequence(const VTA &subseq,
	                            const subseq_t &subseq_type,
	                            const std::string &subseq_name,
	                            const std::string &tab,
	                                  std::ostream &stream = std::cout) const;

	template <class VTA>
	void export_dot_connections(const VTA &subseq,
	                            const std::string &tab,
	                                  std::ostream &stream = std::cout) const;

	template <class SS>
	void export_dot(Generic_node<SS>* root, std::ostream &stream = std::cout) const;

	template <class SS, class MO>
	void duplicate(const Generic_node<SS> *sequence);

	void _exec(const size_t tid,
	           std::function<bool(const std::vector<int>&)> &stop_condition,
	           Generic_node<Sub_sequence>* sequence);

	void _exec_without_statuses(const size_t tid,
	                            std::function<bool()> &stop_condition,
	                            Generic_node<Sub_sequence>* sequence);

};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Chain/Chain.hxx"
#endif

#endif /* CHAIN_HPP_ */
