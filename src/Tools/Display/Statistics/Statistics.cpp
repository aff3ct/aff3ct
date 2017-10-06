#include <iomanip>

#include "Tools/Display/bash_tools.h"

#include "Statistics.hpp"

using namespace aff3ct::tools;

Statistics
::Statistics()
{
}

Statistics
::~Statistics()
{
}

void Statistics
::show_header(std::ostream &stream)
{
	stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
	stream << tools::format("#        Statistics for the given task       ||       Basic statistics       ||       Measured throughput      ||        Measured latency        ", tools::Style::BOLD) << std::endl;
	stream << tools::format("#          ('*' = all the sub-tasks)         ||          on the task         ||   considering the last socket  ||   considering the last socket  ", tools::Style::BOLD) << std::endl;
	stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
	stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;
	stream << tools::format("#       MODULE |              TASK |     SUB ||    CALLS |     TIME |   PERC ||  AVERAGE |  MINIMUM |  MAXIMUM ||  AVERAGE |  MINIMUM |  MAXIMUM ", tools::Style::BOLD) << std::endl;
	stream << tools::format("#              |                   |    TASK ||          |      (s) |    (%) ||   (Mb/s) |   (Mb/s) |   (Mb/s) ||     (us) |     (us) |     (us) ", tools::Style::BOLD) << std::endl;
	stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;
}

void Statistics
::show_task(const float                    total_sec,
            const std::string              module_sname,
            const std::string              task_name,
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
	float    l2 = 99999.99;

	std::stringstream ssmodule, ssprocess, sssp, ssn_calls, sstot_dur, sspercent;
	std::stringstream ssavg_thr, ssmin_thr, ssmax_thr;
	std::stringstream ssavg_lat, ssmin_lat, ssmax_lat;

	ssmodule  << std::setprecision(                        2) <<                                        std::fixed  << std::setw(12) << module_sname;
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

	std::string spercent = sspercent.str();
	     if (percent > 50.0f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::RED);
	else if (percent > 25.0f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::ORANGE);
	else if (percent > 12.5f) spercent = tools::format(spercent, tools::FG::INTENSE | tools::FG::GREEN);
	else if (percent <  5.0f) spercent = tools::format(spercent,                      tools::FG::GRAY);

	stream << "# ";
	stream << ssmodule .str() << tools::format(" | ",  tools::Style::BOLD)
	       << ssprocess.str() << tools::format(" | ",  tools::Style::BOLD)
	       << sssp     .str() << tools::format(" || ", tools::Style::BOLD)
	       << ssn_calls.str() << tools::format(" | ",  tools::Style::BOLD)
	       << sstot_dur.str() << tools::format(" | ",  tools::Style::BOLD)
	       << spercent        << tools::format(" || ", tools::Style::BOLD)
	       << ssavg_thr.str() << tools::format(" | ",  tools::Style::BOLD)
	       << ssmin_thr.str() << tools::format(" | ",  tools::Style::BOLD)
	       << ssmax_thr.str() << tools::format(" || ", tools::Style::BOLD)
	       << ssavg_lat.str() << tools::format(" | ",  tools::Style::BOLD)
	       << ssmin_lat.str() << tools::format(" | ",  tools::Style::BOLD)
	       << ssmax_lat.str() << ""
	       << std::endl;
}

