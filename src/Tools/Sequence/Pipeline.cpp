#include <tuple>
#include <thread>
#include <utility>
#include <cassert>

#include "Module/Adaptor/Adaptor.hpp"
#include "Module/Adaptor/Adaptor_1_to_n.hpp"
#include "Module/Adaptor/Adaptor_n_to_1.hpp"
#include "Tools/Interface/Interface_waiting.hpp"
#include "Tools/Exception/exception.hpp"
#include "Tools/Sequence/Pipeline.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

// Pipeline
// ::Pipeline(const module::Task &first,
//            const module::Task &last,
//            const std::vector<std::pair<std::vector<const module::Task*>, std::vector<const module::Task*>>> &sep_stages,
//            const std::vector<size_t> &n_threads,
//            const std::vector<bool> &thread_pinning,
//            const std::vector<std::vector<size_t>> &puids)
// : original_sequence(first, last, 1),
//   stages(sep_stages.size()),
//   adaptors(sep_stages.size() -1)
// {
// 	this->init<const module::Task>(first, &last, sep_stages, n_threads, thread_pinning, puids);
// }

// Pipeline
// ::Pipeline(const module::Task &first,
//            const std::vector<std::pair<std::vector<const module::Task*>, std::vector<const module::Task*>>> &sep_stages,
//            const std::vector<size_t> &n_threads,
//            const std::vector<bool> &thread_pinning,
//            const std::vector<std::vector<size_t>> &puids)
// : original_sequence(first, 1),
//   stages(sep_stages.size()),
//   adaptors(sep_stages.size() -1)
// {
// 	const module::Task* last = nullptr;
// 	this->init<const module::Task>(first, last, sep_stages, n_threads, thread_pinning, puids);
// }

Pipeline
::Pipeline(const std::vector<module::Task*> &firsts,
           const std::vector<module::Task*> &lasts,
           const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages,
           const std::vector<size_t> &n_threads,
           const std::vector<bool> &thread_pinning,
           const std::vector<std::vector<size_t>> &puids/*,
           const std::vector<bool> &tasks_inplace*/)
: original_sequence(firsts, lasts, 1),
  stages(sep_stages.size()),
  adaptors(sep_stages.size() -1)
{
	this->init<module::Task>(firsts, lasts, sep_stages, n_threads, thread_pinning, puids/*, tasks_inplace*/);
}

Pipeline
::Pipeline(const std::vector<module::Task*> &firsts,
           const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages,
           const std::vector<size_t> &n_threads,
           const std::vector<bool> &thread_pinning,
           const std::vector<std::vector<size_t>> &puids/*,
           const std::vector<bool> &tasks_inplace*/)
: original_sequence(firsts, 1),
  stages(sep_stages.size()),
  adaptors(sep_stages.size() -1)
{
	this->init<module::Task>(firsts, {}, sep_stages, n_threads, thread_pinning, puids/*, tasks_inplace*/);
}

Pipeline
::Pipeline(module::Task &first,
           module::Task &last,
           const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages,
           const std::vector<size_t> &n_threads,
           const std::vector<bool> &thread_pinning,
           const std::vector<std::vector<size_t>> &puids/*,
           const std::vector<bool> &tasks_inplace*/)
: Pipeline({&first}, {&last}, sep_stages, n_threads, thread_pinning, puids/*, tasks_inplace*/)
{
}

Pipeline
::Pipeline(module::Task &first,
           const std::vector<std::pair<std::vector<module::Task*>, std::vector<module::Task*>>> &sep_stages,
           const std::vector<size_t> &n_threads,
           const std::vector<bool> &thread_pinning,
           const std::vector<std::vector<size_t>> &puids/*,
           const std::vector<bool> &tasks_inplace*/)
: Pipeline({&first}, sep_stages, n_threads, thread_pinning, puids/*, tasks_inplace*/)
{
}

std::vector<Sequence*> Pipeline
::get_stages()
{
	std::vector<Sequence*> stages;
	for (auto &stage : this->stages)
		stages.push_back(stage.get());
	return stages;
}

Sequence& Pipeline
::operator[](const size_t stage_id)
{
	assert(stage_id < this->stages.size());
	return *this->stages[stage_id];
}

