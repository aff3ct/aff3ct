#ifdef SYSTEMC

#include <stdexcept>

#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Predicate_ite.hpp"

#include "Tools/Factory/Coset/Factory_coset_real.hpp"
#include "Tools/Factory/Factory_interleaver.hpp"

#include "SC_Simulation_BFER_ite.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SC_Simulation_BFER_ite<B,R,Q>
::SC_Simulation_BFER_ite(const parameters& params, Codec_SISO<B,Q> &codec)
: Simulation_BFER_ite<B,R,Q>(params,codec),

  interleaver_e(nullptr),
  coset_real_i(nullptr),

  duplicator {nullptr, nullptr, nullptr, nullptr, nullptr},
  router     (nullptr),
  predicate  (nullptr),
  dbg_B      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
  dbg_R      {nullptr, nullptr, nullptr},
  dbg_Q      {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{
	if (this->params.simulation.n_threads > 1)
		throw std::invalid_argument("aff3ct::simulation::SC_Simulation_BFERI_ite: SystemC simulation does not support "
		                            "multi-threading.");
	if (params.simulation.benchs)
		throw std::invalid_argument("aff3ct::simulation::SC_Simulation_BFERI_ite: SystemC simulation does not support "
		                            "the bench mode.");

	if (params.simulation.time_report)
		std::clog << bold_yellow("(WW) The time report is not available in the SystemC simulation.") << std::endl;

#ifdef ENABLE_MPI
	std::clog << bold_yellow("(WW) This simulation is not MPI ready, the same computations will be launched ")
	          << bold_yellow("on each MPI processes.") << std::endl;
#endif
}

template <typename B, typename R, typename Q>
SC_Simulation_BFER_ite<B,R,Q>
::~SC_Simulation_BFER_ite()
{
}

template <typename B, typename R, typename Q>
void SC_Simulation_BFER_ite<B,R,Q>
::build_communication_chain(const int tid)
{
	Simulation_BFER_ite<B,R,Q>::build_communication_chain(tid);

	if (*this->interleaver[tid] != *this->interleaver_e)
		throw std::runtime_error("aff3ct::simulation::SC_Simulation_BFERI_ite: \"interleaver[tid]\" and "
		                         "\"interleaver_e\" have to be equal.");

	// create the sc_module inside the objects of the communication chain
	this->source     [tid]->create_sc_module              ();
	this->crc        [tid]->create_sc_module_build        ();
	this->encoder    [tid]->create_sc_module              ();
	this->interleaver_e   ->create_sc_module_interleaver  ();
	this->modulator  [tid]->create_sc_module_modulator    ();
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		this->channel[tid]->create_sc_module_wg           ();
	else
		this->channel[tid]->create_sc_module              ();
	this->modulator  [tid]->create_sc_module_filterer     ();
	this->quantizer  [tid]->create_sc_module              ();
	if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		this->modulator[tid]->create_sc_module_tdemodulator_wg();
	else
		this->modulator[tid]->create_sc_module_tdemodulator();
	this->interleaver[tid]->create_sc_module_deinterleaver();
	this->siso       [tid]->create_sc_module_siso         ();
	this->interleaver[tid]->create_sc_module_interleaver  ();
	this->decoder    [tid]->create_sc_module              ();
	this->monitor    [tid]->create_sc_module              ();
	if (this->params.code.coset)
	{
		this->coset_real[tid]->create_sc_module();
		this->coset_real_i   ->create_sc_module();
		this->coset_bit [tid]->create_sc_module();
	}
	this->crc[tid]->create_sc_module_extract();

	if (this->params.monitor.err_track_enable)
	{
		const auto &U_K = this->source [tid]->sc_module->get_U_K();
		const auto &X_N = this->encoder[tid]->sc_module->get_X_N();

		this->dumper[tid]->register_data(U_K, "src", false, {});
		this->dumper[tid]->register_data(X_N, "enc", false, {(unsigned)this->params.code.K});
		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});
		if (this->interleaver[tid]->is_uniform())
			this->dumper[tid]->register_data(this->interleaver[tid]->get_lut(), "itl", false, {});
	}
}

