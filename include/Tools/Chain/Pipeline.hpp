/*!
 * \file
 * \brief Class tools::Chain_pipeline.
 */
#ifndef CHAIN_PIPELINE_HPP_
#define CHAIN_PIPELINE_HPP_

#include <functional>
#include <cstdint>
#include <memory>
#include <vector>

#include "Tools/Interface/Interface_clone.hpp"
#include "Tools/Chain/Chain.hpp"

namespace aff3ct
{
namespace tools
{

class Pipeline
{
protected:
	Chain sequential_chain;
	std::vector<std::shared_ptr<Chain>> stages;
	std::vector<std::pair<std::vector<std::shared_ptr<module::Adaptor>>,
	                      std::vector<std::shared_ptr<module::Adaptor>>>> adaptors;

public:
	// Pipeline(const module::Task &first,
	//          const std::vector<std::pair<std::vector<const module::Task*>, std::vector<const module::Task*>>> &sep_stages = {},
	//          const std::vector<size_t> &n_threads = {},
	//          const std::vector<bool> &thread_pinning = {},
	//          const std::vector<std::vector<size_t>> &puids = {});
	// Pipeline(const module::Task &first,
	//          const module::Task &last,
	//          const std::vector<std::pair<std::vector<const module::Task*>, std::vector<const module::Task*>>> &sep_stages = {},
	//          const std::vector<size_t> &n_threads = {},
	//          const std::vector<bool> &thread_pinning = {},
	//          const std::vector<std::vector<size_t>> &puids = {});
	Pipeline(const std::vector<module::Task*> &firsts,
	         const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages = {},
	         const std::vector<size_t> &n_threads = {},
	         const std::vector<bool> &thread_pinning = {},
	         const std::vector<std::vector<size_t>> &puids = {}/*,
	         const std::vector<bool> &tasks_inplace = {}*/);
	Pipeline(const std::vector<module::Task*> &firsts,
	         const std::vector<module::Task*> &lasts,
	         const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages = {},
	         const std::vector<size_t> &n_threads = {},
	         const std::vector<bool> &thread_pinning = {},
	         const std::vector<std::vector<size_t>> &puids = {}/*,
	         const std::vector<bool> &tasks_inplace = {}*/);
	Pipeline(module::Task &first,
	         const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages = {},
	         const std::vector<size_t> &n_threads = {},
	         const std::vector<bool> &thread_pinning = {},
	         const std::vector<std::vector<size_t>> &puids = {}/*,
	         const std::vector<bool> &tasks_inplace = {}*/);
	Pipeline(module::Task &first,
	         module::Task &last,
	         const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages = {},
	         const std::vector<size_t> &n_threads = {},
	         const std::vector<bool> &thread_pinning = {},
	         const std::vector<std::vector<size_t>> &puids = {}/*,
	         const std::vector<bool> &tasks_inplace = {}*/);

	virtual ~Pipeline() = default;

	void exec(std::function<bool(const std::vector<int>&)> stop_condition);
	void exec(std::function<bool(                       )> stop_condition);

	std::vector<Chain*> get_stages();
	Chain& operator[](const size_t stage_id);

	template <class C = module::Module>
	std::vector<C*> get_modules(const bool subchain_modules = true) const;
	std::vector<std::vector<module::Module*>> get_modules_per_threads() const;
	std::vector<std::vector<module::Module*>> get_modules_per_types  () const;

	std::vector<std::vector<module::Task*>> get_tasks_per_threads() const;
	std::vector<std::vector<module::Task*>> get_tasks_per_types  () const;

	inline const std::vector<std::vector<module::Task*>>& get_firsts_tasks() const;
	inline const std::vector<std::vector<module::Task*>>& get_lasts_tasks() const;

protected:
	void create_adaptors();

private:
	template <class TA>
	void init(const std::vector<TA*> &firsts,
	          const std::vector<TA*> &lasts,
	          const std::vector<std::pair<std::vector<TA*>, std::vector<TA*>>> &sep_stages = {},
	          const std::vector<size_t> &n_threads = {},
	          const std::vector<bool> &thread_pinning = {},
	          const std::vector<std::vector<size_t>> &puids = {}/*,
	          const std::vector<bool> &tasks_inplace = {}*/);
};
}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "Tools/Chain/Pipeline.hxx"
#endif

#endif /* CHAIN_PIPELINE_HPP_ */
