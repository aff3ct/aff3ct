#include <iomanip>

#include "Tools/Display/bash_tools.h"

#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation
::Simulation(const factory::Simulation::parameters& simu_params)
: stop_terminal(false),
  params(simu_params),
  terminal(nullptr)
{
}

Simulation
::~Simulation()
{
}

void Simulation
::build_communication_chain()
{
	_build_communication_chain();

	// enable the debug mode in the modules
	if (params.debug)
		for (auto &m : modules)
			for (auto mm : m.second)
				if (mm != nullptr)
					for (auto &t : mm->tasks)
					{
						t.second->set_debug(true);
						if (params.debug_limit)
							t.second->set_debug_limit((uint32_t)params.debug_limit);
						if (params.debug_precision)
							t.second->set_debug_precision((uint8_t)params.debug_precision);
					}
}

void Simulation
::display_stats(std::ostream &stream)
{
	size_t max_chars = 0;
	auto d_total = std::chrono::nanoseconds(0);
	for (auto &m : modules)
		for (auto i = 0; i < (int)m.second.size(); i++)
			if (m.second[i] != nullptr)
				for (auto &t : m.second[i]->tasks)
				{
					d_total += t.second->get_duration_total();
					std::max(max_chars, m.second[i]->get_short_name().size() + t.second->get_name().size());
				}
	auto total_sec = ((float)d_total.count()) * 0.000000001f;

	if (d_total.count())
	{
		stream << "#" << std::endl;
		stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
		stream << tools::format("#      Statistics for the given process      ||       Basic statistics       ||       Measured throughput      ||        Measured latency        ", tools::Style::BOLD) << std::endl;
		stream << tools::format("#        ('*' = all the sub-processes)       ||        on the process        ||     considering the output     ||     considering the output     ", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------------------------------------||------------------------------||--------------------------------||--------------------------------", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;
		stream << tools::format("#       MODULE |           PROCESS |     SUB ||    CALLS |     TIME |   PERC ||  AVERAGE |  MINIMUM |  MAXIMUM ||  AVERAGE |  MINIMUM |  MAXIMUM ", tools::Style::BOLD) << std::endl;
		stream << tools::format("#              |                   | PROCESS ||          |      (s) |    (%) ||   (Mb/s) |   (Mb/s) |   (Mb/s) ||     (us) |     (us) |     (us) ", tools::Style::BOLD) << std::endl;
		stream << tools::format("# -------------|-------------------|---------||----------|----------|--------||----------|----------|----------||----------|----------|----------", tools::Style::BOLD) << std::endl;

#ifdef _WIN32
		auto P = 1;
#else
		auto P = 2;
#endif

		for (auto &m : modules)
		{
			if (m.second[0] != nullptr)
			{
				for (auto &t : m.second[0]->tasks)
				{
					size_t n_elmts = t.second->socket.back().get_n_elmts();

					auto module = m.second[0]->get_short_name();
					auto process = t.second->get_name();
					uint32_t n_calls = 0;
					auto tot_duration = std::chrono::nanoseconds(0);
					auto min_duration = m.second[0]->tasks[t.first]->get_duration_max();
					auto max_duration = std::chrono::nanoseconds(0);
					for (auto &mm : m.second)
					{
						n_calls += mm->tasks[t.first]->get_n_calls();
						tot_duration += mm->tasks[t.first]->get_duration_total();
						min_duration = std::min(min_duration, mm->tasks[t.first]->get_duration_min());
						max_duration = std::max(max_duration, mm->tasks[t.first]->get_duration_max());
					}

					if (n_calls)
					{
						auto tot_dur = ((float)tot_duration.count()) * 0.000000001f;
						auto percent = (tot_dur / total_sec) * 100.f;
						auto avg_thr = (float)(n_calls * n_elmts) / ((float)tot_duration.count() * 0.001f);
						auto min_thr = (float)(1.f     * n_elmts) / ((float)max_duration.count() * 0.001f);
						auto max_thr = (float)(1.f     * n_elmts) / ((float)min_duration.count() * 0.001f);
						auto avg_lat = (float)(tot_duration.count() * 0.001f) / n_calls;
						auto min_lat = (float)(min_duration.count() * 0.001f);
						auto max_lat = (float)(max_duration.count() * 0.001f);

						unsigned l1 = 99999999;
						unsigned l2 = 99999.99;

						std::stringstream ssmodule, ssprocess, sssp, ssn_calls, sstot_dur, sspercent;
						std::stringstream ssavg_thr, ssmin_thr, ssmax_thr;
						std::stringstream ssavg_lat, ssmin_lat, ssmax_lat;

						ssmodule  << std::setprecision(                   2) <<                                   std::fixed  << std::setw(12) << module;
						ssprocess << std::setprecision(                   2) <<                                   std::fixed  << std::setw(17) << process;
						sssp      << std::setprecision(                   2) <<                                   std::fixed  << std::setw( 7) << "*";
						ssn_calls << std::setprecision(n_calls > l1 ? P : 2) << (n_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << n_calls;
						sstot_dur << std::setprecision(tot_dur > l1 ? P : 2) << (tot_dur > l1 ? std::scientific : std::fixed) << std::setw( 8) << tot_dur;
						sspercent << std::setprecision(                   2) <<                                   std::fixed  << std::setw( 6) << percent;
						ssavg_thr << std::setprecision(avg_thr > l1 ? P : 2) << (avg_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_thr;
						ssmin_thr << std::setprecision(min_thr > l1 ? P : 2) << (min_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_thr;
						ssmax_thr << std::setprecision(max_thr > l1 ? P : 2) << (max_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_thr;
						ssavg_lat << std::setprecision(avg_lat > l1 ? P : 2) << (avg_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << avg_lat;
						ssmin_lat << std::setprecision(min_lat > l1 ? P : 2) << (min_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << min_lat;
						ssmax_lat << std::setprecision(max_lat > l1 ? P : 2) << (max_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << max_lat;

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

						for (auto &sp : t.second->get_registered_duration())
						{
							uint32_t rn_calls = 0;
							auto rtot_duration = std::chrono::nanoseconds(0);
							auto rmin_duration = m.second[0]->tasks[t.first]->get_registered_duration_max(sp);
							auto rmax_duration = std::chrono::nanoseconds(0);
							for (auto &mm : m.second)
							{
								rn_calls += mm->tasks[t.first]->get_registered_n_calls(sp);
								rtot_duration += mm->tasks[t.first]->get_registered_duration_total(sp);
								rmin_duration = std::min(rmin_duration, mm->tasks[t.first]->get_registered_duration_min(sp));
								rmax_duration = std::max(rmax_duration, mm->tasks[t.first]->get_registered_duration_max(sp));
							}

							if (rn_calls)
							{
								auto rn_elmts = (n_elmts * n_calls) / rn_calls;
								auto rtot_dur = ((float)rtot_duration.count()) * 0.000000001f;
								auto rpercent = (rtot_dur / tot_dur) * 100.f;
								auto ravg_thr = (float)(rn_calls * rn_elmts) / ((float)rtot_duration.count() * 0.001f);
								auto rmin_thr = (float)(1.f      * rn_elmts) / ((float)rmax_duration.count() * 0.001f);
								auto rmax_thr = (float)(1.f      * rn_elmts) / ((float)rmin_duration.count() * 0.001f);
								auto ravg_lat = (float)(rtot_duration.count() * 0.001f) / rn_calls;
								auto rmin_lat = (float)(rmin_duration.count() * 0.001f);
								auto rmax_lat = (float)(rmax_duration.count() * 0.001f);

								std::stringstream spaces, sssp, ssrn_calls, ssrtot_dur, ssrpercent;
								std::stringstream ssravg_thr, ssrmin_thr, ssrmax_thr;
								std::stringstream ssravg_lat, ssrmin_lat, ssrmax_lat;

								spaces     <<                                                                                std::fixed  << std::setw(12) << "-";
								sssp       << std::setprecision(                    2) <<                                    std::fixed  << std::setw( 7) << sp;
								ssrn_calls << std::setprecision(rn_calls > l1 ? P : 2) << (rn_calls > l1 ? std::scientific : std::fixed) << std::setw( 8) << rn_calls;
								ssrtot_dur << std::setprecision(rtot_dur > l1 ? P : 2) << (rtot_dur > l1 ? std::scientific : std::fixed) << std::setw( 8) << rtot_dur;
								ssrpercent << std::setprecision(                    2) <<                                    std::fixed  << std::setw( 6) << rpercent;
								ssravg_thr << std::setprecision(ravg_thr > l1 ? P : 2) << (ravg_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_thr;
								ssrmin_thr << std::setprecision(rmin_thr > l1 ? P : 2) << (rmin_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_thr;
								ssrmax_thr << std::setprecision(rmax_thr > l1 ? P : 2) << (rmax_thr > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_thr;
								ssravg_lat << std::setprecision(ravg_lat > l1 ? P : 2) << (ravg_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << ravg_lat;
								ssrmin_lat << std::setprecision(rmin_lat > l1 ? P : 2) << (rmin_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmin_lat;
								ssrmax_lat << std::setprecision(rmax_lat > l1 ? P : 2) << (rmax_lat > l2 ? std::scientific : std::fixed) << std::setw( 8) << rmax_lat;

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
						}
					}
				}
			}
		}
		stream << "#" << std::endl;
	}
}

void Simulation
::start_terminal_temp_report(const std::chrono::milliseconds &freq)
{
	// launch a thread dedicated to the terminal display
	term_thread = std::thread(Simulation::start_thread_terminal, this, freq);
}

void Simulation
::stop_terminal_temp_report()
{
	stop_terminal = true;
	cond_terminal.notify_all();
	// wait the terminal thread to finish
	term_thread.join();
	stop_terminal = false;
}

void Simulation
::start_thread_terminal(Simulation *simu, const std::chrono::milliseconds &freq)
{
	if (simu->terminal != nullptr)
	{
		const auto sleep_time = freq - std::chrono::milliseconds(0);

		while (!simu->stop_terminal)
		{
			std::unique_lock<std::mutex> lock(simu->mutex_terminal);
			if (simu->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
				simu->terminal->temp_report(std::clog); // display statistics in the terminal
		}
	}
	else
		std::clog << tools::format_warning("Terminal is not allocated: the temporal report can't be called.")
		          << std::endl;
}
