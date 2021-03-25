#include <type_traits>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ios>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/rang_format/rang_format.h"
#include "Tools/Display/Statistics/Statistics.hpp"

using namespace aff3ct;
using namespace aff3ct::tools;

void Statistics
::separation1(std::ostream &stream)
{
	stream << "# " << rang::style::bold << "-------------------------------------------||------------------------------||--------------------------------||--------------------------------" << rang::style::reset << std::endl;
}

void Statistics
::separation2(std::ostream &stream)
{
	stream << "# " << rang::style::bold << "-------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------" << rang::style::reset << std::endl;
}

void Statistics
::show_header(std::ostream &stream)
{
	Statistics::separation1(stream);
//	stream << "# " << rang::style::bold << "-------------------------------------------||------------------------------||--------------------------------||--------------------------------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "       Statistics for the given task       ||       Basic statistics       ||       Measured throughput      ||        Measured latency        " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "    ('*' = any, '-' = same as previous)    ||          on the task         ||   considering the last socket  ||   considering the last socket  " << rang::style::reset << std::endl;
//	stream << "# " << rang::style::bold << "-------------------------------------------||------------------------------||--------------------------------||--------------------------------" << rang::style::reset << std::endl;
	Statistics::separation1(stream);
	Statistics::separation2(stream);
//	stream << "# " << rang::style::bold << "-------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------" << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "      MODULE |              TASK |   TIMER ||    CALLS |     TIME |   PERC ||  AVERAGE |  MINIMUM |  MAXIMUM ||  AVERAGE |  MINIMUM |  MAXIMUM " << rang::style::reset << std::endl;
	stream << "# " << rang::style::bold << "             |                   |         ||          |      (s) |    (%) ||   (Mb/s) |   (Mb/s) |   (Mb/s) ||     (us) |     (us) |     (us) " << rang::style::reset << std::endl;
//	stream << "# " << rang::style::bold << "-------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------" << rang::style::reset << std::endl;
	Statistics::separation2(stream);
}

