#ifdef SYSTEMC

#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#include "Tools/Display/bash_tools.h"
#include "Tools/Factory/Factory_terminal.hpp"
#include "Tools/Algo/Predicate_ite.hpp"

#include "SC_Simulation_BFERI.hpp"

template <typename B, typename R, typename Q>
Simulation_BFERI<B,R,Q>
::Simulation_BFERI(const parameters& params)
: Simulation_BFERI_i<B,R,Q>(params),

  terminal(nullptr),

  duplicator {nullptr, nullptr, nullptr, nullptr, nullptr},
  router     (nullptr),
  predicate  (nullptr),
  dbg_B      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
  dbg_R      {nullptr, nullptr, nullptr},
  dbg_Q      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
	if (params.simulation.n_threads > 1)
	{
		std::cerr << bold_red("(EE) SystemC simulation does not support multi-threading... Exiting.") << std::endl;
		std::exit(-1);
	}

	if (params.simulation.benchs)
	{
		std::cerr << bold_red("(EE) SystemC simulation does not support the bench mode... Exiting") << std::endl;
		std::exit(-1);
	}

	if (params.simulation.time_report)
		std::clog << bold_yellow("(WW) The time report is not available in the SystemC simulation.") << std::endl;

#ifdef ENABLE_MPI
	std::clog << bold_yellow("(WW) This simulation is not MPI ready, the same computations will be launched ")
	          << bold_yellow("on each MPI processes.") << std::endl;
#endif
}