template <class TA>
tools::Sequence* create_sequence(const std::vector<TA*> &firsts,
                                 const std::vector<TA*> &lasts,
                                 const size_t &n_threads,
                                 const bool &thread_pinning,
                                 const std::vector<size_t> &puids,
                                 const bool &tasks_inplace)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <>
tools::Sequence* create_sequence<const module::Task>(const std::vector<const module::Task*> &firsts,
                                                     const std::vector<const module::Task*> &lasts,
                                                     const size_t &n_threads,
                                                     const bool &thread_pinning,
                                                     const std::vector<size_t> &puids,
                                                     const bool &tasks_inplace)
{
	if (lasts.size())
		return new tools::Sequence(firsts, lasts, n_threads, thread_pinning, puids);
	else
		return new tools::Sequence(firsts, n_threads, thread_pinning, puids);
}

template <>
Sequence* create_sequence<module::Task>(const std::vector<module::Task*> &firsts,
                                        const std::vector<module::Task*> &lasts,
                                        const size_t &n_threads,
                                        const bool &thread_pinning,
                                        const std::vector<size_t> &puids,
                                        const bool &tasks_inplace)
{
	if (lasts.size())
		return new tools::Sequence(firsts, lasts, n_threads, thread_pinning, puids, tasks_inplace);
	else
		return new tools::Sequence(firsts, n_threads, thread_pinning, puids, tasks_inplace);
}

template <class TA>
void Pipeline
::init(const std::vector<TA*> &firsts,
       const std::vector<TA*> &lasts,
       const std::vector<std::pair<std::vector<TA*>,std::vector<TA*>>> &sep_stages,
       const std::vector<size_t> &n_threads,
       const std::vector<bool> &thread_pinning,
       const std::vector<std::vector<size_t>> &puids/*,
       const std::vector<bool> &tasks_inplace*/)
{
	if (sep_stages.size() != n_threads.size() && n_threads.size() != 0)
	{
		std::stringstream message;
		message << "'n_threads.size()' has to be equal to 'sep_stages.size()' or equal to '0' ('n_threads.size()' = "
		        << n_threads.size() << " , 'sep_stages.size()' = " << sep_stages.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sep_stages.size() != thread_pinning.size() && thread_pinning.size() != 0)
	{
		std::stringstream message;
		message << "'thread_pinning.size()' has to be equal to 'sep_stages.size()' or equal to '0' ("
		        << "'thread_pinning.size()' = " << thread_pinning.size() << " , 'sep_stages.size()' = "
		        << sep_stages.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (sep_stages.size() != puids.size() && puids.size() != 0)
	{
		std::stringstream message;
		message << "'puids.size()' has to be equal to 'sep_stages.size()' or equal to '0' ('puids.size()' = "
		        << puids.size() << " , 'sep_stages.size()' = " << sep_stages.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	// if (sep_stages.size() != tasks_inplace.size() && tasks_inplace.size() != 0)
	// {
	// 	std::stringstream message;
	// 	message << "'tasks_inplace.size()' has to be equal to 'sep_stages.size()' or equal to '0' ('"
	// 	        << "tasks_inplace.size()' = " << tasks_inplace.size() << " , 'sep_stages.size()' = "
	// 	        << sep_stages.size() << ").";
	// 	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	// }

	bool prev_is_parallel = false;
	for (auto t : n_threads)
	{
		if (t > 1 && prev_is_parallel)
		{
			std::stringstream message;
			message << "Consecutive parallel stages are not supported.";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
		prev_is_parallel = t > 1;
	}

	for (size_t s = 0; s < sep_stages.size(); s++)
	{
		const std::vector<TA*> &stage_firsts = sep_stages[s].first;
		const std::vector<TA*> &stage_lasts = sep_stages[s].second;
		const size_t stage_n_threads = n_threads.size() ? n_threads[s] : 1;
		const bool stage_thread_pinning = thread_pinning.size() ? thread_pinning[s] : false;
		const std::vector<size_t> stage_puids =  puids.size() ? puids[s] : std::vector<size_t>();
		const bool stage_tasks_inplace = /*tasks_inplace.size() ? tasks_inplace[s] :*/ true;

		this->stages[s].reset(create_sequence<TA>(stage_firsts,
		                                          stage_lasts,
		                                          stage_n_threads,
		                                          stage_thread_pinning,
		                                          stage_puids,
		                                          stage_tasks_inplace));
	}

	// verify that the sequential sequence is equivalent to the pipeline sequence
	auto ref_tasks = this->original_sequence.get_tasks_per_threads()[0];
	auto cur_tasks = this->get_tasks_per_threads()[0];

	if (ref_tasks.size() != cur_tasks.size())
	{
		std::stringstream message;
		message << "'ref_tasks.size()' has to be equal to 'cur_tasks.size()' ('ref_tasks.size()' = "
		        << ref_tasks.size() << ", 'cur_tasks.size()' = " << cur_tasks.size() << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	for (size_t ta = 0; ta < cur_tasks.size(); ta++)
	{
		if (std::find(ref_tasks.begin(), ref_tasks.end(), cur_tasks[ta]) == ref_tasks.end())
		{
			std::stringstream message;
			message << "'cur_tasks[ta]' is not contained in the 'ref_tasks' vector ('ta' = " << ta
			        << ", 'cur_tasks[ta]' = " << +cur_tasks[ta]
			        << ", 'cur_tasks[ta]->get_name()' = " << cur_tasks[ta]->get_name() << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}
	}

	this->create_adaptors();
}

void Pipeline
::create_adaptors()
{
	//                     sck out addr     occ     stage   tsk id  sck id
	std::vector<std::tuple<module::Socket*, size_t, size_t, size_t, size_t>> out_sck_orphans;
	//                               sck out addr     stage   tsk id  sck id
	std::vector<std::pair<std::tuple<module::Socket*, size_t, size_t, size_t>,
	//                               sck in addr      stage   tsk id  sck id
	                      std::tuple<module::Socket*, size_t, size_t, size_t>>> sck_orphan_binds;

	// for all the stages in the pipeline
	for (size_t sta = 0; sta < this->stages.size() -1; sta++)
	{
		// ------------------------------------------------------------------------------------------------------------
		// --------------------------------------------------------------- collect orphan output sockets in stage 'sta'
		// ------------------------------------------------------------------------------------------------------------
		std::vector<std::vector<module::Task*>> tasks_per_threads = this->stages[sta]->get_tasks_per_threads();
		// for all the threads in the current stage
		// for (size_t t = 0; t < tasks_per_threads.size(); t++)
		size_t t = 0;
		{
			// for all the tasks in the stage
			for (size_t tsk_id = 0; tsk_id < tasks_per_threads[t].size(); tsk_id++)
			{
				auto tsk = tasks_per_threads[t][tsk_id];
				// for all the sockets of the tasks
				for (size_t sck_id = 0; sck_id < tsk->sockets.size(); sck_id++)
				{
					auto sck = tsk->sockets[sck_id];
					// if the current socket is an output socket type
					if (tsk->get_socket_type(*sck) == module::socket_t::SOUT ||
					    tsk->get_socket_type(*sck) == module::socket_t::SIN_SOUT)
					{
						// for all the bounded sockets to the current socket
						for (auto bsck : sck->get_bound_sockets())
						{
							// check if the task of the bounded socket is not in the current stage
							if (std::find(tasks_per_threads[t].begin(),
							              tasks_per_threads[t].end  (),
							              &bsck->get_task()) == tasks_per_threads[t].end())
							{
								// check the position of the socket in the orphans
								size_t pos = 0;
								for (; pos < out_sck_orphans.size(); pos++)
									if (std::get<0>(out_sck_orphans[pos]) == sck.get())
										break;

								if (pos == out_sck_orphans.size())
									out_sck_orphans.push_back(std::make_tuple(sck.get(), 1, sta, tsk_id, sck_id));
								else
									std::get<1>(out_sck_orphans[pos])++;
							}
						}
					}
				}
			}
		}

		// ------------------------------------------------------------------------------------------------------------
		// -------------------------------------- collect orphan input sockets in stage 'sta +1' and create connections
		// ------------------------------------------------------------------------------------------------------------
		tasks_per_threads = this->stages[sta +1]->get_tasks_per_threads();
		// for all the threads in the current stage
		// for (size_t t = 0; t < tasks_per_threads.size(); t++)
		{
			// for all the tasks in the stage
			// for (auto &tsk : tasks_per_threads[t])
			for (size_t tsk_id = 0; tsk_id < tasks_per_threads[t].size(); tsk_id++)
			{
				auto tsk = tasks_per_threads[t][tsk_id];
				// for all the sockets of the tasks
				// for (auto sck : tsk->sockets)
				for (size_t sck_id = 0; sck_id < tsk->sockets.size(); sck_id++)
				{
					auto sck = tsk->sockets[sck_id];
					// if the current socket is an input socket type
					if (tsk->get_socket_type(*sck) == module::socket_t::SIN ||
					    tsk->get_socket_type(*sck) == module::socket_t::SIN_SOUT)
					{
						try
						{
							// get output bounded socket
							auto bsck = &sck->get_bound_socket(); // can throw if there is no bounded socket

							// check if the task of the bounded socket is not in the current stage
							if (std::find(tasks_per_threads[t].begin(),
							              tasks_per_threads[t].end(),
							              &bsck->get_task()) == tasks_per_threads[t].end())
							{
								// check the position of the bounded socket in the orphans
								size_t pos = 0;
								for (; pos < out_sck_orphans.size(); pos++)
									if (std::get<0>(out_sck_orphans[pos]) == bsck)
										break;

								if (pos < out_sck_orphans.size())
									sck_orphan_binds.push_back(std::make_pair(
									                               std::make_tuple(std::get<0>(out_sck_orphans[pos]),
									                                               std::get<2>(out_sck_orphans[pos]),
									                                               std::get<3>(out_sck_orphans[pos]),
									                                               std::get<4>(out_sck_orphans[pos])),
									                               std::make_tuple(sck.get(), sta +1, tsk_id, sck_id)));
							}
						}
						catch (const std::exception&) { /* do nothing */ }
					}
				}
			}
		}
	}

	// ----------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------- prints for debug
	// ----------------------------------------------------------------------------------------------------------------
	// std::cout << "Orphan output sockets list:" << std::endl;
	// for (auto &sck : out_sck_orphans)
	// {
	// 	auto sck_out_name = std::get<0>(sck)->get_name();
	// 	auto tsk_out_name = std::get<0>(sck)->get_task().get_name();
	// 	auto sck_out_occ  = std::get<1>(sck);
	// 	auto tsk_out_sta  = std::get<2>(sck);
	// 	auto tsk_out_id   = std::get<3>(sck);
	// 	auto sck_out_id   = std::get<4>(sck);

	// 	std::cout << "  " << tsk_out_name << "[" << sck_out_name << "] (stage " << tsk_out_sta << ", " << sck_out_occ
	// 	          << " occurrences, tsk id = " << tsk_out_id << ", sck id = " << sck_out_id << ")" << std::endl;
	// }

	// std::cout << std::endl << "Detected socket binds:" << std::endl;
	// for (auto &bind : sck_orphan_binds)
	// {
	// 	auto sck_out_name = std::get<0>(bind.first)->get_name();
	// 	auto tsk_out_name = std::get<0>(bind.first)->get_task().get_name();
	// 	auto tsk_out_sta  = std::get<1>(bind.first);
	// 	auto tsk_out_id   = std::get<2>(bind.first);
	// 	auto sck_out_id   = std::get<3>(bind.first);

	// 	auto sck_in_name = std::get<0>(bind.second)->get_name();
	// 	auto tsk_in_name = std::get<0>(bind.second)->get_task().get_name();
	// 	auto tsk_in_sta  = std::get<1>(bind.second);
	// 	auto tsk_in_id   = std::get<2>(bind.second);
	// 	auto sck_in_id   = std::get<3>(bind.second);

	// 	std::cout << "  " << tsk_out_name << "[" << sck_out_name << "] (stage " << tsk_out_sta << ", tsk id = "
	// 	                  << tsk_out_id << ", sck id = " << sck_out_id << ")"  << " -> "
	// 	                  << tsk_in_name  << "[" << sck_in_name  << "] (stage " << tsk_in_sta  << ", tsk id = "
	// 	                  << tsk_in_id  << ", sck id = " << sck_in_id  << ")" << std::endl;
	// }

	// ----------------------------------------------------------------------------------------------------------------
	// ------------------------------------------------------------------------------------------------ create adaptors
	// ----------------------------------------------------------------------------------------------------------------
	module::Adaptor* adp = nullptr;
	std::map<module::Socket*, size_t> sck_to_adp_sck_id;
	for (size_t sta = 0; sta < this->stages.size(); sta++)
	{
		const auto n_threads = this->stages[sta]->get_n_threads();
		std::vector<std::vector<module::Task*>> tasks_per_threads = this->stages[sta]->get_tasks_per_threads();

		// ------------------------------------------------------------------------------------------------------------
		// ----------------------------------------------------------------------------------------------- pull adaptor
		// ------------------------------------------------------------------------------------------------------------
		if (sta > 0)
		{
			assert(adp != nullptr);
			auto sck_orphan_binds_new = sck_orphan_binds;
			auto n_threads_prev_sta = this->stages[sta -1]->get_n_threads();
			for (size_t t = 0; t < n_threads; t++)
			{
				module::Adaptor* cur_adp = (t == 0) ? adp : adp->clone();
				if (t > 0)
				{
					this->adaptors[sta -1].second.push_back(std::unique_ptr<module::Adaptor>(cur_adp));
					cur_adp->set_custom_name((n_threads_prev_sta == 1 ? "Adp_1_to_n_" : "Adp_n_to_1_") +
						std::to_string(sta -1));
				}

				auto task_pull = n_threads_prev_sta == 1 ? &(*cur_adp)[(int)module::adp::tsk::pull_n] :
				                                           &(*cur_adp)[(int)module::adp::tsk::pull_1];

				sck_orphan_binds_new.clear();
				for (auto &bind : sck_orphan_binds)
				{
					auto tsk_out_sta = std::get<1>(bind.first);
					if (tsk_out_sta < sta)
					{
						auto tsk_in_sta = std::get<1>(bind.second);
						if (tsk_in_sta == sta)
						{
							auto sck_out_ptr = std::get<0>(bind.first);
							auto tsk_in_id = std::get<2>(bind.second);
							auto sck_in_id = std::get<3>(bind.second);
							auto sck_in = tasks_per_threads[t][tsk_in_id]->sockets[sck_in_id];
							sck_in->reset();
							sck_in->bind(*task_pull->sockets[sck_to_adp_sck_id[sck_out_ptr]]);
						}
						else
							sck_orphan_binds_new.push_back(bind);
					}
					else
						sck_orphan_binds_new.push_back(bind);
				}

				if (t > 0)
				{
					// add the adaptor to the current stage
					this->stages[sta]->all_modules[t].push_back(cur_adp);
				}

				auto ss = this->stages[sta]->sequences[t]->get_contents();
				assert(ss != nullptr);
				ss->tasks    .insert(ss->tasks.begin(), task_pull);
				ss->processes.insert(ss->processes.begin(), [task_pull]() -> int { return task_pull->exec(); });
				this->stages[sta]->update_tasks_id(t);
			}
			this->stages[sta]->firsts_tasks_id.clear();
			this->stages[sta]->firsts_tasks_id.push_back(0);
			this->stages[sta]->n_tasks++;
			sck_orphan_binds = sck_orphan_binds_new;
		}

		// ------------------------------------------------------------------------------------------------------------
		// ----------------------------------------------------------------------------------------------- push adaptor
		// ------------------------------------------------------------------------------------------------------------
		std::map<module::Socket*, size_t> sck_to_adp_sck_id_new;
		if (sta < this->stages.size() -1)
		{
			std::vector<size_t         > adp_n_elmts;
			std::vector<std::type_index> adp_datatype;
			size_t                       adp_buffer_size = 1024;
			bool                         adp_active_waiting = false;
			size_t                       adp_n_frames = 1;

			std::vector<module::Socket*> passed_scks_out;
			for (auto &bind : sck_orphan_binds)
			{
				auto tsk_out_sta = std::get<1>(bind.first);
				if (tsk_out_sta <= sta)
				{
					auto sck_out = std::get<0>(bind.first);
					if (std::find(passed_scks_out.begin(), passed_scks_out.end(), sck_out) == passed_scks_out.end())
					{
						adp_n_frames = sck_out->get_task().get_module().get_n_frames();
						adp_n_elmts.push_back(sck_out->get_n_elmts() / adp_n_frames);
						adp_datatype.push_back(sck_out->get_datatype());
						passed_scks_out.push_back(sck_out);
					}
				}
			}
			passed_scks_out.clear();

			// allocate the adaptor for the first thread
			if (n_threads == 1)
				adp = new module::Adaptor_1_to_n(adp_n_elmts,
				                                 adp_datatype,
				                                 adp_buffer_size,
				                                 adp_active_waiting,
				                                 adp_n_frames);
			else
				adp = new module::Adaptor_n_to_1(adp_n_elmts,
				                                 adp_datatype,
				                                 adp_buffer_size,
				                                 adp_active_waiting,
				                                 adp_n_frames);

			for (size_t t = 0; t < n_threads; t++)
			{
				module::Adaptor* cur_adp = (t == 0) ? adp : adp->clone();
				cur_adp->set_custom_name((n_threads == 1 ? "Adp_1_to_n_" : "Adp_n_to_1_") + std::to_string(sta));
				this->adaptors[sta].first.push_back(std::unique_ptr<module::Adaptor>(cur_adp));
				auto task_push = n_threads == 1 ? &(*cur_adp)[(int)module::adp::tsk::push_1] :
				                                  &(*cur_adp)[(int)module::adp::tsk::push_n];

				sck_to_adp_sck_id_new.clear();
				size_t adp_sck_id = 0;
				for (auto &bind : sck_orphan_binds) // bind standard task to last adaptor
				{
					auto tsk_out_sta = std::get<1>(bind.first);
					if (tsk_out_sta == sta)
					{
						auto sck_out_ptr = std::get<0>(bind.first);
						if (std::find(passed_scks_out.begin(),
						              passed_scks_out.end(),
						              sck_out_ptr) == passed_scks_out.end())
						{
							auto tsk_out_id = std::get<2>(bind.first);
							auto sck_out_id = std::get<3>(bind.first);
							auto sck_out = tasks_per_threads[t][tsk_out_id]->sockets[sck_out_id];
							sck_out->reset();
							sck_to_adp_sck_id_new[sck_out_ptr] = adp_sck_id;
							task_push->sockets[adp_sck_id++]->bind(*sck_out);
							passed_scks_out.push_back(sck_out_ptr);
						}
					}
					else if (tsk_out_sta < sta) // bind prev. adaptor to last adaptor
					{
						auto n_threads_prev_sta = this->stages[sta -1]->get_n_threads();
						auto sck_out_ptr = std::get<0>(bind.first);
						if (std::find(passed_scks_out.begin(),
						              passed_scks_out.end(),
						              sck_out_ptr) == passed_scks_out.end())
						{
							auto tsk_out_id = (n_threads_prev_sta == 1) ? (size_t)module::adp::tsk::pull_n :
							                                              (size_t)module::adp::tsk::pull_1;
							auto sck_out_id = sck_to_adp_sck_id[sck_out_ptr];
							sck_to_adp_sck_id_new[sck_out_ptr] = adp_sck_id;
							auto adp_prev = t == 0 ? this->adaptors[sta -1].first [   0] :
							                         this->adaptors[sta -1].second[t -1];
							auto sck_out = (*adp_prev)[tsk_out_id].sockets[sck_out_id];
							task_push->sockets[adp_sck_id++]->bind(*sck_out);
							passed_scks_out.push_back(sck_out_ptr);
						}
					}
				}
				passed_scks_out.clear();

				// add the adaptor to the current stage
				this->stages[sta]->all_modules[t].push_back(cur_adp);

				auto ss = this->stages[sta]->get_last_subsequence(t);
				assert(ss != nullptr);
				ss->tasks    .push_back(task_push);
				ss->processes.push_back([task_push]() -> int { return task_push->exec(); });
				ss->tasks_id .push_back(ss->tasks_id[ss->tasks_id.size() -1] +1);
				// this->stages[sta]->update_tasks_id(t);
			}
			auto last_task_id = this->stages[sta]->lasts_tasks_id.back() +1;
			this->stages[sta]->lasts_tasks_id.clear();
			this->stages[sta]->lasts_tasks_id.push_back(last_task_id);
			this->stages[sta]->n_tasks++;
		}

		this->stages[sta]->update_firsts_and_lasts_tasks();
		sck_to_adp_sck_id = sck_to_adp_sck_id_new;
	}
}

void Pipeline
::exec(std::function<bool(const std::vector<int>&)> stop_condition)
{
	auto &stages = this->stages;
	auto stop_threads = [&stages]()
	{
		for (auto &stage : stages)
			for (auto &m : stage->get_modules<tools::Interface_waiting>())
				m->cancel_waiting();
	};

	std::vector<std::thread> threads;;
	for (size_t s = 0; s < stages.size() -1; s++)
	{
		auto &stage = stages[s];
		threads.push_back(std::thread([&stage, &stop_condition, &stop_threads]()
		{
			stage->exec(stop_condition);
			stop_threads();
		}));
	}
	stages[stages.size() -1]->exec(stop_condition);
	stop_threads();

	for (auto &t : threads)
		t.join();
}

void Pipeline
::exec(std::function<bool()> stop_condition)
{
	auto &stages = this->stages;
	auto stop_threads = [&stages]()
	{
		for (auto &stage : stages)
			for (auto &m : stage->get_modules<tools::Interface_waiting>())
				m->cancel_waiting();
	};

	std::vector<std::thread> threads;;
	for (size_t s = 0; s < stages.size() -1; s++)
	{
		auto &stage = stages[s];
		threads.push_back(std::thread([&stage, &stop_condition, &stop_threads]()
		{
			stage->exec(stop_condition);
			stop_threads();
		}));
	}
	stages[stages.size() -1]->exec(stop_condition);
	stop_threads();

	for (auto &t : threads)
		t.join();
}

const std::vector<std::vector<module::Task*>>& Pipeline
::get_firsts_tasks() const
{
	return this->stages[0]->get_firsts_tasks();
}

const std::vector<std::vector<module::Task*>>& Pipeline
::get_lasts_tasks() const
{
	return this->stages.back()->get_lasts_tasks();
}

std::vector<std::vector<module::Module*>> Pipeline
::get_modules_per_threads() const
{
	std::vector<std::vector<module::Module*>> modules_per_threads;
	for (auto &stage : this->stages)
	{
		auto modules_per_threads_add = stage->get_modules_per_threads();
		if (modules_per_threads_add.size() > modules_per_threads.size())
			modules_per_threads.resize(modules_per_threads_add.size());

		for (size_t t = 0; t < modules_per_threads_add.size(); t++)
			modules_per_threads[t].insert(modules_per_threads    [t].end  (),
			                              modules_per_threads_add[t].begin(),
			                              modules_per_threads_add[t].end  ());
	}
	return modules_per_threads;
}

std::vector<std::vector<module::Module*>> Pipeline
::get_modules_per_types() const
{
	std::vector<std::vector<module::Module*>> modules_per_types;
	for (auto &stage : this->stages)
	{
		auto modules_per_types_add = stage->get_modules_per_types();
		modules_per_types.insert(modules_per_types    .end  (),
		                         modules_per_types_add.begin(),
		                         modules_per_types_add.end  ());
	}
	return modules_per_types;
}

std::vector<std::vector<module::Task*>> Pipeline
::get_tasks_per_threads() const
{
	std::vector<std::vector<module::Task*>> tasks_per_threads;
	for (auto &stage : this->stages)
	{
		auto tasks_per_threads_add = stage->get_tasks_per_threads();
		if (tasks_per_threads_add.size() > tasks_per_threads.size())
			tasks_per_threads.resize(tasks_per_threads_add.size());

		for (size_t t = 0; t < tasks_per_threads_add.size(); t++)
			tasks_per_threads[t].insert(tasks_per_threads    [t].end  (),
			                            tasks_per_threads_add[t].begin(),
			                            tasks_per_threads_add[t].end  ());
	}
	return tasks_per_threads;
}

std::vector<std::vector<module::Task*>> Pipeline
::get_tasks_per_types() const
{
	std::vector<std::vector<module::Task*>> tasks_per_types;
	for (auto &stage : this->stages)
	{
		auto tasks_per_types_add = stage->get_tasks_per_types();
		tasks_per_types.insert(tasks_per_types    .end  (),
		                       tasks_per_types_add.begin(),
		                       tasks_per_types_add.end  ());
	}
	return tasks_per_types;
}