void Statistics
::show_task(const float                    total_sec,
            const std::string&             module_name,
            const std::string&             task_name,
            const size_t                   task_n_elmts,
            const uint32_t                 task_n_calls,
            const std::chrono::nanoseconds task_tot_duration,
            const std::chrono::nanoseconds task_min_duration,
            const std::chrono::nanoseconds task_max_duration,
                  std::ostream             &stream)
{
	if (task_n_calls == 0)
		return;

	auto tot_dur = ((float)task_tot_duration.count()) * 0.000000001f;
	auto percent = (tot_dur / total_sec) * 100.f;
	auto avg_thr = (float)(task_n_calls * task_n_elmts) / ((float)task_tot_duration.count() * 0.001f);
	auto min_thr = (float)(1.f          * task_n_elmts) / ((float)task_max_duration.count() * 0.001f);
	auto max_thr = (float)(1.f          * task_n_elmts) / ((float)task_min_duration.count() * 0.001f);
	auto avg_lat = (float)(task_tot_duration.count() * 0.001f) / task_n_calls;
	auto min_lat = (float)(task_min_duration.count() * 0.001f);
	auto max_lat = (float)(task_max_duration.count() * 0.001f);

#ifdef _WIN32
	auto P = 1;
#else
	auto P = 2;
#endif

	unsigned l1 = 99999999;
	float    l2 = 99999.99f;

	std::stringstream ssmodule, ssprocess, sssp, ssn_calls, sstot_dur, sspercent;
	std::stringstream ssavg_thr, ssmin_thr, ssmax_thr;
	std::stringstream ssavg_lat, ssmin_lat, ssmax_lat;

	ssmodule  << std::setprecision(                        2) <<                                        std::fixed  << std::setw(12) << module_name;
	ssprocess << std::setprecision(                        2) <<                                        std::fixed  << std::setw(17) << task_name;
	sssp      << std::setprecision(                        2) <<                                        std::fixed  << std::setw( 7) << "*";
	ssn_calls << std::setprecision(task_n_calls > l1 ? P : 2) << (task_n_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << task_n_calls;
	sstot_dur << std::setprecision(tot_dur      > l1 ? P : 2) << (tot_dur      > l1 ? std::scientific : std::fixed) << std::setw( 8) << tot_dur;
	sspercent << std::setprecision(                        2) <<                                        std::fixed  << std::setw( 6) << percent;
	ssavg_thr << std::setprecision(avg_thr      > l1 ? P : 2) << (avg_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_thr;
	ssmin_thr << std::setprecision(min_thr      > l1 ? P : 2) << (min_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_thr;
	ssmax_thr << std::setprecision(max_thr      > l1 ? P : 2) << (max_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_thr;
	ssavg_lat << std::setprecision(avg_lat      > l1 ? P : 2) << (avg_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_lat;
	ssmin_lat << std::setprecision(min_lat      > l1 ? P : 2) << (min_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_lat;
	ssmax_lat << std::setprecision(max_lat      > l1 ? P : 2) << (max_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_lat;

	stream << "# ";
	stream << ssmodule .str() << rang::style::bold << " | "  << rang::style::reset
	       << ssprocess.str() << rang::style::bold << " | "  << rang::style::reset
	       << sssp     .str() << rang::style::bold << " || " << rang::style::reset
	       << ssn_calls.str() << rang::style::bold << " | "  << rang::style::reset
	       << sstot_dur.str() << rang::style::bold << " | "  << rang::style::reset;

	     if (percent > 50.0f) stream << rang::fg::red    << sspercent.str() << rang::style::reset;
	else if (percent > 25.0f) stream << rang::fg::yellow << sspercent.str() << rang::style::reset;
	else if (percent > 12.5f) stream << rang::fg::green  << sspercent.str() << rang::style::reset;
	else if (percent <  5.0f) stream << rang::fg::gray   << sspercent.str() << rang::style::reset;
	else                      stream <<                     sspercent.str();

	stream <<                    rang::style::bold << " || " << rang::style::reset
	       << ssavg_thr.str() << rang::style::bold << " | "  << rang::style::reset
	       << ssmin_thr.str() << rang::style::bold << " | "  << rang::style::reset
	       << ssmax_thr.str() << rang::style::bold << " || " << rang::style::reset
	       << ssavg_lat.str() << rang::style::bold << " | "  << rang::style::reset
	       << ssmin_lat.str() << rang::style::bold << " | "  << rang::style::reset
	       << ssmax_lat.str() << ""
	       << std::endl;
}

void Statistics
::show_timer(const float                    total_sec,
             const uint32_t                 task_n_calls,
             const size_t                   timer_n_elmts,
             const std::string&             timer_name,
             const uint32_t                 timer_n_calls,
             const std::chrono::nanoseconds timer_tot_duration,
             const std::chrono::nanoseconds timer_min_duration,
             const std::chrono::nanoseconds timer_max_duration,
                   std::ostream             &stream)
{
	if (task_n_calls == 0 || timer_n_calls == 0)
		return;

	auto rn_elmts = (timer_n_elmts * task_n_calls) / timer_n_calls;
	auto rtot_dur = ((float)timer_tot_duration.count()) * 0.000000001f;
	auto rpercent = (rtot_dur / total_sec) * 100.f;
	auto ravg_thr = (float)(timer_n_calls * rn_elmts) / ((float)timer_tot_duration.count() * 0.001f);
	auto rmin_thr = (float)(1.f           * rn_elmts) / ((float)timer_max_duration.count() * 0.001f);
	auto rmax_thr = (float)(1.f           * rn_elmts) / ((float)timer_min_duration.count() * 0.001f);
	auto ravg_lat = (float)(timer_tot_duration.count() * 0.001f) / timer_n_calls;
	auto rmin_lat = (float)(timer_min_duration.count() * 0.001f);
	auto rmax_lat = (float)(timer_max_duration.count() * 0.001f);

#ifdef _WIN32
	auto P = 1;
#else
	auto P = 2;
#endif

	unsigned l1 = 99999999;
	float    l2 = 99999.99f;

	std::stringstream spaces, ssprocess, sssp, ssrn_calls, ssrtot_dur, ssrpercent;
	std::stringstream ssravg_thr, ssrmin_thr, ssrmax_thr;
	std::stringstream ssravg_lat, ssrmin_lat, ssrmax_lat;

	spaces     <<                                                                                          std::fixed  << std::setw(12) << "-";
	ssprocess  << std::setprecision(                         2) <<                                         std::fixed  << std::setw(17) << "-";
	sssp       << std::setprecision(                         2) <<                                         std::fixed  << std::setw( 7) << timer_name;
	ssrn_calls << std::setprecision(timer_n_calls > l1 ? P : 2) << (timer_n_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << timer_n_calls;
	ssrtot_dur << std::setprecision(rtot_dur      > l1 ? P : 2) << (rtot_dur      > l1 ? std::scientific : std::fixed) << std::setw( 8) << rtot_dur;
	ssrpercent << std::setprecision(                         2) <<                                         std::fixed  << std::setw( 6) << rpercent;
	ssravg_thr << std::setprecision(ravg_thr      > l1 ? P : 2) << (ravg_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_thr;
	ssrmin_thr << std::setprecision(rmin_thr      > l1 ? P : 2) << (rmin_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_thr;
	ssrmax_thr << std::setprecision(rmax_thr      > l1 ? P : 2) << (rmax_thr      > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_thr;
	ssravg_lat << std::setprecision(ravg_lat      > l1 ? P : 2) << (ravg_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_lat;
	ssrmin_lat << std::setprecision(rmin_lat      > l1 ? P : 2) << (rmin_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_lat;
	ssrmax_lat << std::setprecision(rmax_lat      > l1 ? P : 2) << (rmax_lat      > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_lat;

	stream << "# ";
	stream << spaces.str()                                                  << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssprocess .str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << sssp      .str() << rang::style::reset << rang::style::bold << " || " << rang::style::reset
	       << rang::style::italic << ssrn_calls.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrtot_dur.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrpercent.str() << rang::style::reset << rang::style::bold << " || " << rang::style::reset
	       << rang::style::italic << ssravg_thr.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrmin_thr.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrmax_thr.str() << rang::style::reset << rang::style::bold << " || " << rang::style::reset
	       << rang::style::italic << ssravg_lat.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrmin_lat.str() << rang::style::reset << rang::style::bold << " | "  << rang::style::reset
	       << rang::style::italic << ssrmax_lat.str() << rang::style::reset << ""
	       << std::endl;
}

template <class MODULE_OR_TASK>
void Statistics
::show(std::vector<MODULE_OR_TASK*> modules_or_tasks, const bool ordered, std::ostream &stream)
{
	std::stringstream message;
	message << "The 'Statistics::show' method expect a 'std::vector' of 'module::Module' or 'module::Task'.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
}

namespace aff3ct {
namespace tools {

template <>
void Statistics
::show<module::Module>(std::vector<module::Module*> modules, const bool ordered, std::ostream &stream)
{
	Statistics::show_modules<module::Module>(modules, ordered, stream);
}

template <>
void Statistics
::show<const module::Module>(std::vector<const module::Module*> modules, const bool ordered, std::ostream &stream)
{
	Statistics::show_modules<const module::Module>(modules, ordered, stream);
}

template <>
void Statistics
::show<module::Task>(std::vector<module::Task*> tasks, const bool ordered, std::ostream &stream)
{
	Statistics::show_tasks<module::Task>(tasks, ordered, stream);
}

template <>
void Statistics
::show<const module::Task>(std::vector<const module::Task*> tasks, const bool ordered, std::ostream &stream)
{
	Statistics::show_tasks<const module::Task>(tasks, ordered, stream);
}

}
}

template <class MODULE>
void Statistics
::show_modules(std::vector<MODULE*> modules, const bool ordered, std::ostream &stream)
{
	std::vector<const module::Task*> tasks;
	for (auto& m : modules)
		if (m != nullptr)
			for (auto& t : m->tasks)
				if (t->get_n_calls())
					tasks.push_back(t.get());

	Statistics::show_tasks(tasks, ordered, stream);
}

template <class TASK>
void Statistics
::show_tasks(std::vector<TASK*> tasks, const bool ordered, std::ostream &stream)
{
	for (size_t t = 0; t < tasks.size(); t++)
		if (tasks[t] == nullptr)
			tasks.erase(tasks.begin()+t);

	if (ordered)
	{
		std::sort(tasks.begin(), tasks.end(), [](const module::Task* t1, const module::Task* t2)
		{
			return t1->get_duration_total() > t2->get_duration_total();
		});
	}

	auto ttask_tot_duration = std::chrono::nanoseconds(0);
	auto ttask_min_duration = std::chrono::nanoseconds(0);
	auto ttask_max_duration = std::chrono::nanoseconds(0);

	for (auto *t : tasks)
		ttask_tot_duration += t->get_duration_total();
	auto total_sec = ((float)ttask_tot_duration.count()) * 0.000000001f;

	if (ttask_tot_duration.count())
	{
		Statistics::show_header(stream);

		size_t   ttask_n_elmts = 0;
		uint32_t ttask_n_calls = 0;

		auto is_first = true;
		for (auto *t : tasks)
		{
			auto task_n_elmts = t->sockets.end()[-2]->get_n_elmts();
			auto task_n_calls = t->get_n_calls();

			if (is_first)
			{
				if (task_n_calls)
				{
					ttask_n_elmts = task_n_elmts;
					ttask_n_calls = task_n_calls;
					is_first = false;
				}
			}
			else
			{
				ttask_n_elmts = task_n_elmts ? std::min(ttask_n_elmts, task_n_elmts) : ttask_n_elmts;
				ttask_n_calls = task_n_calls ? std::min(ttask_n_calls, task_n_calls) : ttask_n_calls;
			}
		}

		for (auto *t : tasks)
		{
			auto module_name       = t->get_module().get_custom_name().empty() ? t->get_module().get_short_name() :
			                                                                     t->get_module().get_custom_name();
			auto task_n_elmts      = t->sockets.end()[-2]->get_n_elmts();
			auto task_name         = t->get_name          ();
			auto task_n_calls      = t->get_n_calls       ();
			auto task_tot_duration = t->get_duration_total();
			auto task_min_duration = t->get_duration_min  ();
			auto task_max_duration = t->get_duration_max  ();

			ttask_min_duration += (task_min_duration * task_n_calls) / ttask_n_calls;
			ttask_max_duration += (task_max_duration * task_n_calls) / ttask_n_calls;

			Statistics::show_task(total_sec, module_name, task_name, task_n_elmts, task_n_calls,
			                      task_tot_duration, task_min_duration, task_max_duration, stream);

			auto task_total_sec = ((float)task_tot_duration.count()) * 0.000000001f;

			auto timers_name         = t->get_timers_name();
			auto timers_n_elmts      = task_n_elmts;
			auto timers_n_calls      = t->get_timers_n_calls();
			auto timers_tot_duration = t->get_timers_total();
			auto timers_min_duration = t->get_timers_min();
			auto timers_max_duration = t->get_timers_max();

			for (size_t i = 0; i < timers_name.size(); i++)
			{
				Statistics::show_timer(task_total_sec, task_n_calls, timers_n_elmts,
				                       timers_name[i], timers_n_calls[i], timers_tot_duration[i],
				                       timers_min_duration[i], timers_max_duration[i], stream);
			}
		}
		Statistics::separation2(stream);

		Statistics::show_task(total_sec, "TOTAL", "*", ttask_n_elmts, ttask_n_calls,
		                      ttask_tot_duration, ttask_min_duration, ttask_max_duration, stream);
	}
	else
	{
		stream << rang::tag::comment << rang::tag::info
		       << "Statistics are unavailable. Did you enable the statistics in the tasks?" << std::endl;
	}
}

template <class MODULE_OR_TASK>
void Statistics
::show(std::vector<std::vector<MODULE_OR_TASK*>> modules_or_tasks, const bool ordered, std::ostream &stream)
{
	std::stringstream message;
	message << "The 'Statistics::show' method expect a 'std::vector' of 'std::vector' of 'module::Module' or "
	        << "'module::Task'.";
	throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
}

namespace aff3ct {
namespace tools {

template <>
void Statistics
::show<module::Module>(std::vector<std::vector<module::Module*>> modules, const bool ordered, std::ostream &stream)
{
	Statistics::show_modules<module::Module>(modules, ordered, stream);
}

template <>
void Statistics
::show<const module::Module>(std::vector<std::vector<const module::Module*>> modules, const bool ordered, std::ostream &stream)
{
	Statistics::show_modules<const module::Module>(modules, ordered, stream);
}

template <>
void Statistics
::show<module::Task>(std::vector<std::vector<module::Task*>> tasks, const bool ordered, std::ostream &stream)
{
	Statistics::show_tasks<module::Task>(tasks, ordered, stream);
}

template <>
void Statistics
::show<const module::Task>(std::vector<std::vector<const module::Task*>> tasks, const bool ordered, std::ostream &stream)
{
	Statistics::show_tasks<const module::Task>(tasks, ordered, stream);
}

}
}

template <class MODULE>
void Statistics
::show_modules(std::vector<std::vector<MODULE*>> modules, const bool ordered, std::ostream &stream)
{
	std::vector<std::vector<const module::Task*>> tasks;
	for (auto &vm : modules)
		if (vm.size() > 0 && vm[0] != nullptr)
		{
			auto &tasks0 = vm[0]->tasks;
			for (size_t t = 0; t < tasks0.size(); t++)
			{
				std::vector<const module::Task*> tsk;
				for (auto& m : vm)
					tsk.push_back(m->tasks[t].get());
				tasks.push_back(tsk);
			}
		}

	Statistics::show_tasks(tasks, ordered, stream);
}

template <class TASK>
void Statistics
::show_tasks(std::vector<std::vector<TASK*>> tasks, const bool ordered, std::ostream &stream)
{
	using namespace std::chrono;

	if (tasks.size())
		for (size_t t = 0; t < tasks[0].size(); t++)
			if (tasks[0][t] == nullptr)
				for (size_t i = 0; i < tasks.size(); i++)
					tasks[i].erase(tasks[i].begin()+t);

	if (ordered)
	{
		std::sort(tasks.begin(), tasks.end(), [](const std::vector<TASK*> &t1,
		                                         const std::vector<TASK*> &t2)
		{
			auto total1 = nanoseconds(0);
			auto total2 = nanoseconds(0);
			for (auto *t : t1) total1 += t->get_duration_total();
			for (auto *t : t2) total2 += t->get_duration_total();
			return total1 > total2;
		});
	}


	auto ttask_tot_duration = nanoseconds(0);
	auto ttask_min_duration = nanoseconds(0);
	auto ttask_max_duration = nanoseconds(0);

	for (auto &vt : tasks)
		for (auto *t : vt)
			ttask_tot_duration += t->get_duration_total();
	auto total_sec = ((float)ttask_tot_duration.count()) * 0.000000001f;

	if (ttask_tot_duration.count())
	{
		Statistics::show_header(stream);

		size_t ttask_n_elmts = 0;
		auto   ttask_n_calls = 0;

		auto is_first = true;
		for (auto &vt : tasks)
		{
			auto task_n_elmts = vt[0]->sockets.size() >= 2 ? vt[0]->sockets.end()[-2]->get_n_elmts() : 0;
			auto task_n_calls = 0;

			for (auto *t : vt)
				task_n_calls += t->get_n_calls();

			if (is_first)
			{
				if (task_n_calls)
				{
					ttask_n_elmts = task_n_elmts;
					ttask_n_calls = task_n_calls;
					is_first = false;
				}
			}
			else
			{
				ttask_n_elmts = task_n_elmts ? std::min(ttask_n_elmts, task_n_elmts) : ttask_n_elmts;
				ttask_n_calls = task_n_calls ? std::min(ttask_n_calls, task_n_calls) : ttask_n_calls;
			}
		}

		for (auto &vt : tasks)
		{

			auto module_name       = vt[0]->get_module().get_custom_name().empty() ?
			                         vt[0]->get_module().get_short_name() :
			                         vt[0]->get_module().get_custom_name();
			auto task_n_elmts      = vt[0]->sockets.size() >= 2 ? vt[0]->sockets.end()[-2]->get_n_elmts() : 0;
			auto task_name         = vt[0]->get_name();
			auto task_n_calls      = 0;
			auto task_tot_duration = nanoseconds(0);
			auto task_min_duration = ttask_tot_duration;
			auto task_max_duration = nanoseconds(0);

			for (auto *t : vt)
			{
				task_n_calls      += t->get_n_calls();
				task_tot_duration += t->get_duration_total();
				task_min_duration  = std::min(task_min_duration, t->get_duration_min());
				task_max_duration  = std::max(task_max_duration, t->get_duration_max());
			}

			ttask_min_duration += (task_min_duration * task_n_calls) / ttask_n_calls;
			ttask_max_duration += (task_max_duration * task_n_calls) / ttask_n_calls;

			Statistics::show_task(total_sec, module_name, task_name, task_n_elmts, task_n_calls,
			                      task_tot_duration, task_min_duration, task_max_duration, stream);

			auto task_total_sec = ((float)task_tot_duration.count()) * 0.000000001f;

			auto timers_name         = vt[0]->get_timers_name();
			auto timers_n_elmts      = task_n_elmts;
			auto timers_n_calls      = std::vector<uint32_t>(timers_name.size(), 0);
			auto timers_tot_duration = std::vector<nanoseconds>(timers_name.size(), nanoseconds(0));
			auto timers_min_duration = std::vector<nanoseconds>(timers_name.size(), ttask_tot_duration);
			auto timers_max_duration = std::vector<nanoseconds>(timers_name.size(), nanoseconds(0));

			for (size_t tn = 0; tn < vt[0]->get_timers_name().size(); tn++)
			{
				for (auto *t : vt)
				{
					timers_n_calls     [tn] += t->get_timers_n_calls()[tn];
					timers_tot_duration[tn] += t->get_timers_total()[tn];
					timers_min_duration[tn]  = std::min(task_min_duration, t->get_timers_min()[tn]);
					timers_max_duration[tn]  = std::max(task_max_duration, t->get_timers_max()[tn]);
				}

				Statistics::show_timer(task_total_sec, task_n_calls, timers_n_elmts,
				                       timers_name[tn], timers_n_calls[tn], timers_tot_duration[tn],
				                       timers_min_duration[tn], timers_max_duration[tn], stream);
			}
		}
		Statistics::separation2(stream);

		Statistics::show_task(total_sec, "TOTAL", "*", ttask_n_elmts, ttask_n_calls,
		                      ttask_tot_duration, ttask_min_duration, ttask_max_duration, stream);
	}
	else
	{
		stream << rang::tag::comment << rang::tag::info
		       << "Statistics are unavailable. Did you enable the statistics in the tasks?" << std::endl;
	}
}