template <typename B, typename R, typename Q>
void SC_Simulation_BFER_ite<B,R,Q>
::release_objects()
{
	if (interleaver_e != nullptr) { delete interleaver_e; interleaver_e = nullptr; }
	if (coset_real_i  != nullptr) { delete coset_real_i;  coset_real_i  = nullptr; }

	Simulation_BFER_ite<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
void SC_Simulation_BFER_ite<B,R,Q>
::_launch()
{
	Predicate_ite p(this->params.simulation.n_ite);

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
		this->dbg_B[4] = new SC_Debug<B>("Hard decode Y_N5 and generate V_K1...     \nV_K1:\n", dl, "Debug_B4");
		this->dbg_B[6] = new SC_Debug<B>("Extract CRC bits from V_K1 into V_K2...   \nV_K2:\n", dl, "Debug_B6");

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

		for (auto i = 0; i < 7; i++)
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
		this->bind_sockets();
		sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
		sc_core::sc_start(); // start simulation
	}

	for (auto i = 0; i < 5; i++)
		if (this->duplicator[i] != nullptr)
		{
			delete this->duplicator[i];
			this->duplicator[i] = nullptr;
		}

	delete this->router;      this->router      = nullptr;
	delete this->predicate;   this->predicate   = nullptr;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STD! /!\ //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
	sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void SC_Simulation_BFER_ite<B,R,Q>
::bind_sockets()
{
	if (this->params.code.coset)
	{
		this->source         [0]->sc_module          ->s_out (this->duplicator [0]                     ->s_in );
		this->duplicator     [0]                     ->s_out1(this->monitor    [0]->sc_module          ->s_in1);
		this->duplicator     [0]                     ->s_out2(this->crc        [0]->sc_module_build    ->s_in );
		this->crc            [0]->sc_module_build    ->s_out (this->duplicator [2]                     ->s_in );
		this->duplicator     [2]                     ->s_out1(this->coset_bit  [0]->sc_module          ->s_in1);
		this->duplicator     [2]                     ->s_out2(this->encoder    [0]->sc_module          ->s_in );
		this->encoder        [0]->sc_module          ->s_out (this->duplicator [3]                     ->s_in );
		this->duplicator     [3]                     ->s_out1(this->duplicator [4]                     ->s_in );
		this->duplicator     [4]                     ->s_out1(this->coset_real [0]->sc_module          ->s_in1);
		this->duplicator     [4]                     ->s_out2(this->coset_real_i  ->sc_module          ->s_in1);
		this->duplicator     [3]                     ->s_out2(this->interleaver_e ->sc_module_inter    ->s_in );
		this->interleaver_e     ->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->sc_module_mod      ->s_out (this->channel    [0]->sc_module_wg       ->s_in );
			this->channel    [0]->sc_module_wg       ->s_out1(this->modulator  [0]->sc_module_tdemod_wg->s_in1);
			this->channel    [0]->sc_module_wg       ->s_out2(this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in2);
			this->modulator  [0]->sc_module_tdemod_wg->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		} else {
			this->modulator  [0]->sc_module_mod      ->s_out (this->channel    [0]->sc_module          ->s_in );
			this->channel    [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in1);
			this->modulator  [0]->sc_module_tdemod   ->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		}
		this->interleaver    [0]->sc_module_deinter  ->s_out (this->coset_real [0]->sc_module          ->s_in2);
		this->coset_real     [0]->sc_module          ->s_out (this->router                             ->s_in );
		this->router                                 ->s_out1(this->siso       [0]->sc_module_siso     ->s_in );
		this->router                                 ->s_out2(this->decoder    [0]->sc_module          ->s_in );
		this->siso           [0]->sc_module_siso     ->s_out (this->coset_real_i  ->sc_module          ->s_in2);
		this->coset_real_i      ->sc_module          ->s_out (this->interleaver[0]->sc_module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in2);
		}
		this->decoder        [0]->sc_module          ->s_out (this->coset_bit  [0]->sc_module          ->s_in2);
		this->coset_bit      [0]->sc_module          ->s_out (this->crc        [0]->sc_module_extract  ->s_in );
		this->crc            [0]->sc_module_extract  ->s_out (this->duplicator [1]                     ->s_in );
		this->duplicator     [1]                     ->s_out1(this->monitor    [0]->sc_module          ->s_in2);
		this->duplicator     [1]                     ->s_out2(this->predicate                          ->s_in );
	}
	else // standard simulation
	{
		this->source         [0]->sc_module          ->s_out (this->duplicator [0]                     ->s_in );
		this->duplicator     [0]                     ->s_out1(this->monitor    [0]->sc_module          ->s_in1);
		this->duplicator     [0]                     ->s_out2(this->crc        [0]->sc_module_build    ->s_in );
		this->crc            [0]->sc_module_build    ->s_out (this->encoder    [0]->sc_module          ->s_in );
		this->encoder        [0]->sc_module          ->s_out (this->interleaver_e ->sc_module_inter    ->s_in );
		this->interleaver_e     ->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->sc_module_mod      ->s_out (this->channel    [0]->sc_module_wg       ->s_in );
			this->channel    [0]->sc_module_wg       ->s_out1(this->modulator  [0]->sc_module_tdemod_wg->s_in1);
			this->channel    [0]->sc_module_wg       ->s_out2(this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in2);
			this->modulator  [0]->sc_module_tdemod_wg->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		} else {
			this->modulator  [0]->sc_module_mod      ->s_out (this->channel    [0]->sc_module          ->s_in );
			this->channel    [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in1);
			this->modulator  [0]->sc_module_tdemod   ->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		}
		this->interleaver    [0]->sc_module_deinter  ->s_out (this->router                             ->s_in );
		this->router                                 ->s_out1(this->siso       [0]->sc_module_siso     ->s_in );
		this->router                                 ->s_out2(this->decoder    [0]->sc_module          ->s_in );
		this->siso           [0]->sc_module_siso     ->s_out (this->interleaver[0]->sc_module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->sc_module_inter    ->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in2);
		}
		this->decoder        [0]->sc_module          ->s_out (this->crc        [0]->sc_module_extract  ->s_in );
		this->crc            [0]->sc_module_extract  ->s_out (this->duplicator [1]                     ->s_in );
		this->duplicator     [1]                     ->s_out1(this->monitor    [0]->sc_module          ->s_in2);
		this->duplicator     [1]                     ->s_out2(this->predicate                          ->s_in );
	}
}