template <typename B, typename R, typename Q>
Simulation_BFERI<B,R,Q>
::~Simulation_BFERI()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::_launch()
{
	// launch the simulation
	this->launch_simulation();

	if (!this->params.terminal.disabled && !this->params.simulation.benchs)
		terminal->final_report(std::cout);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::release_objects()
{
	Simulation_BFERI_i<B,R,Q>::release_objects();
	if (terminal != nullptr) { delete terminal; terminal = nullptr; }
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::build_communication_chain()
{
	Simulation_BFERI_i<B,R,Q>::build_communication_chain(this);

	// build the objects
	this->interleaver_e  = this->build_interleaver(); Simulation::check_errors(this->interleaver_e , "Interleaver<int>");
	this->coset_real_i   = this->build_coset_real (); Simulation::check_errors(this->coset_real_i  , "Coset<B,Q>"      );

	this->interleaver_e->rename("Interleaver_e");
	this->coset_real_i ->rename("Coset_real_i" );

	assert(*this->interleaver[0] == *this->interleaver_e);

	// create the sc_module inside the objects of the communication chain
	this->source     [0]->create_sc_module              ();
	this->crc        [0]->create_sc_module              ();
	this->encoder    [0]->create_sc_module              ();
	this->interleaver_e ->create_sc_module_interleaver  ();
	this->modulator  [0]->create_sc_module_modulator    ();
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		this->channel[0]->create_sc_module_wg           ();
	else
		this->channel[0]->create_sc_module              ();
	this->modulator  [0]->create_sc_module_filterer     ();
	this->quantizer  [0]->create_sc_module              ();
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		this->modulator[0]->create_sc_module_tdemodulator_wg();
	else
		this->modulator[0]->create_sc_module_tdemodulator();
	this->interleaver[0]->create_sc_module_deinterleaver();
	this->siso       [0]->create_sc_module_siso         ();
	this->interleaver[0]->create_sc_module_interleaver  ();
	this->decoder    [0]->create_sc_module              ();
	this->monitor    [0]->create_sc_module              ();
	if (this->params.code.coset)
	{
		this->coset_real[0]->create_sc_module();
		this->coset_real_i ->create_sc_module();
		this->coset_bit [0]->create_sc_module();
	}

	// build the terminal to display the BER/FER
	this->terminal = this->build_terminal();
	Simulation::check_errors(this->terminal, "Terminal");
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::launch_simulation()
{
	this->build_communication_chain();

	if ((!this->params.terminal.disabled && this->snr == this->params.simulation.snr_min &&
	    !(this->params.simulation.debug && this->params.simulation.n_threads == 1) && !this->params.simulation.benchs))
		this->terminal->legend(std::cout);

	Predicate_ite p(this->params.demodulator.n_ite);

	this->duplicator[0] = new SC_Duplicator(   "Duplicator0");
	this->duplicator[1] = new SC_Duplicator(   "Duplicator1");
	if (this->params.code.coset)
	{
		this->duplicator[2] = new SC_Duplicator("Duplicator2");
		this->duplicator[3] = new SC_Duplicator("Duplicator3");
		this->duplicator[4] = new SC_Duplicator("Duplicator4");
	}
	this->router        = new SC_Router    (p, "Router"     );
	this->predicate     = new SC_Predicate (p, "Predicate"  );

	if (this->params.simulation.n_threads == 1 && this->params.simulation.debug)
	{
		const auto dl = this->params.simulation.debug_limit;

		this->dbg_B[0] = new SC_Debug<B>("Generate random bits U_K...               \nU_K: \n", dl, "Debug_B0");
		this->dbg_B[1] = new SC_Debug<B>("Add the CRC to U_K...                     \nU_K: \n", dl, "Debug_B1");
		this->dbg_B[2] = new SC_Debug<B>("Encode U_K in X_N1...                     \nX_N1:\n", dl, "Debug_B2");
		this->dbg_B[3] = new SC_Debug<B>("Interleave X_N1 in X_N2...                \nX_N2:\n", dl, "Debug_B3");
		this->dbg_R[0] = new SC_Debug<R>("Modulate X_N2 in X_N3...                  \nX_N3:\n", dl, "Debug_R0");
		this->dbg_R[1] = new SC_Debug<R>("Add noise from X_N3 to Y_N1...            \nY_N1:\n", dl, "Debug_R1");
		this->dbg_R[2] = new SC_Debug<R>("Filter from Y_N1 to Y_N2...               \nY_N2:\n", dl, "Debug_R2");
		this->dbg_Q[0] = new SC_Debug<Q>("Make the quantization from Y_N2 to Y_N3...\nY_N3:\n", dl, "Debug_Q0");
		this->dbg_Q[1] = new SC_Debug<Q>("Demodulate from Y_N3 and Y_N7 to Y_N4...  \nY_N4:\n", dl, "Debug_Q1");
		this->dbg_Q[2] = new SC_Debug<Q>("Deinterleave from Y_N4 to Y_N5...         \nY_N5:\n", dl, "Debug_Q2");
		this->dbg_Q[3] = new SC_Debug<Q>("Soft decode from Y_N5 to Y_N6...          \nY_N6:\n", dl, "Debug_Q3");
		this->dbg_Q[4] = new SC_Debug<Q>("Interleave from Y_N6 to Y_N7...           \nY_N7:\n", dl, "Debug_Q4");
		this->dbg_B[4] = new SC_Debug<B>("Hard decode Y_N5 and generate V_K...      \nV_K: \n", dl, "Debug_B4");

		if (this->params.code.coset)
		{
			this->dbg_Q[5] = new SC_Debug<Q>("Apply the coset approach on Y_N5...       \nY_N5:\n", dl, "Debug_Q5");
			this->dbg_Q[6] = new SC_Debug<Q>("Reverse the coset on Y_N6...              \nY_N6:\n", dl, "Debug_Q6");
			this->dbg_B[5] = new SC_Debug<B>("Apply the coset approach on V_K...        \nV_K: \n", dl, "Debug_B5");
		}
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
			this->dbg_R[3] = new SC_Debug<R>("Channel gains...                          \nH_N: \n", dl, "Debug_R3");

		this->bind_sockets_debug();
		sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation
		this->terminal->legend(std::cout);

		for (auto i = 0; i < 6; i++)
			if (this->dbg_B[i] != nullptr)
			{
				delete this->dbg_B[i];
				this->dbg_B[i] = nullptr;
			}

		for (auto i = 0; i < 4; i++)
			if (this->dbg_R[i] != nullptr)
			{
				delete this->dbg_R[i];
				this->dbg_R[i] = nullptr;
			}

		for (auto i = 0; i < 7; i++)
			if (this->dbg_Q[i] != nullptr)
			{
				delete this->dbg_Q[i];
				this->dbg_Q[i] = nullptr;
			}
	}
	else
	{
		std::thread term_thread;
		if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0))
			// launch a thread dedicated to the terminal display
			term_thread = std::thread(Simulation_BFERI<B,R,Q>::terminal_temp_report, this);

		this->bind_sockets();
		sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation

		if (!this->params.terminal.disabled && this->params.terminal.frequency != std::chrono::nanoseconds(0))
		{
			cond_terminal.notify_all();
			// wait the terminal thread to finish
			term_thread.join();
		}
	}

	for (auto i = 0; i < 5; i++)
		if (this->duplicator[i] != nullptr)
		{
			delete this->duplicator[i];
			this->duplicator[i] = nullptr;
		}

	delete this->router;      this->router      = nullptr;
	delete this->predicate;   this->predicate   = nullptr;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STANDARD! /!\ //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
	sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::bind_sockets()
{
	if (this->params.code.coset)
	{
		this->source         [0]->module          ->s_out (this->crc        [0]->module          ->s_in );
		this->crc            [0]->module          ->s_out (this->duplicator [0]                  ->s_in );
		this->duplicator     [0]                  ->s_out1(this->duplicator [2]                  ->s_in );
		this->duplicator     [2]                  ->s_out1(this->monitor    [0]->module          ->s_in1);
		this->duplicator     [2]                  ->s_out2(this->coset_bit  [0]->module          ->s_in1);
		this->duplicator     [0]                  ->s_out2(this->encoder    [0]->module          ->s_in );
		this->encoder        [0]->module          ->s_out (this->duplicator [3]                  ->s_in );
		this->duplicator     [3]                  ->s_out1(this->duplicator [4]                  ->s_in );
		this->duplicator     [4]                  ->s_out1(this->coset_real [0]->module          ->s_in1);
		this->duplicator     [4]                  ->s_out2(this->coset_real_i  ->module          ->s_in1);
		this->duplicator     [3]                  ->s_out2(this->interleaver_e ->module_inter    ->s_in );
		this->interleaver_e     ->module_inter    ->s_out (this->modulator  [0]->module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->module_mod      ->s_out (this->channel    [0]->module_wg       ->s_in );
			this->channel    [0]->module_wg       ->s_out1(this->modulator  [0]->module_tdemod_wg->s_in1);
			this->channel    [0]->module_wg       ->s_out2(this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->modulator  [0]->module_tdemod_wg->s_in2);
			this->modulator  [0]->module_tdemod_wg->s_out (this->interleaver[0]->module_deinter  ->s_in );
		} else {
			this->modulator  [0]->module_mod      ->s_out (this->channel    [0]->module          ->s_in );
			this->channel    [0]->module          ->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->modulator  [0]->module_tdemod   ->s_in1);
			this->modulator  [0]->module_tdemod   ->s_out (this->interleaver[0]->module_deinter  ->s_in );
		}
		this->interleaver    [0]->module_deinter  ->s_out (this->coset_real [0]->module          ->s_in2);
		this->coset_real     [0]->module          ->s_out (this->router                          ->s_in );
		this->router                              ->s_out1(this->siso       [0]->module_siso     ->s_in );
		this->router                              ->s_out2(this->decoder    [0]->module          ->s_in );
		this->siso           [0]->module_siso     ->s_out (this->coset_real_i  ->module          ->s_in2);
		this->coset_real_i      ->module          ->s_out (this->interleaver[0]->module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->module_inter    ->s_out (this->modulator  [0]->module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->module_inter    ->s_out (this->modulator  [0]->module_tdemod   ->s_in2);
		}
		this->decoder        [0]->module          ->s_out (this->coset_bit  [0]->module          ->s_in2);
		this->coset_bit      [0]->module          ->s_out (this->duplicator [1]                  ->s_in );
		this->duplicator     [1]                  ->s_out1(this->monitor    [0]->module          ->s_in2);
		this->duplicator     [1]                  ->s_out2(this->predicate                       ->s_in );
	}
	else // standard simulation
	{
		this->source         [0]->module          ->s_out (this->crc        [0]->module          ->s_in );
		this->crc            [0]->module          ->s_out (this->duplicator [0]                  ->s_in );
		this->duplicator     [0]                  ->s_out1(this->monitor    [0]->module          ->s_in1);
		this->duplicator     [0]                  ->s_out2(this->encoder    [0]->module          ->s_in );
		this->encoder        [0]->module          ->s_out (this->interleaver_e ->module_inter    ->s_in );
		this->interleaver_e     ->module_inter    ->s_out (this->modulator  [0]->module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->module_mod      ->s_out (this->channel    [0]->module_wg       ->s_in );
			this->channel    [0]->module_wg       ->s_out1(this->modulator  [0]->module_tdemod_wg->s_in1);
			this->channel    [0]->module_wg       ->s_out2(this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->modulator  [0]->module_tdemod_wg->s_in2);
			this->modulator  [0]->module_tdemod_wg->s_out (this->interleaver[0]->module_deinter  ->s_in );
		} else {
			this->modulator  [0]->module_mod      ->s_out (this->channel    [0]->module          ->s_in );
			this->channel    [0]->module          ->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->modulator  [0]->module_tdemod   ->s_in1);
			this->modulator  [0]->module_tdemod   ->s_out (this->interleaver[0]->module_deinter  ->s_in );
		}
		this->interleaver    [0]->module_deinter  ->s_out (this->router                          ->s_in );
		this->router                              ->s_out1(this->siso       [0]->module_siso     ->s_in );
		this->router                              ->s_out2(this->decoder    [0]->module          ->s_in );
		this->siso           [0]->module_siso     ->s_out (this->interleaver[0]->module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->module_inter    ->s_out (this->modulator  [0]->module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->module_inter    ->s_out (this->modulator  [0]->module_tdemod   ->s_in2);
		}
		this->decoder        [0]->module          ->s_out (this->duplicator [1]                  ->s_in );
		this->duplicator     [1]                  ->s_out1(this->monitor    [0]->module          ->s_in2);
		this->duplicator     [1]                  ->s_out2(this->predicate                       ->s_in );
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::bind_sockets_debug()
{
	if (this->params.code.coset)
	{
		this->source         [0]->module          ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->crc        [0]->module          ->s_in );
		this->crc            [0]->module          ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->duplicator [0]                  ->s_in );
		this->duplicator     [0]                                                                ->s_out1(this->duplicator [2]                  ->s_in );
		this->duplicator     [2]                                                                ->s_out1(this->monitor    [0]->module          ->s_in1);
		this->duplicator     [2]                                                                ->s_out2(this->coset_bit  [0]->module          ->s_in1);
		this->duplicator     [0]                                                                ->s_out2(this->encoder    [0]->module          ->s_in );
		this->encoder        [0]->module          ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->duplicator [3]                  ->s_in );
		this->duplicator     [3]                                                                ->s_out1(this->duplicator [4]                  ->s_in );
		this->duplicator     [4]                                                                ->s_out1(this->coset_real [0]->module          ->s_in1);
		this->duplicator     [4]                                                                ->s_out2(this->coset_real_i  ->module          ->s_in1);
		this->duplicator     [3]                                                                ->s_out2(this->interleaver_e ->module_inter    ->s_in );
		this->interleaver_e     ->module_inter    ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator  [0]->module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->module_wg       ->s_in );
			this->channel    [0]->module_wg       ->s_out1(this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->modulator  [0]->module_tdemod_wg->s_in1);
			this->channel    [0]->module_wg       ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->module_tdemod_wg->s_in2);
			this->modulator  [0]->module_tdemod_wg->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->module_deinter  ->s_in );
		} else {
			this->modulator  [0]->module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->module          ->s_in );
			this->channel    [0]->module          ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->module_tdemod   ->s_in1);
			this->modulator  [0]->module_tdemod   ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->module_deinter  ->s_in );
		}
		this->interleaver    [0]->module_deinter  ->s_out (this->dbg_Q[2]->s_in); this->dbg_Q[2]->s_out (this->coset_real [0]->module          ->s_in2);
		this->coset_real     [0]->module          ->s_out (this->dbg_Q[5]->s_in); this->dbg_Q[5]->s_out (this->router                          ->s_in );
		this->router                                                                            ->s_out1(this->siso       [0]->module_siso     ->s_in );
		this->router                                                                            ->s_out2(this->decoder    [0]->module          ->s_in );
		this->siso           [0]->module_siso     ->s_out (this->dbg_Q[3]->s_in); this->dbg_Q[3]->s_out (this->coset_real_i  ->module          ->s_in2);
		this->coset_real_i      ->module          ->s_out (this->dbg_Q[6]->s_in); this->dbg_Q[6]->s_out (this->interleaver[0]->module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->module_tdemod   ->s_in2);
		}
		this->decoder        [0]->module          ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->coset_bit  [0]->module          ->s_in2);
		this->coset_bit      [0]->module          ->s_out (this->dbg_B[5]->s_in); this->dbg_B[5]->s_out (this->duplicator [1]                  ->s_in );
		this->duplicator     [1]                                                                ->s_out1(this->monitor    [0]->module          ->s_in2);
		this->duplicator     [1]                                                                ->s_out2(this->predicate                       ->s_in );
	}
	else // standard simulation
	{
		this->source         [0]->module          ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->crc        [0]->module          ->s_in );
		this->crc            [0]->module          ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->duplicator [0]                  ->s_in );
		this->duplicator     [0]                                                                ->s_out1(this->monitor    [0]->module          ->s_in1);
		this->duplicator     [0]                                                                ->s_out2(this->encoder    [0]->module          ->s_in );
		this->encoder        [0]->module          ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->interleaver_e ->module_inter    ->s_in );
		this->interleaver_e     ->module_inter    ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator  [0]->module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->module_wg       ->s_in );
			this->channel    [0]->module_wg       ->s_out1(this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->modulator  [0]->module_tdemod_wg->s_in1);
			this->channel    [0]->module_wg       ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->module_tdemod_wg->s_in2);
			this->modulator  [0]->module_tdemod_wg->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->module_deinter  ->s_in );
		} else {
			this->modulator  [0]->module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->module          ->s_in );
			this->channel    [0]->module          ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->module_filt     ->s_in );
			this->modulator  [0]->module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->module          ->s_in );
			this->quantizer  [0]->module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->module_tdemod   ->s_in1);
			this->modulator  [0]->module_tdemod   ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->module_deinter  ->s_in );
		}
		this->interleaver    [0]->module_deinter  ->s_out (this->dbg_Q[2]->s_in); this->dbg_Q[2]->s_out (this->router                          ->s_in );
		this->router                                                                            ->s_out1(this->siso       [0]->module_siso     ->s_in );
		this->router                                                                            ->s_out2(this->decoder    [0]->module          ->s_in );
		this->siso           [0]->module_siso     ->s_out (this->dbg_Q[3]->s_in); this->dbg_Q[3]->s_out (this->interleaver[0]->module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->module_tdemod   ->s_in2);
		}
		this->decoder        [0]->module          ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->duplicator [1]                  ->s_in );
		this->duplicator     [1]                                                                ->s_out1(this->monitor    [0]->module          ->s_in2);
		this->duplicator     [1]                                                                ->s_out2(this->predicate                       ->s_in );
	}
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFERI<B,R,Q>
::build_terminal(const int tid)
{
	return Factory_terminal<B,R>::build(this->params, this->snr, this->monitor[0], this->t_snr);
}

template <typename B, typename R, typename Q>
void Simulation_BFERI<B,R,Q>
::terminal_temp_report(Simulation_BFERI<B,R,Q> *simu)
{
	if (simu->terminal != nullptr && simu->monitor[0] != nullptr)
	{
		const auto sleep_time = simu->params.terminal.frequency - std::chrono::milliseconds(0);

		while (!simu->monitor[0]->fe_limit_achieved() && !simu->monitor[0]->is_interrupt())
		{
			std::unique_lock<std::mutex> lock(simu->mutex_terminal);
			if (simu->cond_terminal.wait_for(lock, sleep_time) == std::cv_status::timeout)
				simu->terminal->temp_report(std::clog); // display statistics in the terminal
		}
	}
	else
		std::clog << bold_yellow("(WW) Terminal is not allocated: you can't call the temporal report.") << std::endl;
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFERI<B_8,R_8,Q_8>;
template class Simulation_BFERI<B_16,R_16,Q_16>;
template class Simulation_BFERI<B_32,R_32,Q_32>;
template class Simulation_BFERI<B_64,R_64,Q_64>;
#else
template class Simulation_BFERI<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif
