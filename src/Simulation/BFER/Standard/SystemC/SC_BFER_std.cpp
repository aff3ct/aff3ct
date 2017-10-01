#ifdef SYSTEMC

#include <iostream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"

#include "SC_BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SC_BFER_std<B,R,Q>
::SC_BFER_std(const factory::BFER_std::parameters &params)
: BFER_std<B,R,Q>(params),

  duplicator{nullptr, nullptr, nullptr}
{
	if (this->params.n_threads > 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support "
		                                                            "multi-threading.");

	if (params.coded_monitoring)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support the coded "
		                                                            "monitoring.");
}

template <typename B, typename R, typename Q>
SC_BFER_std<B,R,Q>
::~SC_BFER_std()
{
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::__build_communication_chain(const int tid)
{
	BFER_std<B,R,Q>::__build_communication_chain(tid);

	if (this->params.err_track_enable)
	{
		auto &source  = *this->source[tid];
		auto &encoder = *this->codec [tid]->get_encoder();

		if (this->params.src->type != "AZCW")
		{
			auto src_data = (B*)(source["generate"]["U_K"].get_dataptr());
			auto src_size = source["generate"]["U_K"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(src_data, src_size, "src", false, {});
		}

		if (this->params.coset)
		{
			auto enc_data = (B*)(encoder["encode"]["X_N"].get_dataptr());
			auto enc_size = encoder["encode"]["X_N"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(enc_data, enc_size, "enc", false, {(unsigned)this->params.cdc->enc->K});
		}
	}

	this->monitor[tid]->add_handler_check([&]() -> void
	{
		if (this->monitor_red->fe_limit_achieved()) // will make the MPI communication
			sc_core::sc_stop();
	});
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::create_sc_modules()
{
	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source   [tid]                 ->sc.create_module("generate"  );
	this->crc      [tid]                 ->sc.create_module("build"     );
	this->codec    [tid]->get_encoder()  ->sc.create_module("encode"    );
	this->codec    [tid]->get_puncturer()->sc.create_module("puncture"  );
	this->codec    [tid]->get_puncturer()->sc.create_module("depuncture");
	this->modem    [tid]                 ->sc.create_module("modulate"  );
	this->modem    [tid]                 ->sc.create_module("filter"    );
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.create_module("add_noise_wg" );
		this->modem  [tid]->sc.create_module("demodulate_wg");
	}
	else
	{
		this->channel[tid]->sc.create_module("add_noise" );
		this->modem  [tid]->sc.create_module("demodulate");
	}
	this->quantizer[tid]                    ->sc.create_module("process"     );
	this->codec    [tid]->get_decoder_siho()->sc.create_module("decode_siho" );
	this->monitor  [tid]                    ->sc.create_module("check_errors");
	if (this->params.coset)
	{
		this->coset_real[tid]->sc.create_module("apply");
		this->coset_bit [tid]->sc.create_module("apply");
	}
	this->crc[tid]->sc.create_module("extract");
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::erase_sc_modules()
{
	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source   [tid]                 ->sc.erase_module("generate"  );
	this->crc      [tid]                 ->sc.erase_module("build"     );
	this->codec    [tid]->get_encoder()  ->sc.erase_module("encode"    );
	this->codec    [tid]->get_puncturer()->sc.erase_module("puncture"  );
	this->codec    [tid]->get_puncturer()->sc.erase_module("depuncture");
	this->modem    [tid]                 ->sc.erase_module("modulate"  );
	this->modem    [tid]                 ->sc.erase_module("filter"    );
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.erase_module("add_noise_wg" );
		this->modem  [tid]->sc.erase_module("demodulate_wg");
	}
	else
	{
		this->channel[tid]->sc.erase_module("add_noise" );
		this->modem  [tid]->sc.erase_module("demodulate");
	}
	this->quantizer[tid]                    ->sc.erase_module("process"     );
	this->codec    [tid]->get_decoder_siho()->sc.erase_module("decode_siho" );
	this->monitor  [tid]                    ->sc.erase_module("check_errors");
	if (this->params.coset)
	{
		this->coset_real[tid]->sc.erase_module("apply");
		this->coset_bit [tid]->sc.erase_module("apply");
	}
	this->crc[tid]->sc.erase_module("extract");
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::_launch()
{
	BFER_std<B,R,Q>::_launch();

	this->create_sc_modules();

	this->duplicator[0] = new tools::SC_Duplicator("Duplicator0");
	if (this->params.coset)
	{
		this->duplicator[1] = new tools::SC_Duplicator("Duplicator1");
		this->duplicator[2] = new tools::SC_Duplicator("Duplicator2");
	}

	this->bind_sockets();
	sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
	sc_core::sc_start(); // start simulation

	for (auto i = 0; i < 3; i++)
		if (this->duplicator[i] != nullptr)
		{
			delete this->duplicator[i];
			this->duplicator[i] = nullptr;
		}

	this->erase_sc_modules();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STD! /!\ //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
	sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::bind_sockets()
{
	auto &duplicator0 = *this->duplicator[0];
	auto &duplicator1 = *this->duplicator[1];
	auto &duplicator2 = *this->duplicator[2];

	auto &source     = *this->source    [0];
	auto &crc        = *this->crc       [0];
	auto &encoder    = *this->codec     [0]->get_encoder();
	auto &puncturer  = *this->codec     [0]->get_puncturer();
	auto &modem      = *this->modem     [0];
	auto &channel    = *this->channel   [0];
	auto &quantizer  = *this->quantizer [0];
	auto &coset_real = *this->coset_real[0];
	auto &decoder    = *this->codec     [0]->get_decoder_siho();
	auto &coset_bit  = *this->coset_bit [0];
	auto &monitor    = *this->monitor   [0];

	if (this->params.coset)
	{
		source     .sc["generate"     ].s_out ["U_K"     ](duplicator0                    .s_in            );
		duplicator0                    .s_out1            (monitor    .sc["check_errors" ].s_in ["U"      ]);
		duplicator0                    .s_out2            (crc        .sc["build"        ].s_in ["U_K1"   ]);
		crc        .sc["build"        ].s_out ["U_K2"    ](duplicator1                    .s_in            );
		duplicator1                    .s_out1            (coset_bit  .sc["apply"        ].s_in ["ref"    ]);
		duplicator1                    .s_out2            (encoder    .sc["encode"       ].s_in ["U_K"    ]);
		encoder    .sc["encode"       ].s_out ["X_N"     ](duplicator2                    .s_in            );
		duplicator2                    .s_out1            (coset_real .sc["apply"        ].s_in ["ref"    ]);
		duplicator2                    .s_out2            (puncturer  .sc["puncture"     ].s_in ["X_N1"   ]);
		puncturer  .sc["puncture"     ].s_out ["X_N2"    ](modem      .sc["modulate"     ].s_in ["X_N1"   ]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
			modem  .sc["modulate"     ].s_out ["X_N2"    ](channel    .sc["add_noise_wg" ].s_in ["X_N"    ]);
			channel.sc["add_noise_wg" ].s_out ["H_N"     ](modem      .sc["demodulate_wg"].s_in ["H_N"    ]);
			channel.sc["add_noise_wg" ].s_out ["Y_N"     ](modem      .sc["filter"       ].s_in ["Y_N1"   ]);
			modem  .sc["filter"       ].s_out ["Y_N2"    ](modem      .sc["demodulate_wg"].s_in ["Y_N1"   ]);
			modem  .sc["demodulate_wg"].s_out ["Y_N2"    ](quantizer  .sc["process"      ].s_in ["Y_N1"   ]);
		} else { // additive channel (AWGN, USER, NO)
			modem  .sc["modulate"     ].s_out ["X_N2"    ](channel    .sc["add_noise"    ].s_in ["X_N"    ]);
			channel.sc["add_noise"    ].s_out ["Y_N"     ](modem      .sc["filter"       ].s_in ["Y_N1"   ]);
			modem  .sc["filter"       ].s_out ["Y_N2"    ](modem      .sc["demodulate"   ].s_in ["Y_N1"   ]);
			modem  .sc["demodulate"   ].s_out ["Y_N2"    ](quantizer  .sc["process"      ].s_in ["Y_N1"   ]);
		}
		quantizer  .sc["process"      ].s_out ["Y_N2"    ](puncturer  .sc["depuncture"   ].s_in ["Y_N1"   ]);
		puncturer  .sc["depuncture"   ].s_out ["Y_N2"    ](coset_real .sc["apply"        ].s_in ["in_data"]);
		coset_real .sc["apply"        ].s_out ["out_data"](decoder    .sc["decode_siho"  ].s_in ["Y_N"    ]);
		decoder    .sc["decode_siho"  ].s_out ["V_K"     ](coset_bit  .sc["apply"        ].s_in ["in_data"]);
		coset_bit  .sc["apply"        ].s_out ["out_data"](crc        .sc["extract"      ].s_in ["V_K1"   ]);
		crc        .sc["extract"      ].s_out ["V_K2"    ](monitor    .sc["check_errors" ].s_in ["V"      ]);
	}
	else // standard simulation
	{
		source     .sc["generate"     ].s_out ["U_K" ](duplicator0                    .s_in         );
		duplicator0                    .s_out1        (monitor    .sc["check_errors" ].s_in ["U"   ]);
		duplicator0                    .s_out2        (crc        .sc["build"        ].s_in ["U_K1"]);
		crc        .sc["build"        ].s_out ["U_K2"](encoder    .sc["encode"       ].s_in ["U_K" ]);
		encoder    .sc["encode"       ].s_out ["X_N" ](puncturer  .sc["puncture"     ].s_in ["X_N1"]);
		puncturer  .sc["puncture"     ].s_out ["X_N2"](modem      .sc["modulate"     ].s_in ["X_N1"]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) { // Rayleigh channel
			modem  .sc["modulate"     ].s_out ["X_N2"](channel    .sc["add_noise_wg" ].s_in ["X_N" ]);
			channel.sc["add_noise_wg" ].s_out ["H_N" ](modem      .sc["demodulate_wg"].s_in ["H_N" ]);
			channel.sc["add_noise_wg" ].s_out ["Y_N" ](modem      .sc["filter"       ].s_in ["Y_N1"]);
			modem  .sc["filter"       ].s_out ["Y_N2"](modem      .sc["demodulate_wg"].s_in ["Y_N1"]);
			modem  .sc["demodulate_wg"].s_out ["Y_N2"](quantizer  .sc["process"      ].s_in ["Y_N1"]);
		} else { // additive channel (AWGN, USER, NO)
			modem  .sc["modulate"     ].s_out ["X_N2"](channel    .sc["add_noise"    ].s_in ["X_N" ]);
			channel.sc["add_noise"    ].s_out ["Y_N" ](modem      .sc["filter"       ].s_in ["Y_N1"]);
			modem  .sc["filter"       ].s_out ["Y_N2"](modem      .sc["demodulate"   ].s_in ["Y_N1"]);
			modem  .sc["demodulate"   ].s_out ["Y_N2"](quantizer  .sc["process"      ].s_in ["Y_N1"]);
		}
		quantizer  .sc["process"      ].s_out ["Y_N2"](puncturer  .sc["depuncture"   ].s_in ["Y_N1"]);
		puncturer  .sc["depuncture"   ].s_out ["Y_N2"](decoder    .sc["decode_siho"  ].s_in ["Y_N" ]);
		decoder    .sc["decode_siho"  ].s_out ["V_K" ](crc        .sc["extract"      ].s_in ["V_K1"]);
		crc        .sc["extract"      ].s_out ["V_K2"](monitor    .sc["check_errors" ].s_in ["V"   ]);
	}
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::SC_BFER_std<B_8,R_8,Q_8>;
template class aff3ct::simulation::SC_BFER_std<B_16,R_16,Q_16>;
template class aff3ct::simulation::SC_BFER_std<B_32,R_32,Q_32>;
template class aff3ct::simulation::SC_BFER_std<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::SC_BFER_std<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif /* SYSTEMC*/