template <typename B, typename R, typename Q>
void SC_Simulation_BFER_ite<B,R,Q>
::bind_sockets_debug()
{
	if (this->params.code.coset)
	{
		this->source         [0]->sc_module          ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->duplicator [0]                     ->s_in );
		this->duplicator     [0]                                                                   ->s_out1(this->monitor    [0]->sc_module          ->s_in1);
		this->duplicator     [0]                                                                   ->s_out2(this->crc        [0]->sc_module_build    ->s_in );
		this->crc            [0]->sc_module_build    ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->duplicator [2]                     ->s_in );
		this->duplicator     [2]                                                                   ->s_out1(this->coset_bit  [0]->sc_module          ->s_in1);
		this->duplicator     [2]                                                                   ->s_out2(this->encoder    [0]->sc_module          ->s_in );
		this->encoder        [0]->sc_module          ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->duplicator [3]                     ->s_in );
		this->duplicator     [3]                                                                   ->s_out1(this->duplicator [4]                     ->s_in );
		this->duplicator     [4]                                                                   ->s_out1(this->coset_real [0]->sc_module          ->s_in1);
		this->duplicator     [4]                                                                   ->s_out2(this->coset_real_i  ->sc_module          ->s_in1);
		this->duplicator     [3]                                                                   ->s_out2(this->interleaver_e ->sc_module_inter    ->s_in );
		this->interleaver_e     ->sc_module_inter    ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator  [0]->sc_module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->sc_module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->sc_module_wg       ->s_in );
			this->channel    [0]->sc_module_wg       ->s_out1(this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in1);
			this->channel    [0]->sc_module_wg       ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in2);
			this->modulator  [0]->sc_module_tdemod_wg->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		} else {
			this->modulator  [0]->sc_module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->sc_module          ->s_in );
			this->channel    [0]->sc_module          ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in1);
			this->modulator  [0]->sc_module_tdemod   ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		}
		this->interleaver    [0]->sc_module_deinter  ->s_out (this->dbg_Q[2]->s_in); this->dbg_Q[2]->s_out (this->coset_real [0]->sc_module          ->s_in2);
		this->coset_real     [0]->sc_module          ->s_out (this->dbg_Q[5]->s_in); this->dbg_Q[5]->s_out (this->router                             ->s_in );
		this->router                                                                               ->s_out1(this->siso       [0]->sc_module_siso     ->s_in );
		this->router                                                                               ->s_out2(this->decoder    [0]->sc_module          ->s_in );
		this->siso           [0]->sc_module_siso     ->s_out (this->dbg_Q[3]->s_in); this->dbg_Q[3]->s_out (this->coset_real_i  ->sc_module          ->s_in2);
		this->coset_real_i      ->sc_module          ->s_out (this->dbg_Q[6]->s_in); this->dbg_Q[6]->s_out (this->interleaver[0]->sc_module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->sc_module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->sc_module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in2);
		}
		this->decoder        [0]->sc_module          ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->coset_bit  [0]->sc_module          ->s_in2);
		this->coset_bit      [0]->sc_module          ->s_out (this->dbg_B[5]->s_in); this->dbg_B[5]->s_out (this->crc        [0]->sc_module_extract  ->s_in );
		this->crc            [0]->sc_module_extract  ->s_out (this->dbg_B[6]->s_in); this->dbg_B[6]->s_out (this->duplicator [1]                     ->s_in );
		this->duplicator     [1]                                                                   ->s_out1(this->monitor    [0]->sc_module          ->s_in2);
		this->duplicator     [1]                                                                   ->s_out2(this->predicate                          ->s_in );
	}
	else // standard simulation
	{
		this->source         [0]->sc_module          ->s_out (this->dbg_B[0]->s_in); this->dbg_B[0]->s_out (this->duplicator [0]                     ->s_in );
		this->duplicator     [0]                     ->s_out1                                              (this->monitor    [0]->sc_module          ->s_in1);
		this->duplicator     [0]                     ->s_out2                                              (this->crc        [0]->sc_module_build    ->s_in );
		this->crc            [0]->sc_module_build    ->s_out (this->dbg_B[1]->s_in); this->dbg_B[1]->s_out (this->encoder    [0]->sc_module          ->s_in );
		this->encoder        [0]->sc_module          ->s_out (this->dbg_B[2]->s_in); this->dbg_B[2]->s_out (this->interleaver_e ->sc_module_inter    ->s_in );
		this->interleaver_e     ->sc_module_inter    ->s_out (this->dbg_B[3]->s_in); this->dbg_B[3]->s_out (this->modulator  [0]->sc_module_mod      ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->modulator  [0]->sc_module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->sc_module_wg       ->s_in );
			this->channel    [0]->sc_module_wg       ->s_out1(this->dbg_R[3]->s_in); this->dbg_R[3]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in1);
			this->channel    [0]->sc_module_wg       ->s_out2(this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in2);
			this->modulator  [0]->sc_module_tdemod_wg->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		} else {
			this->modulator  [0]->sc_module_mod      ->s_out (this->dbg_R[0]->s_in); this->dbg_R[0]->s_out (this->channel    [0]->sc_module          ->s_in );
			this->channel    [0]->sc_module          ->s_out (this->dbg_R[1]->s_in); this->dbg_R[1]->s_out (this->modulator  [0]->sc_module_filt     ->s_in );
			this->modulator  [0]->sc_module_filt     ->s_out (this->dbg_R[2]->s_in); this->dbg_R[2]->s_out (this->quantizer  [0]->sc_module          ->s_in );
			this->quantizer  [0]->sc_module          ->s_out (this->dbg_Q[0]->s_in); this->dbg_Q[0]->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in1);
			this->modulator  [0]->sc_module_tdemod   ->s_out (this->dbg_Q[1]->s_in); this->dbg_Q[1]->s_out (this->interleaver[0]->sc_module_deinter  ->s_in );
		}
		this->interleaver    [0]->sc_module_deinter  ->s_out (this->dbg_Q[2]->s_in); this->dbg_Q[2]->s_out (this->router                             ->s_in );
		this->router                                                                               ->s_out1(this->siso       [0]->sc_module_siso     ->s_in );
		this->router                                                                               ->s_out2(this->decoder    [0]->sc_module          ->s_in );
		this->siso           [0]->sc_module_siso     ->s_out (this->dbg_Q[3]->s_in); this->dbg_Q[3]->s_out (this->interleaver[0]->sc_module_inter    ->s_in );
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos) {
			this->interleaver[0]->sc_module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->sc_module_tdemod_wg->s_in3);
		} else {
			this->interleaver[0]->sc_module_inter    ->s_out (this->dbg_Q[4]->s_in); this->dbg_Q[4]->s_out (this->modulator  [0]->sc_module_tdemod   ->s_in2);
		}
		this->decoder        [0]->sc_module          ->s_out (this->dbg_B[4]->s_in); this->dbg_B[4]->s_out (this->crc        [0]->sc_module_extract  ->s_in );
		this->crc            [0]->sc_module_extract  ->s_out (this->dbg_B[6]->s_in); this->dbg_B[6]->s_out (this->duplicator [1]                     ->s_in );
		this->duplicator     [1]                                                                   ->s_out1(this->monitor    [0]->sc_module          ->s_in2);
		this->duplicator     [1]                                                                   ->s_out2(this->predicate                          ->s_in );
	}
}

template <typename B, typename R, typename Q>
Interleaver<int>* SC_Simulation_BFER_ite<B,R,Q>
::build_interleaver(const int tid)
{
	const auto seed = (this->params.interleaver.uniform) ? this->rd_engine_seed[tid]() : this->params.interleaver.seed;

	// build the objects
	this->interleaver_e = Factory_interleaver<int>::build(this->params.interleaver.type,
	                                                      this->params.code.N_code,
	                                                      this->params.interleaver.path,
	                                                      this->params.interleaver.uniform,
	                                                      this->params.interleaver.n_cols,
	                                                      seed,
	                                                      this->params.simulation.inter_frame_level);
	Simulation::check_errors(this->interleaver_e, "Interleaver<int>");

	this->interleaver_e->init();
	this->interleaver_e->rename("Interleaver_e");
	if (this->interleaver_e->is_uniform())
		this->monitor[tid]->add_handler_check(std::bind(&Interleaver<int>::refresh, this->interleaver_e));

	return Factory_interleaver<int>::build(this->params.interleaver.type,
	                                       this->params.code.N_code,
	                                       this->params.interleaver.path,
	                                       this->params.interleaver.uniform,
	                                       this->params.interleaver.n_cols,
	                                       seed,
	                                       this->params.simulation.inter_frame_level);
}

template <typename B, typename R, typename Q>
Coset<B,Q>* SC_Simulation_BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	this->coset_real_i = Simulation_BFER_ite<B,R,Q>::build_coset_real(tid);
	Simulation::check_errors(this->coset_real_i, "Coset<B,Q>");
	this->coset_real_i->rename("Coset_real_i");

	return Simulation_BFER_ite<B,R,Q>::build_coset_real(tid);
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::SC_Simulation_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::SC_Simulation_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::SC_Simulation_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::SC_Simulation_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::SC_Simulation_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif
