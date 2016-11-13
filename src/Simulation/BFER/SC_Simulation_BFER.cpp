#ifdef SYSTEMC

#include <thread>
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "Tools/Display/bash_tools.h"
#include "Tools/Factory/Factory_terminal.hpp"

#include "SC_Simulation_BFER.hpp"

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::Simulation_BFER(const parameters& params)
: Simulation_BFER_i<B,R,Q>(params),

  duplicator{nullptr, nullptr, nullptr},
  dbg_B     {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
  dbg_R     {nullptr, nullptr, nullptr},
  dbg_Q     {nullptr, nullptr, nullptr},

  d_decod_total_fake(std::chrono::nanoseconds(0))
{
	if (this->n_obj > 1)
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
		std::cerr << bold_yellow("(WW) The time report is not available in the SystemC simulation.") << std::endl;
}

template <typename B, typename R, typename Q>
Simulation_BFER<B,R,Q>
::~Simulation_BFER()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::_launch()
{
	// launch the simulation
	this->launch_simulation();

	if (!this->params.terminal.disabled && !this->params.simulation.benchs)
		terminal->final_report(std::cout);
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::release_objects()
{
	Simulation_BFER_i<B,R,Q>::release_objects();
	if (terminal != nullptr) { delete terminal; terminal = nullptr; }
}


template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::build_communication_chain()
{
	Simulation_BFER_i<B,R,Q>::build_communication_chain(this);

	// create the sc_module inside the objects of the communication chain
	this->source   [0]->create_sc_module            ();
	this->crc      [0]->create_sc_module            ();
	this->encoder  [0]->create_sc_module            ();
	this->puncturer[0]->create_sc_module_puncturer  ();
	this->puncturer[0]->create_sc_module_depuncturer();
	this->modulator[0]->create_sc_module_modulator  ();
	this->modulator[0]->create_sc_module_filterer   ();
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel  [0]->create_sc_module_wg            ();
		this->modulator[0]->create_sc_module_demodulator_wg();
	}
	else
	{
		this->channel  [0]->create_sc_module            ();
		this->modulator[0]->create_sc_module_demodulator();
	}
	this->quantizer[0]->create_sc_module();
	this->decoder  [0]->create_sc_module();
	this->monitor  [0]->create_sc_module();
	if (this->params.code.coset)
	{
		this->coset_real[0]->create_sc_module();
		this->coset_bit [0]->create_sc_module();
	}

	// build the terminal to display the BER/FER
	this->terminal = this->build_terminal();
	Simulation::check_errors(this->terminal, "Terminal");
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::launch_simulation()
{
	this->build_communication_chain();

	if ((!this->params.terminal.disabled && this->snr == this->params.simulation.snr_min &&
	    !(this->params.simulation.debug && this->n_obj == 1) && !this->params.simulation.benchs))
		this->terminal->legend(std::cout);

	this->duplicator[0] = new SC_Duplicator("Duplicator0");
	if (this->params.code.coset)
	{
		this->duplicator[1] = new SC_Duplicator("Duplicator1");
		this->duplicator[2] = new SC_Duplicator("Duplicator2");
	}

	if (this->n_obj == 1 && this->params.simulation.debug)
	{
		const auto dl = this->params.simulation.debug_limit;

		this->dbg_B[0] = new SC_Debug<B>("Generate random bits U_K...               \nU_K: \n", dl, "Debug_B0");
		this->dbg_B[1] = new SC_Debug<B>("Add the CRC to U_K...                     \nU_K: \n", dl, "Debug_B1");
		this->dbg_B[2] = new SC_Debug<B>("Encode U_K in X_N1...                     \nX_N1:\n", dl, "Debug_B2");
		this->dbg_B[3] = new SC_Debug<B>("Puncture X_N1 in X_N2...                  \nX_N2:\n", dl, "Debug_B3");
		this->dbg_R[0] = new SC_Debug<R>("Modulate X_N2 in X_N3...                  \nX_N3:\n", dl, "Debug_R0");
		this->dbg_R[1] = new SC_Debug<R>("Add noise from X_N3 to Y_N1...            \nY_N1:\n", dl, "Debug_R1");
		this->dbg_R[2] = new SC_Debug<R>("Filter from Y_N1 to Y_N2...               \nY_N2:\n", dl, "Debug_R2");
		this->dbg_R[3] = new SC_Debug<R>("Demodulate from Y_N3 to Y_N3...           \nY_N3:\n", dl, "Debug_R3");
		this->dbg_Q[0] = new SC_Debug<Q>("Make the quantization from Y_N3 to Y_N4...\nY_N4:\n", dl, "Debug_Q0");
		this->dbg_Q[1] = new SC_Debug<Q>("Depuncture Y_N4 and generate Y_N5...      \nY_N5:\n", dl, "Debug_Q1");
		this->dbg_B[4] = new SC_Debug<B>("Decode Y_N5 and generate V_K...           \nV_K: \n", dl, "Debug_B4");

		if (this->params.code.coset)
		{
			this->dbg_Q[2] = new SC_Debug<Q>("Apply the coset approach on Y_N5...       \nY_N5:\n", dl, "Debug_Q2");
			this->dbg_B[5] = new SC_Debug<B>("Apply the coset approach on V_K...        \nV_K: \n", dl, "Debug_B5");
		}
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
			this->dbg_R[4] = new SC_Debug<R>("Channel gains...                          \nH_N: \n", dl, "Debug_R4");

		this->bind_sockets_debug();
		sc_core::sc_start(); // start simulation
		this->terminal->legend(std::cout);

		for (auto i = 0; i < 6; i++)
			if (this->dbg_B[i] != nullptr)
			{
				delete this->dbg_B[i];
				this->dbg_B[i] = nullptr;
			}

		for (auto i = 0; i < 5; i++)
			if (this->dbg_R[i] != nullptr)
			{
				delete this->dbg_R[i];
				this->dbg_R[i] = nullptr;
			}

		for (auto i = 0; i < 3; i++)
			if (this->dbg_Q[i] != nullptr)
			{
				delete this->dbg_Q[i];
				this->dbg_Q[i] = nullptr;
			}
	}
	else
	{
		// launch a thread dedicated to the terminal display
		std::thread thread(Simulation_BFER<B,R,Q>::terminal_temp_report, this);

		this->bind_sockets();
		sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation

		// wait the terminal thread to finish
		thread.join();
	}

	for (auto i = 0; i < 3; i++)
		if (this->duplicator[i] != nullptr)
		{
			delete this->duplicator[i];
			this->duplicator[i] = nullptr;
		}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STANDARD! /!\ //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
	sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::bind_sockets()
{
	if (this->params.code.coset)
	{
		this->source       [0]->module         ->s_out (this->crc       [0]->module         ->s_in );
		this->crc          [0]->module         ->s_out (this->duplicator[0]                 ->s_in );
		this->duplicator   [0]                 ->s_out1(this->duplicator[1]                 ->s_in );
		this->duplicator   [1]                 ->s_out1(this->monitor   [0]->module         ->s_in1);
		this->duplicator   [1]                 ->s_out2(this->coset_bit [0]->module         ->s_in1);
		this->duplicator   [0]                 ->s_out2(this->encoder   [0]->module         ->s_in );
		this->encoder      [0]->module         ->s_out (this->duplicator[2]                 ->s_in );
		this->duplicator   [2]                 ->s_out1(this->coset_real[0]->module         ->s_in1);
		this->duplicator   [2]                 ->s_out2(this->puncturer [0]->module_punct   ->s_in );
		this->puncturer    [0]->module_punct   ->s_out (this->modulator [0]->module_mod     ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
			this->modulator[0]->module_mod     ->s_out (this->channel   [0]->module_wg      ->s_in );
			this->channel  [0]->module_wg      ->s_out1(this->modulator [0]->module_demod_wg->s_in1);
			this->channel  [0]->module_wg      ->s_out2(this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->modulator [0]->module_demod_wg->s_in2);
			this->modulator[0]->module_demod_wg->s_out (this->quantizer [0]->module         ->s_in );
		} else { // additive channel (AWGN, USER, NO)
			this->modulator[0]->module_mod     ->s_out (this->channel   [0]->module         ->s_in );
			this->channel  [0]->module         ->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->modulator [0]->module_demod   ->s_in );
			this->modulator[0]->module_demod   ->s_out (this->quantizer [0]->module         ->s_in );
		}
		this->quantizer    [0]->module         ->s_out (this->puncturer [0]->module_depunct ->s_in );
		this->puncturer    [0]->module_depunct ->s_out (this->coset_real[0]->module         ->s_in2);
		this->coset_real   [0]->module         ->s_out (this->decoder   [0]->module         ->s_in );
		this->decoder      [0]->module         ->s_out (this->coset_bit [0]->module         ->s_in2);
		this->coset_bit    [0]->module         ->s_out (this->monitor   [0]->module         ->s_in2);
	}
	else // standard simulation
	{
		this->source       [0]->module         ->s_out (this->crc       [0]->module         ->s_in );
		this->crc          [0]->module         ->s_out (this->duplicator[0]                 ->s_in );
		this->duplicator   [0]                 ->s_out1(this->monitor   [0]->module         ->s_in1);
		this->duplicator   [0]                 ->s_out2(this->encoder   [0]->module         ->s_in );
		this->encoder      [0]->module         ->s_out (this->puncturer [0]->module_punct   ->s_in );
		this->puncturer    [0]->module_punct   ->s_out (this->modulator [0]->module_mod     ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
			this->modulator[0]->module_mod     ->s_out (this->channel   [0]->module_wg      ->s_in );
			this->channel  [0]->module_wg      ->s_out1(this->modulator [0]->module_demod_wg->s_in1);
			this->channel  [0]->module_wg      ->s_out2(this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->modulator [0]->module_demod_wg->s_in2);
			this->modulator[0]->module_demod_wg->s_out (this->quantizer [0]->module         ->s_in );
		} else { // additive channel (AWGN, USER, NO)
			this->modulator[0]->module_mod     ->s_out (this->channel   [0]->module         ->s_in );
			this->channel  [0]->module         ->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->modulator [0]->module_demod   ->s_in );
			this->modulator[0]->module_demod   ->s_out (this->quantizer [0]->module         ->s_in );
		}
		this->quantizer    [0]->module         ->s_out (this->puncturer [0]->module_depunct ->s_in );
		this->puncturer    [0]->module_depunct ->s_out (this->decoder   [0]->module         ->s_in );
		this->decoder      [0]->module         ->s_out (this->monitor   [0]->module         ->s_in2);
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::bind_sockets_debug()
{
	if (this->params.code.coset)
	{
		this->source       [0]->module         ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->crc       [0]->module         ->s_in );
		this->crc          [0]->module         ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->duplicator[0]                 ->s_in );
		this->duplicator   [0]                                                               ->s_out1(this->duplicator[1]                 ->s_in );
		this->duplicator   [1]                                                               ->s_out1(this->monitor   [0]->module         ->s_in1);
		this->duplicator   [1]                                                               ->s_out2(this->coset_bit [0]->module         ->s_in1);
		this->duplicator   [0]                                                               ->s_out2(this->encoder   [0]->module         ->s_in );
		this->encoder      [0]->module         ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->duplicator[2]                 ->s_in );
		this->duplicator   [2]                                                               ->s_out1(this->coset_real[0]->module         ->s_in1);
		this->duplicator   [2]                                                               ->s_out2(this->puncturer [0]->module_punct   ->s_in );
		this->puncturer    [0]->module_punct   ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator [0]->module_mod     ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
 			this->modulator[0]->module_mod     ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel   [0]->module_wg      ->s_in );
			this->channel  [0]->module_wg      ->s_out1(this->dbg_R[4]->s_in); this->dbg_R[4]->s_out (this->modulator [0]->module_demod_wg->s_in1);
			this->channel  [0]->module_wg      ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->modulator [0]->module_demod_wg->s_in2);
			this->modulator[0]->module_demod_wg->s_out (this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->quantizer [0]->module         ->s_in );
		} else { // additive channel (AWGN, USER, NO)
			this->modulator[0]->module_mod     ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel   [0]->module         ->s_in );
			this->channel  [0]->module         ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->modulator [0]->module_demod   ->s_in );
			this->modulator[0]->module_demod   ->s_out (this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->quantizer [0]->module         ->s_in );
		}
		this->quantizer    [0]->module         ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->puncturer [0]->module_depunct ->s_in );
		this->puncturer    [0]->module_depunct ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->coset_real[0]->module         ->s_in2);
		this->coset_real   [0]->module         ->s_out (this->dbg_Q[2]->s_in); this->dbg_Q[2]->s_out (this->decoder   [0]->module         ->s_in );
		this->decoder      [0]->module         ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->coset_bit [0]->module         ->s_in2);
		this->coset_bit    [0]->module         ->s_out (this->dbg_B[5]->s_in); this->dbg_B[5]->s_out (this->monitor   [0]->module         ->s_in2);
	}
	else // standard simulation
	{
		this->source       [0]->module         ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->crc       [0]->module         ->s_in );
		this->crc          [0]->module         ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->duplicator[0]                 ->s_in );
		this->duplicator   [0]                                                               ->s_out1(this->monitor   [0]->module         ->s_in1);
		this->duplicator   [0]                                                               ->s_out2(this->encoder   [0]->module         ->s_in );
		this->encoder      [0]->module         ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->puncturer [0]->module_punct   ->s_in );
		this->puncturer    [0]->module_punct   ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator [0]->module_mod     ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
			this->modulator[0]->module_mod     ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel   [0]->module_wg      ->s_in );
			this->channel  [0]->module_wg      ->s_out1(this->dbg_R[4]->s_in); this->dbg_R[4]->s_out (this->modulator [0]->module_demod_wg->s_in1);
			this->channel  [0]->module_wg      ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->modulator [0]->module_demod_wg->s_in2);
			this->modulator[0]->module_demod_wg->s_out (this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->quantizer [0]->module         ->s_in );
		} else { // additive channel (AWGN, USER, NO)
			this->modulator[0]->module_mod     ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel   [0]->module         ->s_in );
			this->channel  [0]->module         ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator [0]->module_filt    ->s_in );
			this->modulator[0]->module_filt    ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->modulator [0]->module_demod   ->s_in );
			this->modulator[0]->module_demod   ->s_out (this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->quantizer [0]->module         ->s_in );
		}
		this->quantizer    [0]->module         ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->puncturer [0]->module_depunct ->s_in );
		this->puncturer    [0]->module_depunct ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->decoder   [0]->module         ->s_in );
		this->decoder      [0]->module         ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->monitor   [0]->module         ->s_in2);
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER<B,R,Q>
::terminal_temp_report(Simulation_BFER<B,R,Q> *simu)
{
	if (!simu->params.terminal.disabled && simu->params.terminal.frequency != std::chrono::nanoseconds(0))
	{
		while (!simu->monitor[0]->fe_limit_achieved() && !simu->monitor[0]->is_interrupt())
		{
			const auto sleep_time = simu->params.terminal.frequency - std::chrono::milliseconds(0);
			std::this_thread::sleep_for(sleep_time);

			// display statistics in terminal
			simu->terminal->temp_report(std::clog);
		}
	}
}

template <typename B, typename R, typename Q>
Terminal* Simulation_BFER<B,R,Q>
::build_terminal()
{
	return Factory_terminal<B,R>::build(this->params, this->snr, this->monitor[0], this->t_snr, d_decod_total_fake);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class Simulation_BFER<B_8,R_8,Q_8>;
template class Simulation_BFER<B_16,R_16,Q_16>;
template class Simulation_BFER<B_32,R_32,Q_32>;
template class Simulation_BFER<B_64,R_64,Q_64>;
#else
template class Simulation_BFER<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* SYSTEMC*/
