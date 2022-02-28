#include <set>
#include <thread>
#include <utility>
#include <sstream>
#include <fstream>
#include <cstring>
#include <numeric>
#include <exception>
#include <algorithm>

#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Thread_pinning/Thread_pinning.hpp"
#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Task.hpp"
#include "Module/Socket.hpp"
#include "Module/Probe/Probe.hpp"
#include "Module/Switcher/Switcher.hpp"
#include "Module/Adaptor/Adaptor.hpp"
#include "Tools/Sequence/Sequence.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

Sequence
::Sequence(const std::vector<const module::Task*> &firsts,
           const std::vector<const module::Task*> &lasts,
           const std::vector<const module::Task*> &exclusions,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids)
: n_threads(n_threads),
  sequences(n_threads, nullptr),
  modules(n_threads),
  all_modules(n_threads),
  mtx_exception(new std::mutex()),
  force_exit_loop(new std::atomic<bool>(false)),
  tasks_inplace(false),
  thread_pinning(thread_pinning),
  puids(puids),
  no_copy_mode(true),
  saved_exclusions(exclusions),
  switchers_reset(n_threads),
  auto_stop(true),
  next_round_is_over(n_threads, false),
  cur_task_id(n_threads,0),
  cur_ss(n_threads, nullptr)
{
#ifndef AFF3CT_HWLOC
	if (thread_pinning)
		std::clog << rang::tag::warning << "AFF3CT has not been linked with the 'hwloc' library, the 'thread_pinning' "
		                                   "option of the 'tools::Sequence' will have no effect." << std::endl;
#endif

	if (thread_pinning && puids.size() < n_threads)
	{
		std::stringstream message;
		message << "'puids.size()' has to be greater or equal to 'n_threads' ('puids.size()' = " << puids.size()
		        << " , 'n_threads' = " << n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init<tools::Sub_sequence_const,const module::Task>(firsts, lasts, exclusions);
}

Sequence
::Sequence(const std::vector<const module::Task*> &firsts,
           const std::vector<const module::Task*> &lasts,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids)
: Sequence(firsts, lasts, {}, n_threads, thread_pinning, puids)
{
}

Sequence
::Sequence(const std::vector<const module::Task*> &firsts,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids)
: Sequence(firsts, {}, {}, n_threads, thread_pinning, puids)
{
}

Sequence
::Sequence(const module::Task &first,
           const module::Task &last,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids)
: Sequence({&first}, {&last}, n_threads, thread_pinning, puids)
{
}

Sequence
::Sequence(const module::Task &first,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids)
: Sequence({&first}, n_threads, thread_pinning, puids)
{
}

std::vector<const module::Task*> exclusions_convert_to_const(const std::vector<module::Task*> &exclusions)
{
	std::vector<const module::Task*> exclusions_const;
	for (auto exception : exclusions)
		exclusions_const.push_back(exception);
	return exclusions_const;
}

Sequence
::Sequence(const std::vector<module::Task*> &firsts,
           const std::vector<module::Task*> &lasts,
           const std::vector<module::Task*> &exclusions,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids,
           const bool tasks_inplace)
: n_threads(n_threads),
  sequences(n_threads, nullptr),
  modules(tasks_inplace ? n_threads -1 : n_threads),
  all_modules(n_threads),
  mtx_exception(new std::mutex()),
  force_exit_loop(new std::atomic<bool>(false)),
  tasks_inplace(tasks_inplace),
  thread_pinning(thread_pinning),
  puids(puids),
  no_copy_mode(true),
  saved_exclusions(exclusions_convert_to_const(exclusions)),
  switchers_reset(n_threads),
  auto_stop(true),
  next_round_is_over(n_threads, false),
  cur_task_id(n_threads,0),
  cur_ss(n_threads, nullptr)
{
	if (thread_pinning && puids.size() < n_threads)
	{
		std::stringstream message;
		message << "'puids.size()' has greater or equal to 'n_threads' ('puids.size()' = " << puids.size()
		        << " , 'n_threads' = " << n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (tasks_inplace)
		this->init<tools::Sub_sequence,module::Task>(firsts, lasts, exclusions);
	else
	{
		std::vector<const module::Task*> firsts_bis;
		for (auto first : firsts) firsts_bis.push_back(first);
		std::vector<const module::Task*> lasts_bis;
		for (auto last : lasts) lasts_bis.push_back(last);
		std::vector<const module::Task*> exclusions_bis;
		for (auto exception : exclusions) exclusions_bis.push_back(exception);
		this->init<tools::Sub_sequence_const,const module::Task>(firsts_bis, lasts_bis, exclusions_bis);
	}
}

Sequence
::Sequence(const std::vector<module::Task*> &firsts,
           const std::vector<module::Task*> &lasts,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids,
           const bool tasks_inplace)
: Sequence(firsts, lasts, {}, n_threads, thread_pinning, puids, tasks_inplace)
{
}

Sequence
::Sequence(const std::vector<module::Task*> &firsts,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids,
           const bool tasks_inplace)
: Sequence(firsts, {}, {}, n_threads, thread_pinning, puids, tasks_inplace)
{
}

Sequence
::Sequence(module::Task &first,
           module::Task &last,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids,
           const bool tasks_inplace)
: Sequence({&first}, {&last}, n_threads, thread_pinning, puids, tasks_inplace)
{
}

Sequence
::Sequence(module::Task &first,
           const size_t n_threads,
           const bool thread_pinning,
           const std::vector<size_t> &puids,
           const bool tasks_inplace)
: Sequence({&first}, n_threads, thread_pinning, puids, tasks_inplace)
{
}

Sequence
::~Sequence()
{
	std::vector<Digraph_node<Sub_sequence>*> already_deleted_nodes;
	for (auto s : this->sequences)
		this->delete_tree(s, already_deleted_nodes);
}

template <class SS, class TA>
void Sequence
::init(const std::vector<TA*> &firsts, const std::vector<TA*> &lasts, const std::vector<TA*> &exclusions)
{
	if (this->is_thread_pinning())
		Thread_pinning::pin(this->puids[0]);

	if (firsts.size() == 0)
	{
		std::stringstream message;
		message << "'firsts.size()' has to be strictly greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->n_threads == 0)
	{
		std::stringstream message;
		message << "'n_threads' has to be strictly greater than 0.";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto exclusion : exclusions)
	{
		if (std::find(firsts.begin(), firsts.end(), exclusion) != firsts.end())
		{
			std::stringstream message;
			message << "'exclusion' can't be contained in the 'firsts' vector ("
			        << "'exclusion'"                 << " = " << +exclusion            << ", "
			        << "'exclusion->get_name()'"     << " = " << exclusion->get_name() << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (std::find(lasts.begin(), lasts.end(), exclusion) != lasts.end())
		{
			std::stringstream message;
			message << "'exclusion' can't be contained in the 'lasts' vector ("
			        << "'exclusion'"                 << " = " << +exclusion            << ", "
			        << "'exclusion->get_name()'"     << " = " << exclusion->get_name() << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	auto root = new Digraph_node<SS>({}, {}, nullptr, 0);
	root->set_contents(nullptr);
	size_t ssid = 0, taid = 0;
	std::vector<TA*> switchers;
	std::vector<std::pair<TA*,Digraph_node<SS>*>> selectors;
	std::vector<TA*> real_lasts;

	this->lasts_tasks_id.clear();
	this->firsts_tasks_id.clear();
	auto last_subseq = root;
	std::map<TA*,unsigned> in_sockets_feed;
	for (auto first : firsts)
	{
		std::map<TA*,std::pair<Digraph_node<SS>*,size_t>> task_subseq;
		auto contents = last_subseq->get_contents();
		this->firsts_tasks_id.push_back(contents ? contents->tasks_id[contents->tasks_id.size() -1] : 0);
		last_subseq = this->init_recursive<SS,TA>(last_subseq,
		                                          ssid,
		                                          taid,
		                                          selectors,
		                                          switchers,
		                                          *first,
		                                          *first,
		                                          lasts,
		                                          exclusions,
		                                          this->lasts_tasks_id,
		                                          real_lasts,
		                                          in_sockets_feed,
		                                          task_subseq);
	}

	std::stringstream real_lasts_ss;
	for (size_t rl = 0; rl < real_lasts.size(); rl++)
		real_lasts_ss << "'real_lasts" << "[" << rl << "]'"             << " = " << +real_lasts[rl] << ", "
		              << "'real_lasts" << "[" << rl << "]->get_name()'" << " = " <<  real_lasts[rl]->get_name()
		              << ((rl < real_lasts.size() -1) ? ", " : "");

	for (auto last : lasts)
	{
		if (std::find(real_lasts.begin(), real_lasts.end(), last) == real_lasts.end())
		{
			std::stringstream message;
			message << "'last' is not contained in the 'real_lasts[" << real_lasts.size() << "]' vector ("
			        << "'last'"                 << " = " << +last            << ", "
			        << "'last->get_name()'"     << " = " << last->get_name() << ", "
			        << real_lasts_ss.str() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->n_tasks = taid;
	this->_init<SS>(root);
	this->update_firsts_and_lasts_tasks();
	this->gen_processes();
	this->donners = get_modules<tools::Interface_is_done>(true);

	for (size_t tid = 0; tid < this->n_threads; tid++)
		for (auto &mdl : this->all_modules[tid])
			if (auto swi = dynamic_cast<module::Switcher*>(mdl))
				this->switchers_reset[tid].push_back(dynamic_cast<tools::Interface_reset*>(swi));

	for (size_t tid = 0; tid < this->sequences.size(); tid++)
		this->cur_ss[tid] = this->sequences[tid];
}

Sequence* Sequence
::clone() const
{
	auto c = new Sequence(*this);

	c->tasks_inplace = false;
	c->modules.resize(c->get_n_threads());

	std::vector<const module::Task*> firsts_tasks;
	for (auto ta : this->get_firsts_tasks()[0])
		firsts_tasks.push_back(ta);

	std::vector<const module::Task*> lasts_tasks;
	for (auto ta : this->get_lasts_tasks()[0])
		lasts_tasks.push_back(ta);

	c->init<tools::Sub_sequence_const,const module::Task>(firsts_tasks, lasts_tasks, this->saved_exclusions);
	c->mtx_exception.reset(new std::mutex());
	c->force_exit_loop.reset(new std::atomic<bool>(false));
	return c;
}

void Sequence
::set_thread_pinning(const bool thread_pinning, const std::vector<size_t> &puids)
{
	if (thread_pinning && puids.size() < n_threads)
	{
		std::stringstream message;
		message << "'puids.size()' has greater or equal to 'n_threads' ('puids.size()' = " << puids.size()
		        << " , 'n_threads' = " << n_threads << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->thread_pinning = thread_pinning;
	this->puids = puids;
}

bool Sequence
::is_thread_pinning()
{
	return this->thread_pinning;
}

std::vector<std::vector<module::Module*>> Sequence
::get_modules_per_threads() const
{
	std::vector<std::vector<module::Module*>> modules_per_threads(this->all_modules.size());
	size_t tid = 0;
	for (auto &e : this->all_modules)
	{
		for (auto &ee : e)
			modules_per_threads[tid].push_back(ee);
		tid++;
	}
	return modules_per_threads;
}

std::vector<std::vector<module::Module*>> Sequence
::get_modules_per_types() const
{
	std::vector<std::vector<module::Module*>> modules_per_types(this->all_modules[0].size());
	for (auto &e : this->all_modules)
	{
		size_t mid = 0;
		for (auto &ee : e)
			modules_per_types[mid++].push_back(ee);
	}
	return modules_per_types;
}

std::vector<std::vector<module::Task*>> Sequence
::get_tasks_per_threads() const
{
	std::vector<std::vector<module::Task*>> tasks_per_threads(this->n_threads);

	std::function<void(Digraph_node<Sub_sequence>*, const size_t,
		               std::vector<Digraph_node<Sub_sequence>*> &)> get_tasks_recursive =
		[&](Digraph_node<Sub_sequence>* cur_ss, const size_t tid,
			std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_ss) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_ss);
				tasks_per_threads[tid].insert(tasks_per_threads[tid].end(),
				                              cur_ss->get_c()->tasks.begin(),
				                              cur_ss->get_c()->tasks.end());

				for (auto c : cur_ss->get_children())
					get_tasks_recursive(c, tid, already_parsed_nodes);
			}
		};

	for (size_t tid = 0; tid < this->n_threads; tid++)
	{
		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
		get_tasks_recursive(this->sequences[tid], tid, already_parsed_nodes);
	}

	return tasks_per_threads;
}

std::vector<std::vector<module::Task*>> Sequence
::get_tasks_per_types() const
{
	std::vector<std::vector<module::Task*>> tasks_per_types(this->n_tasks);

	std::function<void(Digraph_node<Sub_sequence>*, size_t&,
		               std::vector<Digraph_node<Sub_sequence>*>&)> get_tasks_recursive =
		[&](Digraph_node<Sub_sequence>* cur_ss, size_t &mid,
			std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (std::find(already_parsed_nodes.begin(),
			    already_parsed_nodes.end(),
			    cur_ss) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_ss);
				for (auto &t : cur_ss->get_c()->tasks)
					tasks_per_types[mid++].push_back(t);

				for (auto c : cur_ss->get_children())
					get_tasks_recursive(c, mid, already_parsed_nodes);
			}
		};

	for (size_t tid = 0; tid < this->n_threads; tid++)
	{
		size_t mid = 0;
		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
		get_tasks_recursive(this->sequences[tid], mid, already_parsed_nodes);
	}

	return tasks_per_types;
}

bool Sequence
::is_done() const
{
	for (auto donner : this->donners)
		if (donner->is_done())
			return true;
	return false;
}

void Sequence
::_exec(const size_t tid,
        std::function<bool(const std::vector<const int*>&)> &stop_condition,
        Digraph_node<Sub_sequence>* sequence)
{
	if (this->is_thread_pinning())
		Thread_pinning::pin(this->puids[tid]);

	std::function<void(Digraph_node<Sub_sequence>*, std::vector<const int*>&)> exec_sequence =
		[&exec_sequence](Digraph_node<Sub_sequence>* cur_ss, std::vector<const int*>& statuses)
		{
			auto type = cur_ss->get_c()->type;
			auto &tasks_id = cur_ss->get_c()->tasks_id;
			auto &processes = cur_ss->get_c()->processes;

			if (type == subseq_t::COMMUTE)
			{
				statuses[tasks_id[0]] = processes[0]();
				const int path = statuses[tasks_id[0]][0];
				if (cur_ss->get_children().size() > (size_t)path)
					exec_sequence(cur_ss->get_children()[path], statuses);
			}
			else
			{
				for (size_t p = 0; p < processes.size(); p++)
					statuses[tasks_id[p]] = processes[p]();
				for (auto c : cur_ss->get_children())
					exec_sequence(c, statuses);
			}
		};

	std::vector<const int*> statuses(this->n_tasks, nullptr);
	try
	{
		do
		{
			// force switchers reset to reinitialize the path to the last input socket
			for (size_t s = 0; s < this->switchers_reset[tid].size(); s++)
				this->switchers_reset[tid][s]->reset();

			std::fill(statuses.begin(), statuses.end(), nullptr);
			try
			{
				exec_sequence(sequence, statuses);
			}
			catch (tools::processing_aborted const&)
			{
				// do nothing, this is normal
			}
		}
		while (!*force_exit_loop && !stop_condition(statuses));
	}
	catch (tools::waiting_canceled const&)
	{
		// do nothing, this is normal
	}
	catch (std::exception const& e)
	{
		*force_exit_loop = true;

		this->mtx_exception->lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(this->prev_exception_messages.begin(), this->prev_exception_messages.end(), msg) ==
			this->prev_exception_messages.end())
		{
			this->prev_exception_messages.push_back(msg); // save only the function signature
			this->prev_exception_messages_to_display.push_back(e.what()); // with backtrace if debug mode
		}
		else if (std::strlen(e.what()) > this->prev_exception_messages_to_display.back().size())
			this->prev_exception_messages_to_display[prev_exception_messages_to_display.size() -1] = e.what();

		this->mtx_exception->unlock();
	}

	if (this->is_thread_pinning())
		Thread_pinning::unpin();
}

void Sequence
::_exec_without_statuses(const size_t tid,
                         std::function<bool()> &stop_condition,
                         Digraph_node<Sub_sequence>* sequence)
{
	if (this->is_thread_pinning())
		Thread_pinning::pin(this->puids[tid]);

	std::function<void(Digraph_node<Sub_sequence>*)> exec_sequence =
		[&exec_sequence](Digraph_node<Sub_sequence>* cur_ss)
		{
			auto type = cur_ss->get_c()->type;
			auto &processes = cur_ss->get_c()->processes;

			if (type == subseq_t::COMMUTE)
			{
				const int path = processes[0]()[0];
				if (cur_ss->get_children().size() > (size_t)path)
					exec_sequence(cur_ss->get_children()[path]);
			}
			else
			{
				for (auto &process : processes)
					process();
				for (auto c : cur_ss->get_children())
					exec_sequence(c);
			}
		};

	try
	{
		do
		{
			// force switchers reset to reinitialize the path to the last input socket
			for (size_t s = 0; s < this->switchers_reset[tid].size(); s++)
				this->switchers_reset[tid][s]->reset();

			try
			{
				exec_sequence(sequence);
			}
			catch (tools::processing_aborted const&)
			{
				// do nothing, this is normal
			}
		}
		while (!*force_exit_loop && !stop_condition());
	}
	catch (tools::waiting_canceled const&)
	{
		// do nothing, this is normal
	}
	catch (std::exception const& e)
	{
		*force_exit_loop = true;

		this->mtx_exception->lock();

		auto save = tools::exception::no_backtrace;
		tools::exception::no_backtrace = true;
		std::string msg = e.what(); // get only the function signature
		tools::exception::no_backtrace = save;

		if (std::find(this->prev_exception_messages.begin(), this->prev_exception_messages.end(), msg) ==
			this->prev_exception_messages.end())
		{
			this->prev_exception_messages.push_back(msg); // save only the function signature
			this->prev_exception_messages_to_display.push_back(e.what()); // with backtrace if debug mode
		}
		else if (std::strlen(e.what()) > this->prev_exception_messages_to_display.back().size())
			this->prev_exception_messages_to_display[prev_exception_messages_to_display.size() -1] = e.what();

		this->mtx_exception->unlock();
	}

	if (this->is_thread_pinning())
		Thread_pinning::unpin();
}

void Sequence
::exec(std::function<bool(const std::vector<const int*>&)> stop_condition)
{
	if (this->is_no_copy_mode())
		this->gen_processes(true);

	std::function<bool(const std::vector<const int*>&)> real_stop_condition;
	if (this->auto_stop)
		real_stop_condition = [this, stop_condition](const std::vector<const int*>& statuses)
		{
			bool res = stop_condition(statuses);
			return res || this->is_done();
		};
	else
		real_stop_condition = stop_condition;

	std::vector<std::thread> threads(n_threads);
	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid] = std::thread(&Sequence::_exec, this, tid, std::ref(real_stop_condition),
		                           std::ref(this->sequences[tid]));

	this->_exec(0, real_stop_condition, this->sequences[0]);

	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid].join();

	if (this->is_no_copy_mode())
	{
		this->reset_no_copy_mode();
		this->gen_processes(false);
	}

	if (!this->prev_exception_messages_to_display.empty())
	{
		*force_exit_loop = false;
		throw std::runtime_error(this->prev_exception_messages_to_display.back());
	}
}

void Sequence
::exec(std::function<bool()> stop_condition)
{
	if (this->is_no_copy_mode())
		this->gen_processes(true);

	std::function<bool()> real_stop_condition;
	if (this->auto_stop)
		real_stop_condition = [this, stop_condition]()
		{
			bool res = stop_condition();
			return res || this->is_done();
		};
	else
		real_stop_condition = stop_condition;

	std::vector<std::thread> threads(n_threads);
	for (size_t tid = 1; tid < n_threads; tid++)
	{
		threads[tid] = std::thread(&Sequence::_exec_without_statuses, this, tid, std::ref(real_stop_condition),
		                           std::ref(this->sequences[tid]));
	}

	this->_exec_without_statuses(0, real_stop_condition, this->sequences[0]);

	for (size_t tid = 1; tid < n_threads; tid++)
		threads[tid].join();

	if (this->is_no_copy_mode())
	{
		this->reset_no_copy_mode();
		this->gen_processes(false);
	}

	if (!this->prev_exception_messages_to_display.empty())
	{
		*force_exit_loop = false;
		throw std::runtime_error(this->prev_exception_messages_to_display.back());
	}
}

void Sequence
::exec()
{
	this->exec([]() { return false; });
}

void Sequence
::exec_seq(const size_t tid, const int frame_id)
{
	if (tid >= this->sequences.size())
	{
		std::stringstream message;
		message << "'tid' has to be smaller than 'sequences.size()' ('tid' = " << tid
		        << ", 'sequences.size()' = " << this->sequences.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::function<void(Digraph_node<Sub_sequence>*)> exec_sequence =
		[&exec_sequence, frame_id](Digraph_node<Sub_sequence>* cur_ss)
		{
			auto type = cur_ss->get_c()->type;
			auto &tasks = cur_ss->get_c()->tasks;
			if (type == subseq_t::COMMUTE)
			{
				const int path = tasks[0]->exec(frame_id)[0];
				if (cur_ss->get_children().size() > (size_t)path)
					exec_sequence(cur_ss->get_children()[path]);
			}
			else
			{
				for (size_t ta = 0; ta < tasks.size(); ta++)
					tasks[ta]->exec(frame_id);
				for (auto c : cur_ss->get_children())
					exec_sequence(c);
			}
		};

	exec_sequence(this->sequences[tid]);
}

module::Task* Sequence
::exec_step(const size_t tid, const int frame_id)
{
	if (tid >= this->sequences.size())
	{
		std::stringstream message;
		message << "'tid' has to be smaller than 'sequences.size()' ('tid' = " << tid
		        << ", 'sequences.size()' = " << this->sequences.size() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	module::Task* executed_task = nullptr;
	if (this->next_round_is_over[tid])
	{
		this->next_round_is_over[tid] = false;
		this->cur_ss[tid] = this->sequences[tid];
		this->cur_task_id[tid] = 0;
	}
	else
	{
		executed_task = this->cur_ss[tid]->get_c()->tasks[cur_task_id[tid]];
		const std::vector<int>& ret = executed_task->exec(frame_id);

		auto type = this->cur_ss[tid]->get_c()->type;
		if (type == subseq_t::COMMUTE)
		{
			const size_t path = (size_t)ret[0];
			if (this->cur_ss[tid]->get_children().size() > path)
			{
				this->cur_ss[tid] = this->cur_ss[tid]->get_children()[path];
				this->cur_task_id[tid] = 0;

				if (this->cur_ss[tid]->get_c()->tasks.size() == 0)
				{
					// skip nodes without tasks if any
					while (this->cur_ss[tid]->get_children().size() > 0)
					{
						this->cur_ss[tid] = this->cur_ss[tid]->get_children()[0];
						this->cur_task_id[tid] = 0;
						if (this->cur_ss[tid]->get_c() && this->cur_ss[tid]->get_c()->tasks.size() > 0)
							break;
					}
					if (this->cur_task_id[tid] >= this->cur_ss[tid]->get_c()->tasks.size())
						this->next_round_is_over[tid] = true;
				}
			}
			else
			{
				std::stringstream message;
				message << "This should never happen ('path' = " << path
				        << ", 'cur_ss[tid]->get_children().size()' = " << this->cur_ss[tid]->get_children().size()
				        << ", 'tid' = " << tid << ").";
				throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
			}
		}
		else
		{
			this->cur_task_id[tid]++;
			if (this->cur_task_id[tid] == (this->cur_ss[tid]->get_c()->tasks.size()))
			{
				// skip nodes without tasks if any
				while (this->cur_ss[tid]->get_children().size() > 0)
				{
					this->cur_ss[tid] = this->cur_ss[tid]->get_children()[0];
					this->cur_task_id[tid] = 0;
					if (this->cur_ss[tid]->get_c() && this->cur_ss[tid]->get_c()->tasks.size() > 0)
						break;
				}
				if (this->cur_task_id[tid] >= this->cur_ss[tid]->get_c()->tasks.size())
					this->next_round_is_over[tid] = true;
			}
		}
	}

	return executed_task;
}

template <class SS, class TA>
Digraph_node<SS>* Sequence
::init_recursive(Digraph_node<SS> *cur_subseq,
                 size_t &ssid,
                 size_t &taid,
                 std::vector<std::pair<TA*,Digraph_node<SS>*>> &selectors,
                 std::vector<TA*> &switchers,
                 TA &first,
                 TA &current_task,
                 const std::vector<TA*> &lasts,
                 const std::vector<TA*> &exclusions,
                 std::vector<size_t> &real_lasts_id,
                 std::vector<TA*> &real_lasts,
                 std::map<TA*,unsigned> &in_sockets_feed,
                 std::map<TA*,std::pair<Digraph_node<SS>*,size_t>> &task_subseq)
{
	if (this->tasks_inplace && !current_task.is_autoalloc())
	{
		std::stringstream message;
		message << "When 'tasks_inplace' is set to true 'current_task' should be in autoalloc mode ("
		        << "'current_task.get_name()'"              << " = " << current_task.get_name()              << ", "
		        << "'current_task.get_module().get_name()'" << " = " << current_task.get_module().get_name() << ").";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (dynamic_cast<const module::Adaptor*>(&current_task.get_module()) && !this->tasks_inplace)
	{
		std::stringstream message;
		message << "'module::Adaptor' objects are not supported if 'tasks_inplace' is set to false.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	auto it = std::find(real_lasts.begin(), real_lasts.end(), &current_task);
	if (it != real_lasts.end())
	{
		real_lasts.erase(it);
		auto dist = std::distance(real_lasts.begin(), it);
		real_lasts_id.erase(real_lasts_id.begin() + dist);
	}

	if (cur_subseq->get_contents() == nullptr)
	{
		cur_subseq->set_contents(new SS());
		cur_subseq->get_c()->id = ssid++;
	}

	bool is_last = true;
	Digraph_node<SS>* last_subseq = nullptr;

	if (auto switcher = dynamic_cast<const module::Switcher*>(&current_task.get_module()))
	{
		const auto current_task_name = current_task.get_name();
		if (current_task_name == "commute") // ---------------------------------------------------------------- COMMUTE
		{
			if (std::find(switchers.begin(), switchers.end(), &current_task) == switchers.end())
			{
				switchers.push_back(&current_task);
				auto node_switcher = new Digraph_node<SS>({cur_subseq}, {}, nullptr, cur_subseq->get_depth() +1);

				node_switcher->set_contents(new SS());
				node_switcher->get_c()->tasks.push_back(&current_task);
				node_switcher->get_c()->tasks_id.push_back(taid++);
				node_switcher->get_c()->type = subseq_t::COMMUTE;
				node_switcher->get_c()->id = ssid++;

				cur_subseq->add_child(node_switcher);

				for (size_t sdo = 0; sdo < switcher->get_n_data_sockets(); sdo++)
				{
					auto node_switcher_son = new Digraph_node<SS>({node_switcher}, {}, nullptr,
						node_switcher->get_depth() +1);

					node_switcher_son->set_contents(new SS());
					node_switcher_son->get_c()->id = ssid++;

					node_switcher->add_child(node_switcher_son);

					auto &bss = (*switcher)[module::swi::tsk::commute].sockets[sdo+2]->get_bound_sockets();
					for (auto bs : bss)
					{
						if (bs != nullptr)
						{
							auto &t = bs->get_task();
							if (std::find(exclusions.begin(), exclusions.end(), &t) == exclusions.end())
							{
								if (task_subseq.find(&t) == task_subseq.end() || task_subseq[&t].second < ssid)
									task_subseq[&t] = {node_switcher_son, ssid};

								in_sockets_feed.find(&t) != in_sockets_feed.end() ? in_sockets_feed[&t]++ :
								                                                    in_sockets_feed[&t] = 1;
								bool t_is_select = dynamic_cast<const module::Switcher*>(&(t.get_module())) &&
								                   t.get_name() == "select";
								if ((!t_is_select && in_sockets_feed[&t] >= t.get_n_input_sockets() - t.get_n_static_input_sockets()) ||
								    ( t_is_select && t.is_last_input_socket(*bs)))
								{
									is_last = false;
									last_subseq = Sequence::init_recursive<SS,TA>(task_subseq[&t].first,
										task_subseq[&t].second, taid, selectors, switchers, first, t, lasts, exclusions,
										real_lasts_id, real_lasts, in_sockets_feed, task_subseq);
								}
								else
								{
									if (dynamic_cast<const module::Switcher*>(&t.get_module()) &&
										t.get_name() == "select")
									{
										Digraph_node<SS>* node_selector = nullptr;
										for (auto &sel : selectors)
											if (sel.first == &t)
											{
												node_selector = sel.second;
												break;
											}

										if (!node_selector)
										{
											node_selector = new Digraph_node<SS>({node_switcher_son}, {}, nullptr,
												node_switcher_son->get_depth() +1);
											selectors.push_back({&t, node_selector});
										}
										else
											node_selector->add_father(node_switcher_son);
										node_switcher_son->add_child(node_selector);
									}
								}
							}
						}
					}
				}
				// exception for the status socket
				auto &bss = (*switcher)[module::swi::tsk::commute].sockets[switcher->get_n_data_sockets()+2]
					->get_bound_sockets();
				for (auto bs : bss)
				{
					if (bs != nullptr)
					{
						auto &t = bs->get_task();
						if (std::find(exclusions.begin(), exclusions.end(), &t) == exclusions.end())
						{
							if (task_subseq.find(&t) == task_subseq.end() || task_subseq[&t].second < ssid)
								task_subseq[&t] = {node_switcher, ssid};

							in_sockets_feed.find(&t) != in_sockets_feed.end() ? in_sockets_feed[&t]++ :
							                                                    in_sockets_feed[&t] = 1;
							bool t_is_select = dynamic_cast<const module::Switcher*>(&(t.get_module())) &&
							                   t.get_name() == "select";
							if ((!t_is_select && in_sockets_feed[&t] >= t.get_n_input_sockets() - t.get_n_static_input_sockets()) ||
							    ( t_is_select && t.is_last_input_socket(*bs)))
							{
								is_last = false;
								last_subseq = Sequence::init_recursive<SS,TA>(task_subseq[&t].first,
									task_subseq[&t].second, taid, selectors, switchers, first, t, lasts, exclusions,
									real_lasts_id, real_lasts, in_sockets_feed, task_subseq);
							}
						}
					}
				}
			}
		}
		else if (current_task_name == "select") // ------------------------------------------------------------- SELECT
		{
			Digraph_node<SS>* node_selector = nullptr;

			for (auto &sel : selectors)
				if (sel.first == &current_task)
				{
					node_selector = sel.second;
					break;
				}

			if (!node_selector)
			{
				node_selector = new Digraph_node<SS>({cur_subseq}, {}, nullptr, cur_subseq->get_depth() +1);
				selectors.push_back({&current_task, node_selector});
			}

			node_selector->set_contents(new SS());
			node_selector->get_c()->tasks.push_back(&current_task);
			node_selector->get_c()->tasks_id.push_back(taid++);
			node_selector->get_c()->type = subseq_t::SELECT;
			node_selector->get_c()->id = ssid++;

			cur_subseq->add_child(node_selector);

			auto node_selector_son = new Digraph_node<SS>({node_selector}, {}, nullptr, node_selector->get_depth() +1);

			node_selector_son->set_contents(new SS());
			node_selector_son->get_c()->id = ssid++;

			node_selector->add_child(node_selector_son);

			for (auto &s : current_task.sockets)
			{
				if (current_task.get_socket_type(*s) == module::socket_t::SOUT)
				{
					auto bss = s->get_bound_sockets();
					for (auto bs : bss)
					{
						if (bs != nullptr)
						{
							auto &t = bs->get_task();
							if (std::find(exclusions.begin(), exclusions.end(), &t) == exclusions.end())
							{
								if (task_subseq.find(&t) == task_subseq.end() || task_subseq[&t].second < ssid)
									task_subseq[&t] = {node_selector_son, ssid};

								in_sockets_feed.find(&t) != in_sockets_feed.end() ? in_sockets_feed[&t]++ :
								                                                    in_sockets_feed[&t] = 1;
								bool t_is_select = dynamic_cast<const module::Switcher*>(&(t.get_module())) &&
								                   t.get_name() == "select";

								if ((!t_is_select && in_sockets_feed[&t] >= t.get_n_input_sockets() - t.get_n_static_input_sockets()) ||
								    ( t_is_select && t.is_last_input_socket(*bs)))
								{
									is_last = false;
									last_subseq = Sequence::init_recursive<SS,TA>(task_subseq[&t].first,
										task_subseq[&t].second, taid, selectors, switchers, first, t, lasts, exclusions,
										real_lasts_id, real_lasts, in_sockets_feed, task_subseq);
								}
								else
								{
									if (dynamic_cast<const module::Switcher*>(&t.get_module()) &&
										t.get_name() == "select")
									{
										Digraph_node<SS>* node_selector = nullptr;
										for (auto &sel : selectors)
											if (sel.first == &t)
											{
												node_selector = sel.second;
												break;
											}

										if (!node_selector)
										{
											node_selector = new Digraph_node<SS>({node_selector_son}, {}, nullptr,
												node_selector_son->get_depth() +1);
											selectors.push_back({&t, node_selector});
										}
										else
											node_selector->add_father(node_selector_son);
										node_selector_son->add_child(node_selector);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else // --------------------------------------------------------------------------------------------- STANDARD CASE
	{
		cur_subseq->get_c()->tasks.push_back(&current_task);
		cur_subseq->get_c()->tasks_id.push_back(taid++);

		if (std::find(lasts.begin(), lasts.end(), &current_task) == lasts.end())
		{
			for (auto &s : current_task.sockets)
			{
				if (current_task.get_socket_type(*s) == module::socket_t::SOUT)
				{
					auto bss = s->get_bound_sockets();
					for (auto &bs : bss)
					{
						if (bs != nullptr)
						{
							auto &t = bs->get_task();
							if (std::find(exclusions.begin(), exclusions.end(), &t) == exclusions.end())
							{
								if (task_subseq.find(&t) == task_subseq.end() || task_subseq[&t].second < ssid)
									task_subseq[&t] = {cur_subseq, ssid};

								in_sockets_feed.find(&t) != in_sockets_feed.end() ? in_sockets_feed[&t]++ :
								                                                    in_sockets_feed[&t] = 1;
								bool t_is_select = dynamic_cast<const module::Switcher*>(&(t.get_module())) &&
								                   t.get_name() == "select";
								if ((!t_is_select && in_sockets_feed[&t] >= t.get_n_input_sockets() - t.get_n_static_input_sockets()) ||
								    ( t_is_select && t.is_last_input_socket(*bs)))
								{
									is_last = false;
									last_subseq = Sequence::init_recursive<SS,TA>(task_subseq[&t].first,
										task_subseq[&t].second, taid, selectors, switchers, first, t, lasts, exclusions,
										real_lasts_id, real_lasts, in_sockets_feed, task_subseq);
								}
								else
								{
									if (dynamic_cast<const module::Switcher*>(&t.get_module()) &&
										t.get_name() == "select")
									{
										Digraph_node<SS>* node_selector = nullptr;
										for (auto &sel : selectors)
											if (sel.first == &t)
											{
												node_selector = sel.second;
												break;
											}

										if (!node_selector)
										{
											node_selector = new Digraph_node<SS>({cur_subseq}, {}, nullptr,
												cur_subseq->get_depth() +1);
											selectors.push_back({&t, node_selector});
										}
										else
											node_selector->add_father(cur_subseq);
										cur_subseq->add_child(node_selector);
									}
								}
							}
						}
					}
				}
				else if (current_task.get_socket_type(*s) == module::socket_t::SIN)
				{
					if (s->get_bound_sockets().size() > 1)
					{
						std::stringstream message;
						message << "'s->get_bound_sockets().size()' has to be smaller or equal to 1 ("
						        << "'s->get_bound_sockets().size()'"         << " = " << s->get_bound_sockets().size() << ", "
						        << "'get_socket_type(*s)'"                   << " = " << "socket_t::SIN"               << ", "
						        << "'s->get_name()'"                         << " = " << s->get_name()                 << ", "
						        << "'s->get_task().get_name()'"              << " = " << s->get_task().get_name()      << ", "
						        << "'s->get_task().get_module().get_name()'" << " = " << s->get_task().get_module().get_name()
						        << ").";
						throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
					}
				}
			}
		}
	}

	if (is_last && std::find(real_lasts.begin(), real_lasts.end(), &current_task) == real_lasts.end())
	{
		real_lasts.push_back(&current_task);
		real_lasts_id.push_back(cur_subseq->get_contents()->tasks_id.back());
	}

	if (last_subseq)
		return last_subseq;
	else
		return cur_subseq;
}

template <class SS, class MO>
void Sequence
::duplicate(const Digraph_node<SS> *sequence)
{
	std::set<MO*> modules_set;

	std::function<void(const Digraph_node<SS>*, std::vector<const Digraph_node<SS>*> &)> collect_modules_list;
	collect_modules_list = [&](const Digraph_node<SS> *node, std::vector<const Digraph_node<SS>*> &already_parsed_nodes)
	{
		if (node != nullptr &&
		    std::find(already_parsed_nodes.begin(), already_parsed_nodes.end(), node) == already_parsed_nodes.end())
		{
			already_parsed_nodes.push_back(node);
			if (node->get_c())
				for (auto ta : node->get_c()->tasks)
					modules_set.insert(&ta->get_module());
			for (auto c : node->get_children())
				collect_modules_list(c, already_parsed_nodes);
		}
	};
	std::vector<const Digraph_node<SS>*> already_parsed_nodes;
	collect_modules_list(sequence, already_parsed_nodes);

	std::vector<MO*> modules_vec;
	for (auto m : modules_set)
		modules_vec.push_back(m);

	// clone the modules
	for (size_t tid = 0; tid < this->n_threads - (this->tasks_inplace ? 1 : 0); tid++)
	{
		if (this->is_thread_pinning())
		{
			const auto real_tid = tid + (this->tasks_inplace ? 1 : 0);
			Thread_pinning::pin(this->puids[real_tid]);
		}

		this->modules[tid].resize(modules_vec.size());
		this->all_modules[tid + (this->tasks_inplace ? 1 : 0)].resize(modules_vec.size());
		for (size_t m = 0; m < modules_vec.size(); m++)
		{
			try
			{
				this->modules[tid][m].reset(modules_vec[m]->clone());
			}
			catch (std::exception &e)
			{
				std::cerr << rang::tag::error << "Module clone failed when trying to duplicate the sequence: module "
				                              << "name is '" << modules_vec[m]->get_name() << "'." << std::endl;
				throw e;
			}
			this->all_modules[tid + (this->tasks_inplace ? 1 : 0)][m] = this->modules[tid][m].get();
		}

		if (this->is_thread_pinning())
			Thread_pinning::unpin();
	}

	auto get_module_id = [](const std::vector<MO*> &modules, const module::Module &module)
	{
		int m_id;
		for (m_id = 0; m_id < (int)modules.size(); m_id++)
			if (modules[m_id] == &module)
				return m_id;
		return -1;
	};

	auto get_task_id = [](const std::vector<std::shared_ptr<module::Task>> &tasks, const module::Task &task)
	{
		int t_id;
		for (t_id = 0; t_id < (int)tasks.size(); t_id++)
			if (tasks[t_id].get() == &task)
				return t_id;
		return -1;
	};

	auto get_socket_id = [](const std::vector<std::shared_ptr<module::Socket>> &sockets, const module::Socket &socket)
	{
		int s_id;
		for (s_id = 0; s_id < (int)sockets.size(); s_id++)
			if (sockets[s_id].get() == &socket)
				return s_id;
		return -1;
	};

	std::function<void(const Digraph_node<SS>*,
	                         Digraph_node<Sub_sequence>*,
	                   const size_t,
	                         std::vector<const Digraph_node<SS>*>&,
	                         std::map<size_t,Digraph_node<Sub_sequence>*>&)> duplicate_sequence;

	duplicate_sequence = [&](const Digraph_node<SS>           *sequence_ref,
	                               Digraph_node<Sub_sequence> *sequence_cpy,
	                         const size_t thread_id,
	                               std::vector<const Digraph_node<SS>*> &already_parsed_nodes,
	                               std::map<size_t,Digraph_node<Sub_sequence>*> &allocated_nodes)
	{
		if (sequence_ref != nullptr && sequence_ref->get_c() &&
		    std::find(already_parsed_nodes.begin(),
		              already_parsed_nodes.end(),
		              sequence_ref) == already_parsed_nodes.end())
		{
			already_parsed_nodes.push_back(sequence_ref);

			auto ss_ref = sequence_ref->get_c();
			auto ss_cpy = new Sub_sequence();

			ss_cpy->type = ss_ref->type;
			ss_cpy->id = ss_ref->id;
			ss_cpy->tasks_id = ss_ref->tasks_id;
			for (auto t_ref : ss_ref->tasks)
			{
				auto &m_ref = t_ref->get_module();

				auto m_id = get_module_id(modules_vec, m_ref);
				auto t_id = get_task_id(m_ref.tasks, *t_ref);

				assert(m_id != -1);
				assert(t_id != -1);

				// add the task to the sub-sequence
				ss_cpy->tasks.push_back(this->all_modules[thread_id][m_id]->tasks[t_id].get());

				// replicate the sockets binding
				for (size_t s_id = 0; s_id < t_ref->sockets.size(); s_id++)
				{
					if (t_ref->get_socket_type(*t_ref->sockets[s_id]) == module::socket_t::SIN)
					{
						const module::Socket* s_ref_out = nullptr;
						try { s_ref_out = &t_ref->sockets[s_id]->get_bound_socket(); } catch (...) {}
						if (s_ref_out)
						{
							auto &t_ref_out = s_ref_out->get_task();
							auto &m_ref_out = t_ref_out.get_module();

							auto m_id_out = get_module_id(modules_vec, m_ref_out);

							if (m_id_out != -1)
							{
								auto t_id_out = get_task_id(m_ref_out.tasks, t_ref_out);
								auto s_id_out = get_socket_id(t_ref_out.sockets, *s_ref_out);

								assert(t_id_out != -1);
								assert(s_id_out != -1);

								(*this->all_modules[thread_id][m_id_out]).tasks[t_id_out]->set_autoalloc(true);

								auto &s_in  = *this->all_modules[thread_id][m_id    ]->tasks[t_id    ]->sockets[s_id    ];
								auto &s_out = *this->all_modules[thread_id][m_id_out]->tasks[t_id_out]->sockets[s_id_out];
								s_in.bind(s_out);
							}
						}
					}
				}

				// replicate the tasks binding
				if (t_ref->is_no_input_socket() && t_ref->fake_input_socket != nullptr)
				{
					const module::Socket* s_ref_out = nullptr;
					try { s_ref_out = &t_ref->fake_input_socket->get_bound_socket(); } catch (...) {}
					if (s_ref_out)
					{
						auto &t_ref_out = s_ref_out->get_task();
						auto &m_ref_out = t_ref_out.get_module();

						auto m_id_out = get_module_id(modules_vec, m_ref_out);

						if (m_id_out != -1)
						{
							auto t_id_out = get_task_id(m_ref_out.tasks, t_ref_out);
							auto s_id_out = get_socket_id(t_ref_out.sockets, *s_ref_out);

							assert(t_id_out != -1);
							assert(s_id_out != -1);

							(*this->all_modules[thread_id][m_id_out]).tasks[t_id_out]->set_autoalloc(true);

							auto &t_in  = *this->all_modules[thread_id][m_id    ]->tasks[t_id    ];
							auto &s_out = *this->all_modules[thread_id][m_id_out]->tasks[t_id_out]->sockets[s_id_out];
							t_in.bind(s_out);
						}
					}
				}
			}

			// add the sub-sequence to the current tree node
			sequence_cpy->set_contents(ss_cpy);
			allocated_nodes[sequence_cpy->get_c()->id] = sequence_cpy;

			for (size_t c = 0; c < sequence_ref->get_children().size(); c++)
			{
				if (std::find(already_parsed_nodes.begin(),
				              already_parsed_nodes.end(),
				              sequence_ref->get_children()[c]) != already_parsed_nodes.end())
					sequence_cpy->add_child(allocated_nodes[sequence_ref->get_children()[c]->get_c()->id]);
				else
					sequence_cpy->add_child(new Digraph_node<Sub_sequence>({sequence_cpy},
					                                                       {},
					                                                       nullptr,
					                                                       sequence_cpy->get_depth() +1));
			}

			for (size_t c = 0; c < sequence_ref->get_children().size(); c++)
				duplicate_sequence(sequence_ref->get_children()[c], sequence_cpy->get_children()[c], thread_id,
				                   already_parsed_nodes, allocated_nodes);
		}
	};

	std::function<void(Digraph_node<Sub_sequence>*, std::vector<Digraph_node<Sub_sequence>*> &)> set_autoalloc_true =
		[&set_autoalloc_true](Digraph_node<Sub_sequence>* node,
		                      std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(node);

				for (auto t : node->get_c()->tasks)
					t->set_autoalloc(true);
				for (auto c : node->get_children())
					set_autoalloc_true(c, already_parsed_nodes);
			}
		};

	for (size_t thread_id = (this->tasks_inplace ? 1 : 0); thread_id < this->sequences.size(); thread_id++)
	{
		if (this->is_thread_pinning())
			Thread_pinning::pin(this->puids[thread_id]);

		this->sequences[thread_id] = new Digraph_node<Sub_sequence>({}, {}, nullptr, 0);
		already_parsed_nodes.clear();
		std::map<size_t,Digraph_node<Sub_sequence>*> allocated_nodes;
		duplicate_sequence(sequence, this->sequences[thread_id], thread_id, already_parsed_nodes, allocated_nodes);
		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes_bis;
		set_autoalloc_true(this->sequences[thread_id], already_parsed_nodes_bis);

		if (this->is_thread_pinning())
			Thread_pinning::unpin();
	}
}

template void tools::Sequence::duplicate<tools::Sub_sequence_const, const module::Module>(const Digraph_node<tools::Sub_sequence_const>*);
template void tools::Sequence::duplicate<tools::Sub_sequence,             module::Module>(const Digraph_node<tools::Sub_sequence      >*);

template <class SS>
void Sequence
::delete_tree(Digraph_node<SS> *node, std::vector<Digraph_node<SS>*> &already_deleted_nodes)
{
	if (node != nullptr &&
	    std::find(already_deleted_nodes.begin(), already_deleted_nodes.end(), node) == already_deleted_nodes.end())
	{
		already_deleted_nodes.push_back(node);
		for (auto c : node->get_children())
			this->delete_tree(c, already_deleted_nodes);
		auto c = node->get_c();
		if (c != nullptr) delete c;
		delete node;
	}
}

template void tools::Sequence::delete_tree<tools::Sub_sequence_const>(Digraph_node<tools::Sub_sequence_const>*, std::vector<Digraph_node<Sub_sequence_const>*> &already_deleted_nodes);
template void tools::Sequence::delete_tree<tools::Sub_sequence      >(Digraph_node<tools::Sub_sequence      >*, std::vector<Digraph_node<Sub_sequence      >*> &already_deleted_nodes);

template <class VTA>
void Sequence
::export_dot_subsequence(const VTA &subseq,
                         const std::vector<size_t> &tasks_id,
                         const subseq_t &subseq_type,
                         const std::string &subseq_name,
                         const std::string &tab,
                               std::ostream &stream) const
{
	if (!subseq_name.empty())
	{
		stream << tab << "subgraph \"cluster_" << subseq_name << "_" << +this << "\" {" << std::endl;
		stream << tab << tab << "node [style=filled];" << std::endl;
	}
	size_t exec_order = 0;
	for (auto &t : subseq)
	{
		std::string color = dynamic_cast<module::Adaptor*>(&t->get_module()) ? "green" :"blue";
		color = dynamic_cast<module::AProbe*>(&t->get_module()) ? "pink" : color;
		stream << tab << tab << "subgraph \"cluster_" << +&t->get_module() << "_" << +t << "\" {" << std::endl;
		stream << tab << tab << tab << "node [style=filled];" << std::endl;
		stream << tab << tab << tab << "subgraph \"cluster_" << +&t << "\" {" << std::endl;
		stream << tab << tab << tab << tab << "node [style=filled];" << std::endl;

		if (t->fake_input_socket != nullptr)
		{
			auto &s = t->fake_input_socket;
			std::string stype = "in";
			stream << tab << tab << tab << tab << "\"" << +s.get() << "\""
			                                   << "[label=\"" << stype << ":" << s->get_name() << "\", style=filled, "
			                                   << "fillcolor=red, penwidth=\"2.0\"];" << std::endl;
		}

		size_t sid = 0;
		for (auto &s : t->sockets)
		{
			std::string stype;
			bool static_input = false;
			switch (t->get_socket_type(*s))
			{
				case module::socket_t::SIN:
					stype = "in[" + std::to_string(sid) + "]";
					static_input = s->get_dataptr() != nullptr && s->bound_socket == nullptr;
					break;
				case module::socket_t::SOUT: stype = "out[" + std::to_string(sid) + "]"; break;
				default: stype = "unkn"; break;
			}

			std::string bold_or_not;
			if (t->is_last_input_socket(*s))
				bold_or_not = ", penwidth=\"2.0\"";

			stream << tab << tab << tab << tab << "\"" << +s.get() << "\""
			                                   << "[label=\"" << stype << ":" << s->get_name() << "\"" << bold_or_not
			                                   << (static_input ? ", style=filled, fillcolor=green" : "")
			                                   << "];" << std::endl;
			sid++;
		}

		stream << tab << tab << tab << tab << "label=\"" << t->get_name() << " (id = " << tasks_id[exec_order] << ")" << "\";" << std::endl;
		stream << tab << tab << tab << tab << "color=" << color << ";" << std::endl;
		stream << tab << tab << tab << "}" << std::endl;
		stream << tab << tab << tab << "label=\"" << t->get_module().get_name() << "\n"
		                            << (t->get_module().get_custom_name().empty() ? "" : t->get_module().get_custom_name() + "\n")
		                            << "exec order: [" << exec_order++ << "]\n"
		                            << "addr: " << +&t->get_module() << "\";" << std::endl;
		stream << tab << tab << tab << "color=" << color << ";" << std::endl;
		stream << tab << tab << "}" << std::endl;
	}
	if (!subseq_name.empty())
	{
		stream << tab << tab << "label=\"" << subseq_name << "\";"
		       << std::endl;
		std::string color = subseq_type == subseq_t::COMMUTE || subseq_type == subseq_t::SELECT ? "red" : "blue";
		stream << tab << tab << "color=" << color << ";" << std::endl;
		stream << tab << "}" << std::endl;
	}
}

template void tools::Sequence::export_dot_subsequence<std::vector<      module::Task*>>(const std::vector<      module::Task*>&, const std::vector<size_t>&, const subseq_t&, const std::string&, const std::string&, std::ostream&) const;
template void tools::Sequence::export_dot_subsequence<std::vector<const module::Task*>>(const std::vector<const module::Task*>&, const std::vector<size_t>&, const subseq_t&, const std::string&, const std::string&, std::ostream&) const;

template <class VTA>
void Sequence
::export_dot_connections(const VTA &subseq,
                         const std::string &tab,
                               std::ostream &stream) const
{
	for (auto &t : subseq)
	{
		for (auto &s : t->sockets)
		{
			if (t->get_socket_type(*s) == module::socket_t::SOUT)
			{
				auto &bss = s->get_bound_sockets();
				size_t id = 0;
				for (auto &bs : bss)
				{
					stream << tab << "\"" << +s.get() << "\" -> \"" << +bs << "\""
					       << (bss.size() > 1 ? "[label=\"" + std::to_string(id++) + "\"]" : "") << std::endl;
				}
			}
		}
	}
}

template void tools::Sequence::export_dot_connections<std::vector<      module::Task*>>(const std::vector<      module::Task*>&, const std::string&, std::ostream&) const;
template void tools::Sequence::export_dot_connections<std::vector<const module::Task*>>(const std::vector<const module::Task*>&, const std::string&, std::ostream&) const;

void Sequence
::export_dot(std::ostream &stream) const
{
	auto root = this->sequences[0];
	this->export_dot(root, stream);
}

template <class SS>
void Sequence
::export_dot(Digraph_node<SS>* root, std::ostream &stream) const
{
	std::function<void(Digraph_node<SS>*,
	                   const std::string&,
	                   std::ostream&,
	                   std::vector<Digraph_node<SS>*>&)> export_dot_subsequences_recursive =
		[&export_dot_subsequences_recursive, this](Digraph_node<SS>* cur_node,
		                                           const std::string &tab,
		                                           std::ostream &stream,
		                                           std::vector<Digraph_node<SS>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				this->export_dot_subsequence(cur_node->get_c()->tasks,
				                             cur_node->get_c()->tasks_id,
				                             cur_node->get_c()->type,
				                             "Sub-sequence "+std::to_string(cur_node->get_c()->id),
				                             tab,
				                             stream);

				for (auto c : cur_node->get_children())
					export_dot_subsequences_recursive(c, tab, stream, already_parsed_nodes);
			}
		};

	std::function<void(Digraph_node<SS>*,
	                   const std::string&,
	                   std::ostream&,
	                   std::vector<Digraph_node<SS>*> &)> export_dot_connections_recursive =
		[&export_dot_connections_recursive, this](Digraph_node<SS> *cur_node,
		                                          const std::string &tab,
		                                          std::ostream &stream,
		                                          std::vector<Digraph_node<SS>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
				std::find(already_parsed_nodes.begin(),
				          already_parsed_nodes.end(),
				          cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				this->export_dot_connections(cur_node->get_c()->tasks, tab, stream);

				for (auto c : cur_node->get_children())
					export_dot_connections_recursive(c, tab, stream, already_parsed_nodes);
			}
		};

	std::string tab = "\t";
	stream << "digraph Sequence {" << std::endl;
	std::vector<Digraph_node<SS>*> already_parsed_nodes;
	export_dot_subsequences_recursive(root, tab, stream, already_parsed_nodes);
	already_parsed_nodes.clear();
	export_dot_connections_recursive (root, tab, stream, already_parsed_nodes);
	stream << "}" << std::endl;
}

void Sequence
::gen_processes(const bool no_copy_mode)
{
	std::function<void(            Digraph_node<Sub_sequence>*,
	                   std::vector<Digraph_node<Sub_sequence>*>&)> gen_processes_recursive =
		[&gen_processes_recursive, no_copy_mode](            Digraph_node<Sub_sequence>   *cur_node,
		                                         std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);

				std::map<module::Task*, std::function<const int*()>> modified_tasks;
				auto contents = cur_node->get_c();
				contents->processes.clear();
				contents->rebind_sockets.clear();
				contents->rebind_dataptrs.clear();
				for (auto task : contents->tasks)
				{
					if (dynamic_cast<module::Switcher*>(&task->get_module()) &&
						task->get_name().find("select") != std::string::npos && no_copy_mode)
					{
						auto select_task = task;
						auto switcher = dynamic_cast<module::Switcher*>(&select_task->get_module());
						switcher->set_no_copy_select(true);

						const auto rebind_id = contents->rebind_sockets.size();
						contents->rebind_sockets.resize(rebind_id +1);
						contents->rebind_dataptrs.resize(rebind_id +1);

						for (size_t s = 0; s < select_task->sockets.size() -1; s++)
						{
							// there should be only one output socket at this time
							if (select_task->get_socket_type(*select_task->sockets[s]) == module::socket_t::SOUT)
							{
								std::vector<module::Socket*> bound_sockets;
								std::vector<void*> dataptrs;

								auto bs = select_task->sockets[s]->get_bound_sockets();
								bound_sockets.insert(bound_sockets.end(), bs.begin(), bs.end());
								for (auto sck : bs)
									dataptrs.push_back(sck->get_dataptr());

								contents->rebind_sockets[rebind_id].push_back(bound_sockets);
								contents->rebind_dataptrs[rebind_id].push_back(dataptrs);
							}
						}

						modified_tasks[select_task] = [contents, select_task, switcher, rebind_id]() -> const int*
						{
							select_task->exec();
							const int* status = (int*)select_task->sockets.back()->get_dataptr();

							const auto path = switcher->get_path();
							const auto in_dataptr = select_task->sockets[path]->get_dataptr();

							// rebind input sockets on the fly
							// there should be only one output socket at this time (sout_id == 0)
							for (size_t sout_id = 0; sout_id < contents->rebind_sockets[rebind_id].size(); sout_id++)
								for (size_t sin_id = 0; sin_id < contents->rebind_sockets[rebind_id][sout_id].size(); sin_id++)
									contents->rebind_sockets[rebind_id][sout_id][sin_id]->dataptr = in_dataptr;

							return status;
						};
					}

					if (dynamic_cast<module::Switcher*>(&task->get_module()) &&
						task->get_name().find("commute") != std::string::npos && no_copy_mode)
					{
						auto commute_task = task;
						auto switcher = dynamic_cast<module::Switcher*>(&commute_task->get_module());
						switcher->set_no_copy_commute(true);

						const auto rebind_id = contents->rebind_sockets.size();
						contents->rebind_sockets.resize(rebind_id +1);
						contents->rebind_dataptrs.resize(rebind_id +1);

						for (size_t s = 0; s < commute_task->sockets.size() -1; s++)
						{
							if (commute_task->get_socket_type(*commute_task->sockets[s]) == module::socket_t::SOUT)
							{
								std::vector<module::Socket*> bound_sockets;
								std::vector<void*> dataptrs;

								auto bs = commute_task->sockets[s]->get_bound_sockets();
								bound_sockets.insert(bound_sockets.end(), bs.begin(), bs.end());
								for (auto sck : bs)
									dataptrs.push_back(sck->get_dataptr());

								contents->rebind_sockets[rebind_id].push_back(bound_sockets);
								contents->rebind_dataptrs[rebind_id].push_back(dataptrs);
							}
						}

						modified_tasks[commute_task] = [contents, commute_task, switcher, rebind_id]() -> const int*
						{
							commute_task->exec();
							const int* status = (int*)commute_task->sockets.back()->get_dataptr();
							const auto in_dataptr = commute_task->sockets[0]->get_dataptr();
							const auto path = switcher->get_path();

							// rebind input sockets on the fly
							for (size_t sin_id = 0; sin_id < contents->rebind_sockets[rebind_id][path].size(); sin_id++)
									contents->rebind_sockets[rebind_id][path][sin_id]->dataptr = in_dataptr;

							return status;
						};
					}

					if (dynamic_cast<module::Adaptor*>(&task->get_module()) &&
						task->get_name().find("pull") != std::string::npos && no_copy_mode)
					{
						auto pull_task = task;
						auto adp_pull = dynamic_cast<module::Adaptor*>(&pull_task->get_module());
						adp_pull->set_no_copy_pull(true);
						const auto rebind_id = contents->rebind_sockets.size();
						contents->rebind_sockets.resize(rebind_id +1);
						contents->rebind_dataptrs.resize(rebind_id +1);

						for (size_t s = 0; s < pull_task->sockets.size() -1; s++)
						{
							if (pull_task->get_socket_type(*pull_task->sockets[s]) == module::socket_t::SOUT)
							{
								std::vector<module::Socket*> bound_sockets;
								std::vector<void*> dataptrs;

								bound_sockets.push_back(pull_task->sockets[s].get());
								dataptrs.push_back(pull_task->sockets[s]->get_dataptr());

								auto bs = pull_task->sockets[s]->get_bound_sockets();
								bound_sockets.insert(bound_sockets.end(), bs.begin(), bs.end());
								for (auto sck : bs)
									dataptrs.push_back(sck->get_dataptr());

								contents->rebind_sockets[rebind_id].push_back(bound_sockets);
								contents->rebind_dataptrs[rebind_id].push_back(dataptrs);
							}
						}

						modified_tasks[pull_task] = [contents, pull_task, adp_pull, rebind_id]() -> const int*
						{
							// active or passive waiting here
							pull_task->exec();
							const int* status = (int*)pull_task->sockets.back()->get_dataptr();

							// rebind input sockets on the fly
							for (size_t sin_id = 0; sin_id < contents->rebind_sockets[rebind_id].size(); sin_id++)
							{
								if (contents->rebind_sockets[rebind_id][sin_id].size() > 1)
								{
									// we start to 1 because the rebinding of the 'pull_task' is made in the
									// 'pull_task->exec()' call (this way the debug mode is still working)
									auto swap_buff = contents->rebind_sockets[rebind_id][sin_id][1]->get_dataptr();
									auto buff = adp_pull->get_filled_buffer(sin_id, swap_buff);
									contents->rebind_sockets[rebind_id][sin_id][1]->dataptr = buff;
									// for the next tasks the same buffer 'buff' is required, an easy mistake is to re-swap
									// and the data will be false, this is why we just bind 'buff'
									for (size_t ta = 2; ta < contents->rebind_sockets[rebind_id][sin_id].size(); ta++)
										contents->rebind_sockets[rebind_id][sin_id][ta]->dataptr = buff;
								}
							}
							adp_pull->wake_up_pusher();
							return status;
						};
					}

					if (dynamic_cast<module::Adaptor*>(&task->get_module()) &&
						task->get_name().find("push") != std::string::npos && no_copy_mode)
					{
						auto push_task = task;
						auto adp_push = dynamic_cast<module::Adaptor*>(&push_task->get_module());
						adp_push->set_no_copy_push(true);
						const auto rebind_id = contents->rebind_sockets.size();
						contents->rebind_sockets.resize(rebind_id +1);
						contents->rebind_dataptrs.resize(rebind_id +1);

						for (size_t s = 0; s < push_task->sockets.size() -1; s++)
							if (push_task->get_socket_type(*push_task->sockets[s]) == module::socket_t::SIN)
							{
								std::vector<module::Socket*> bound_sockets;
								std::vector<void*> dataptrs;

								bound_sockets.push_back(push_task->sockets[s].get());
								dataptrs.push_back(push_task->sockets[s]->get_dataptr());

								auto bound_socket = &push_task->sockets[s]->get_bound_socket();
								bound_sockets.push_back(bound_socket);
								dataptrs.push_back(bound_socket->get_dataptr());

								auto &bs = bound_socket->get_bound_sockets();
								for (size_t s = 0; s < bs.size(); s++)
									if (&bs[s]->get_task() != push_task)
									{
										bound_sockets.push_back(bs[s]);
										dataptrs.push_back(bs[s]->get_dataptr());
									}

								contents->rebind_sockets[rebind_id].push_back(bound_sockets);
								contents->rebind_dataptrs[rebind_id].push_back(dataptrs);
							}

						modified_tasks[push_task] = [contents, push_task, adp_push, rebind_id]() -> const int*
						{
							// active or passive waiting here
							push_task->exec();
							const int* status = (int*)push_task->sockets.back()->get_dataptr();
							// rebind output sockets on the fly
							for (size_t sout_id = 0; sout_id < contents->rebind_sockets[rebind_id].size(); sout_id++)
							{
								// we start to 1 because the rebinding of the 'push_task' is made in the
								// 'push_task->exec()' call (this way the debug mode is still working)
								auto swap_buff = contents->rebind_sockets[rebind_id][sout_id][1]->get_dataptr();
								auto buff = adp_push->get_empty_buffer(sout_id, swap_buff);
								contents->rebind_sockets[rebind_id][sout_id][1]->dataptr = buff;
								// the output socket linked to the push adp can have more than one socket bound and so
								// we have to rebind all the input sokects bound to the current output socket
								for (size_t ta = 2; ta < contents->rebind_sockets[rebind_id][sout_id].size(); ta++)
									contents->rebind_sockets[rebind_id][sout_id][ta]->dataptr = buff;
							}
							adp_push->wake_up_puller();
							return status;
						};
					}
				}

				for (auto task : contents->tasks)
					if (modified_tasks.count(task))
						contents->processes.push_back(modified_tasks[task]);
					else
						contents->processes.push_back([task]() -> const int*
						{
							task->exec();
							const int* status = (int*)task->sockets.back()->get_dataptr();
							return status;
						});

				for (auto c : cur_node->get_children())
					gen_processes_recursive(c, already_parsed_nodes);
			}
		};

	size_t thread_id = 0;
	for (auto &sequence : this->sequences)
	{
		if (this->is_thread_pinning())
			Thread_pinning::pin(this->puids[thread_id++]);

		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
		gen_processes_recursive(sequence, already_parsed_nodes);

		if (this->is_thread_pinning())
			Thread_pinning::unpin();
	}
}

void Sequence
::reset_no_copy_mode()
{
	std::function<void(Digraph_node<Sub_sequence>*,
	                   std::vector<Digraph_node<Sub_sequence>*> &)> reset_no_copy_mode_recursive =
		[&reset_no_copy_mode_recursive](Digraph_node<Sub_sequence>* cur_node,
		                                std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				auto contents = cur_node->get_c();
				for (auto task : contents->tasks)
				{
					if (dynamic_cast<module::Switcher*>(&task->get_module()) &&
						task->get_name().find("select") != std::string::npos)
					{
						auto select_task = task;
						auto switcher = dynamic_cast<module::Switcher*>(&select_task->get_module());
						switcher->set_no_copy_select(false);
					}

					if (dynamic_cast<module::Switcher*>(&task->get_module()) &&
						task->get_name().find("commute") != std::string::npos)
					{
						auto commute_task = task;
						auto switcher = dynamic_cast<module::Switcher*>(&commute_task->get_module());
						switcher->set_no_copy_commute(false);
					}

					if (dynamic_cast<module::Adaptor*>(&task->get_module()) &&
						task->get_name().find("pull") != std::string::npos)
					{
						auto pull_task = task;
						auto adp_pull = dynamic_cast<module::Adaptor*>(&pull_task->get_module());
						adp_pull->set_no_copy_pull(false);
						adp_pull->reset_buffer();
					}

					if (dynamic_cast<module::Adaptor*>(&task->get_module()) &&
						task->get_name().find("push") != std::string::npos)
					{
						auto push_task = task;
						auto adp_push = dynamic_cast<module::Adaptor*>(&push_task->get_module());
						adp_push->set_no_copy_push(false);
						adp_push->reset_buffer();
					}
				}

				for (size_t rebind_id = 0; rebind_id < contents->rebind_sockets.size(); rebind_id++)
					for (size_t s = 0; s < contents->rebind_sockets[rebind_id].size(); s++)
						for (size_t ta = 0; ta < contents->rebind_sockets[rebind_id][s].size(); ta++)
							contents->rebind_sockets[rebind_id][s][ta]->dataptr =
								contents->rebind_dataptrs[rebind_id][s][ta];

				for (auto c : cur_node->get_children())
					reset_no_copy_mode_recursive(c, already_parsed_nodes);
			}
		};

	for (auto &sequence : this->sequences)
	{
		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
		reset_no_copy_mode_recursive(sequence, already_parsed_nodes);
	}
}

void Sequence
::set_no_copy_mode(const bool no_copy_mode)
{
	this->no_copy_mode = no_copy_mode;
}

bool Sequence
::is_no_copy_mode() const
{
	return this->no_copy_mode;
}

void Sequence
::set_auto_stop(const bool auto_stop)
{
	this->auto_stop = auto_stop;
}

bool Sequence
::is_auto_stop() const
{
	return this->auto_stop;
}

Sub_sequence* Sequence
::get_last_subsequence(const size_t tid)
{
	std::function<Sub_sequence*(Digraph_node<Sub_sequence>*,
	              std::vector<Digraph_node<Sub_sequence>*>&)> get_last_subsequence_recursive =
		[&get_last_subsequence_recursive](Digraph_node<Sub_sequence>* cur_node,
		                                  std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes) -> Sub_sequence*
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				Sub_sequence* last_ss = nullptr;
				for (auto c : cur_node->get_children())
					last_ss = get_last_subsequence_recursive(c, already_parsed_nodes);
				return last_ss ? last_ss : cur_node->get_contents();
			}
			else
			{
				// should never happen
				return nullptr;
			}
		};

	std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
	return get_last_subsequence_recursive(this->sequences[tid], already_parsed_nodes);
}

void Sequence
::update_tasks_id(const size_t tid)
{
	std::function<void(Digraph_node<Sub_sequence>*, size_t&,
		               std::vector<Digraph_node<Sub_sequence>*> &)> update_tasks_id_recursive =
		[&update_tasks_id_recursive](Digraph_node<Sub_sequence>* cur_node, size_t& taid,
		                             std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				Sub_sequence* ss = cur_node->get_contents();
				ss->tasks_id.resize(ss->tasks.size());
				std::iota(ss->tasks_id.begin(), ss->tasks_id.end(), taid);
				taid += ss->tasks_id.size();

				for (auto c : cur_node->get_children())
					update_tasks_id_recursive(c, taid, already_parsed_nodes);
			}
		};

	size_t taid = 0;
	std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
	return update_tasks_id_recursive(this->sequences[tid], taid, already_parsed_nodes);
}

std::vector<module::Task*> Sequence
::get_tasks_from_id(const size_t taid)
{
	std::function<void(Digraph_node<Sub_sequence>*, const size_t, std::vector<module::Task*>&,
		               std::vector<Digraph_node<Sub_sequence>*> &)> get_tasks_from_id_recursive =
		[&get_tasks_from_id_recursive](Digraph_node<Sub_sequence>* cur_node,
		                               const size_t taid,
		                               std::vector<module::Task*> &tasks,
		                               std::vector<Digraph_node<Sub_sequence>*> &already_parsed_nodes)
		{
			if (cur_node != nullptr &&
			    std::find(already_parsed_nodes.begin(),
			              already_parsed_nodes.end(),
			              cur_node) == already_parsed_nodes.end())
			{
				already_parsed_nodes.push_back(cur_node);
				Sub_sequence* ss = cur_node->get_contents();
				bool found = false;
				for (size_t t = 0; t < ss->tasks_id.size(); t++)
					if (ss->tasks_id[t] == taid)
					{
						tasks.push_back(ss->tasks[t]);
						found = true;
						break;
					}

				if (!found)
					for (auto c : cur_node->get_children())
						get_tasks_from_id_recursive(c, taid, tasks, already_parsed_nodes);
			}
		};

	std::vector<module::Task*> tasks;
	for (auto &s : this->sequences)
	{
		std::vector<Digraph_node<Sub_sequence>*> already_parsed_nodes;
		get_tasks_from_id_recursive(s, taid, tasks, already_parsed_nodes);
	}
	return tasks;
}

void Sequence
::update_firsts_and_lasts_tasks()
{
	this->firsts_tasks.clear();
	this->firsts_tasks.resize(this->n_threads);
	for (auto taid : firsts_tasks_id)
	{
		auto tasks = this->get_tasks_from_id(taid);
		for (size_t tid = 0; tid < tasks.size(); tid++)
			firsts_tasks[tid].push_back(tasks[tid]);
	}

	this->lasts_tasks.clear();
	this->lasts_tasks.resize(this->n_threads);
	for (auto taid : lasts_tasks_id)
	{
		auto tasks = this->get_tasks_from_id(taid);
		for (size_t tid = 0; tid < tasks.size(); tid++)
			lasts_tasks[tid].push_back(tasks[tid]);
	}
}

void Sequence
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		if (n_frames <= 0)
		{
			std::stringstream message;
			message << "'n_frames' has to be greater than 0 ('n_frames' = " << n_frames << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		std::vector<std::pair<module::Socket*, module::Socket*>> unbind_sockets;
		std::vector<std::pair<module::Task*, module::Socket*>> unbind_tasks;
		std::function<void(const std::vector<module::Task*>, module::Task*)> unbind_sockets_recursive =
		[&unbind_sockets, &unbind_tasks, &unbind_sockets_recursive](const std::vector<module::Task*> possessed_tsks,
		                                                            module::Task* tsk_out)
		{
			for (auto sck_out : tsk_out->sockets)
			{
				if (tsk_out->get_socket_type(*sck_out) == module::socket_t::SOUT)
				{
					for (auto sck_in : sck_out->get_bound_sockets())
					{
						auto tsk_in = &sck_in->get_task();
						// if the task of the current input socket is in the tasks of the sequence
						if (std::find(possessed_tsks.begin(), possessed_tsks.end(), tsk_in) != possessed_tsks.end())
						{
							if (tsk_in->is_no_input_socket())
							{
								tsk_in->unbind(*sck_out);
								unbind_tasks.push_back(std::make_pair(tsk_in, sck_out.get()));
							}
							else
							{
								sck_in->unbind(*sck_out);
								// memorize the unbinds to rebind after!
								unbind_sockets.push_back(std::make_pair(sck_in, sck_out.get()));
							}
							// do the same operation recursively on the current "tsk_in"
							unbind_sockets_recursive(possessed_tsks, tsk_in);
						}
					}
				}
			}
		};

		auto tsks_per_threads = this->get_tasks_per_threads();
		for (size_t t = 0; t < this->get_n_threads(); t++)
			for (auto &tsk : this->firsts_tasks[t])
				unbind_sockets_recursive(tsks_per_threads[t], tsk);

		// set_n_frames for all the modules (be aware that this operation can fail)
		for (auto &mm : this->all_modules)
			for (auto &m : mm)
				m->set_n_frames(n_frames);

		// rebind the sockets
		for (auto &u : unbind_sockets)
			u.first->bind(*u.second);

		// rebind the tasks
		for (auto &u : unbind_tasks)
			u.first->bind(*u.second);
	}
}