void Statistics
::show_sub_task(const float                    total_sec,
                const std::string              task_name,
                const uint32_t                 task_n_calls,
                const size_t                   subtask_n_elmts,
                const std::string              subtask_name,
                const uint32_t                 subtask_n_calls,
                const std::chrono::nanoseconds subtask_tot_duration,
                const std::chrono::nanoseconds subtask_min_duration,
                const std::chrono::nanoseconds subtask_max_duration,
                      std::ostream             &stream)
{
	if (task_n_calls == 0 || subtask_n_calls == 0)
		return;

	auto rn_elmts = (subtask_n_elmts * task_n_calls) / subtask_n_calls;
	auto rtot_dur = ((float)subtask_tot_duration.count()) * 0.000000001f;
	auto rpercent = (rtot_dur / total_sec) * 100.f;
	auto ravg_thr = (float)(subtask_n_calls * rn_elmts) / ((float)subtask_tot_duration.count() * 0.001f);
	auto rmin_thr = (float)(1.f             * rn_elmts) / ((float)subtask_max_duration.count() * 0.001f);
	auto rmax_thr = (float)(1.f             * rn_elmts) / ((float)subtask_min_duration.count() * 0.001f);
	auto ravg_lat = (float)(subtask_tot_duration.count() * 0.001f) / subtask_n_calls;
	auto rmin_lat = (float)(subtask_min_duration.count() * 0.001f);
	auto rmax_lat = (float)(subtask_max_duration.count() * 0.001f);

#ifdef _WIN32
	auto P = 1;
#else
	auto P = 2;
#endif

	unsigned l1 = 99999999;
	float    l2 = 99999.99;

	std::stringstream spaces, ssprocess, sssp, ssrn_calls, ssrtot_dur, ssrpercent;
	std::stringstream ssravg_thr, ssrmin_thr, ssrmax_thr;
	std::stringstream ssravg_lat, ssrmin_lat, ssrmax_lat;

	spaces     <<                                                                                              std::fixed  << std::setw(12) << "-";
	ssprocess  << std::setprecision(                           2) <<                                           std::fixed  << std::setw(17) << task_name;
	sssp       << std::setprecision(                           2) <<                                           std::fixed  << std::setw( 7) << subtask_name;
	ssrn_calls << std::setprecision(subtask_n_calls > l1 ? P : 2) << (subtask_n_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << subtask_n_calls;
	ssrtot_dur << std::setprecision(rtot_dur        > l1 ? P : 2) << (rtot_dur        > l1 ? std::scientific : std::fixed) << std::setw( 8) << rtot_dur;
	ssrpercent << std::setprecision(                           2) <<                                           std::fixed  << std::setw( 6) << rpercent;
	ssravg_thr << std::setprecision(ravg_thr        > l1 ? P : 2) << (ravg_thr        > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_thr;
	ssrmin_thr << std::setprecision(rmin_thr        > l1 ? P : 2) << (rmin_thr        > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_thr;
	ssrmax_thr << std::setprecision(rmax_thr        > l1 ? P : 2) << (rmax_thr        > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_thr;
	ssravg_lat << std::setprecision(ravg_lat        > l1 ? P : 2) << (ravg_lat        > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_lat;
	ssrmin_lat << std::setprecision(rmin_lat        > l1 ? P : 2) << (rmin_lat        > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_lat;
	ssrmax_lat << std::setprecision(rmax_lat        > l1 ? P : 2) << (rmax_lat        > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_lat;

	stream << "# ";
	stream << spaces.str()                                          << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssprocess .str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(sssp      .str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
	       << tools::format(ssrn_calls.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrtot_dur.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrpercent.str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
	       << tools::format(ssravg_thr.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrmin_thr.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrmax_thr.str(), tools::Style::ITALIC) << tools::format(" || ", tools::Style::BOLD)
	       << tools::format(ssravg_lat.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrmin_lat.str(), tools::Style::ITALIC) << tools::format(" | ",  tools::Style::BOLD)
	       << tools::format(ssrmax_lat.str(), tools::Style::ITALIC) << ""
	       << std::endl;
}

void Statistics
::show(std::vector<module::Module*> &modules, std::ostream &stream)
{
	size_t max_chars = 0;
	auto d_total = std::chrono::nanoseconds(0);
	for (auto *m : modules)
		for (auto &t : m->tasks)
		{
			d_total += t.second->get_duration_total();
			std::max(max_chars, m->get_short_name().size() + t.second->get_name().size());
		}
	auto total_sec = ((float)d_total.count()) * 0.000000001f;

	if (d_total.count())
	{
		Statistics::show_header(stream);

		for (auto *m : modules)
		{
			for (auto &t : m->tasks)
			{
				auto module_sname      = m->get_short_name();
				auto task_n_elmts      = t.second->socket.back().get_n_elmts();
				auto task_name         = t.second->get_name          ();
				auto task_n_calls      = t.second->get_n_calls       ();
				auto task_tot_duration = t.second->get_duration_total();
				auto task_min_duration = t.second->get_duration_min  ();
				auto task_max_duration = t.second->get_duration_max  ();

				Statistics::show_task(total_sec, module_sname, task_name, task_n_elmts, task_n_calls,
				                      task_tot_duration, task_min_duration, task_max_duration, stream);

				auto task_total_sec = ((float)task_tot_duration.count()) * 0.000000001f;
				for (auto &sp : t.second->get_registered_duration())
				{
					auto subtask_name         = sp;
					auto subtask_n_elmts      = task_n_elmts;
					auto subtask_n_calls      = t.second->get_registered_n_calls       (sp);
					auto subtask_tot_duration = t.second->get_registered_duration_total(sp);
					auto subtask_min_duration = t.second->get_registered_duration_min  (sp);
					auto subtask_max_duration = t.second->get_registered_duration_max  (sp);

					Statistics::show_sub_task(task_total_sec, task_name, task_n_calls, subtask_n_elmts,
					                          subtask_name, subtask_n_calls, subtask_tot_duration,
					                          subtask_min_duration, subtask_max_duration, stream);
				}
			}
		}
	}
	else
	{
		stream << "# Statistics are unavailable (did you enable the statistics in the tasks?)." << std::endl;
	}
}

void Statistics
::show(std::vector<std::vector<module::Module*>> &modules, std::ostream &stream)
{
	size_t max_chars = 0;
	auto d_total = std::chrono::nanoseconds(0);
	for (auto &vm : modules)
		for (auto *m : vm)
			for (auto &t : m->tasks)
			{
				d_total += t.second->get_duration_total();
				std::max(max_chars, m->get_short_name().size() + t.second->get_name().size());
			}
	auto total_sec = ((float)d_total.count()) * 0.000000001f;

	if (d_total.count())
	{
		Statistics::show_header(stream);

		for (auto &vm : modules)
		{
			for (auto &t : vm[0]->tasks)
			{
				auto module_sname      = vm[0]->get_short_name();
				auto task_n_elmts      = t.second->socket.back().get_n_elmts();
				auto task_name         = t.second->get_name();
				auto task_n_calls      = 0;
				auto task_tot_duration = std::chrono::nanoseconds(0);
				auto task_min_duration = d_total;
				auto task_max_duration = std::chrono::nanoseconds(0);

				for (auto *m : vm)
				{
					task_n_calls      += (*m)[task_name].get_n_calls();
					task_tot_duration += (*m)[task_name].get_duration_total();
					task_min_duration  = std::min(task_min_duration, (*m)[task_name].get_duration_min());
					task_max_duration  = std::max(task_max_duration, (*m)[task_name].get_duration_max());
				}

				Statistics::show_task(total_sec, module_sname, task_name, task_n_elmts, task_n_calls,
				                      task_tot_duration, task_min_duration, task_max_duration, stream);

				auto task_total_sec = ((float)task_tot_duration.count()) * 0.000000001f;
				for (auto &sp : t.second->get_registered_duration())
				{
					auto subtask_name         = sp;
					auto subtask_n_elmts      = task_n_elmts;
					auto subtask_n_calls      = 0;
					auto subtask_tot_duration = std::chrono::nanoseconds(0);
					auto subtask_min_duration = d_total;
					auto subtask_max_duration = std::chrono::nanoseconds(0);

					for (auto *m : vm)
					{
						subtask_n_calls      += (*m)[task_name].get_registered_n_calls(sp);
						subtask_tot_duration += (*m)[task_name].get_registered_duration_total(sp);
						subtask_min_duration  = std::min(task_min_duration, (*m)[task_name].get_registered_duration_min(sp));
						subtask_max_duration  = std::max(task_max_duration, (*m)[task_name].get_registered_duration_max(sp));
					}

					Statistics::show_sub_task(task_total_sec, task_name, task_n_calls, subtask_n_elmts,
					                          subtask_name, subtask_n_calls, subtask_tot_duration,
					                          subtask_min_duration, subtask_max_duration, stream);
				}
			}
		}
	}
	else
	{
		stream << "# Statistics are unavailable (did you enable the statistics in the tasks?)." << std::endl;
	}
}
