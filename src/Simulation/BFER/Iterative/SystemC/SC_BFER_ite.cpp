#ifdef SYSTEMC

#include "Factory/Module/Coset/Coset.hpp"

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/bash_tools.h"
#include "Tools/Algo/Predicate_ite.hpp"

#include "SC_BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SC_BFER_ite<B,R,Q>
::SC_BFER_ite(const factory::BFER_ite::parameters &params)
: BFER_ite<B,R,Q>(params),

  coset_real_i(nullptr),

  duplicator {nullptr, nullptr, nullptr, nullptr, nullptr},
  router     (nullptr),
  predicate  (nullptr)
{
	if (this->params.n_threads > 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support "
		                                                            "multi-threading.");

	if (params.coded_monitoring)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support the coded "
		                                                            "monitoring.");
}

template <typename B, typename R, typename Q>
SC_BFER_ite<B,R,Q>
::~SC_BFER_ite()
{
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::__build_communication_chain(const int tid)
{
	BFER_ite<B,R,Q>::__build_communication_chain(tid);

	this->interleaver_bit[tid]->rename(this->interleaver_llr[tid]->get_name() + "_bit");
	this->interleaver_llr[tid]->rename(this->interleaver_llr[tid]->get_name() + "_llr");

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
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::create_sc_modules()
{
	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source         [tid]               ->sc.create_module("generate"  );
	this->crc            [tid]               ->sc.create_module("build"     );
	this->codec          [tid]->get_encoder()->sc.create_module("encode"    );
	this->interleaver_bit[tid]               ->sc.create_module("interleave");
	this->modem          [tid]               ->sc.create_module("modulate"  );
	this->modem          [tid]               ->sc.create_module("filter"    );
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.create_module("add_noise_wg"  );
		this->modem  [tid]->sc.create_module("demodulate_wg" );
		this->modem  [tid]->sc.create_module("tdemodulate_wg");
	}
	else
	{
		this->channel[tid]->sc.create_module("add_noise"  );
		this->modem  [tid]->sc.create_module("demodulate" );
		this->modem  [tid]->sc.create_module("tdemodulate");
	}
	this->interleaver_llr[tid]                    ->sc.create_module("interleave"  );
	this->quantizer      [tid]                    ->sc.create_module("process"     );
	this->interleaver_llr[tid]                    ->sc.create_module("deinterleave");
	this->codec          [tid]->get_decoder_siho()->sc.create_module("decode_siho" );
	this->codec          [tid]->get_decoder_siso()->sc.create_module("decode_siso" );
	this->monitor        [tid]                    ->sc.create_module("check_errors");
	if (this->params.coset)
	{
		this->coset_real[tid]->sc.create_module("apply");
		this->coset_real_i   ->sc.create_module("apply");
		this->coset_bit [tid]->sc.create_module("apply");
	}
	this->crc[tid]->sc.create_module("extract");
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::erase_sc_modules()
{
	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source         [tid]               ->sc.erase_module("generate"  );
	this->crc            [tid]               ->sc.erase_module("build"     );
	this->codec          [tid]->get_encoder()->sc.erase_module("encode"    );
	this->interleaver_bit[tid]               ->sc.erase_module("interleave");
	this->modem          [tid]               ->sc.erase_module("modulate"  );
	this->modem          [tid]               ->sc.erase_module("filter"    );
	if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.erase_module("add_noise_wg"  );
		this->modem  [tid]->sc.erase_module("demodulate_wg" );
		this->modem  [tid]->sc.erase_module("tdemodulate_wg");
	}
	else
	{
		this->channel[tid]->sc.erase_module("add_noise"  );
		this->modem  [tid]->sc.erase_module("demodulate" );
		this->modem  [tid]->sc.erase_module("tdemodulate");
	}
	this->interleaver_llr[tid]                    ->sc.erase_module("interleave"  );
	this->quantizer      [tid]                    ->sc.erase_module("process"     );
	this->interleaver_llr[tid]                    ->sc.erase_module("deinterleave");
	this->codec          [tid]->get_decoder_siho()->sc.erase_module("decode_siho" );
	this->codec          [tid]->get_decoder_siso()->sc.erase_module("decode_siso" );
	this->monitor        [tid]                    ->sc.erase_module("check_errors");
	if (this->params.coset)
	{
		this->coset_real[tid]->sc.erase_module("apply");
		this->coset_real_i   ->sc.erase_module("apply");
		this->coset_bit [tid]->sc.erase_module("apply");
	}
	this->crc[tid]->sc.erase_module("extract");
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::release_objects()
{
	if (coset_real_i != nullptr) { delete coset_real_i; coset_real_i = nullptr; }

	BFER_ite<B,R,Q>::release_objects();
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::_launch()
{
	BFER_ite<B,R,Q>::_launch();

	this->create_sc_modules();

	tools::Predicate_ite p(this->params.n_ite);

	this->duplicator[0] = new tools::SC_Duplicator("Duplicator0");
	this->duplicator[1] = new tools::SC_Duplicator("Duplicator1");
	if (this->params.coset)
	{
		this->duplicator[2] = new tools::SC_Duplicator("Duplicator2");
		this->duplicator[3] = new tools::SC_Duplicator("Duplicator3");
		this->duplicator[4] = new tools::SC_Duplicator("Duplicator4");
	}
	this->router    = new tools::SC_Router   (p, "Router"   );
	this->predicate = new tools::SC_Predicate(p, "Predicate");

	this->bind_sockets();
	sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
	sc_core::sc_start(); // start simulation

	for (auto i = 0; i < 5; i++)
		if (this->duplicator[i] != nullptr)
		{
			delete this->duplicator[i];
			this->duplicator[i] = nullptr;
		}

	delete this->router;    this->router    = nullptr;
	delete this->predicate; this->predicate = nullptr;

	this->erase_sc_modules();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// /!\ VERY DIRTY WAY TO CREATE A NEW SIMULATION CONTEXT IN SYSTEMC, BE CAREFUL THIS IS NOT IN THE STD! /!\ //
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sc_core::sc_curr_simcontext = new sc_core::sc_simcontext();
	sc_core::sc_default_global_context = sc_core::sc_curr_simcontext;
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::bind_sockets()
{
	auto &duplicator0 = *this->duplicator[0];
	auto &duplicator1 = *this->duplicator[1];
	auto &duplicator2 = *this->duplicator[2];
	auto &duplicator3 = *this->duplicator[3];
	auto &duplicator4 = *this->duplicator[4];

	auto &router    = *this->router;
	auto &predicate = *this->predicate;

	auto &source          = *this->source         [0];
	auto &crc             = *this->crc            [0];
	auto &encoder         = *this->codec          [0]->get_encoder();
	auto &interleaver_bit = *this->interleaver_bit[0];
	auto &interleaver_llr = *this->interleaver_llr[0];
	auto &modem           = *this->modem          [0];
	auto &channel         = *this->channel        [0];
	auto &quantizer       = *this->quantizer      [0];
	auto &coset_real      = *this->coset_real     [0];
	auto &coset_real_i    = *this->coset_real_i;
	auto &decoder_siho    = *this->codec          [0]->get_decoder_siho();
	auto &decoder_siso    = *this->codec          [0]->get_decoder_siso();
	auto &coset_bit       = *this->coset_bit      [0];
	auto &monitor         = *this->monitor        [0];

	if (this->params.coset)
	{
		source             .sc["generate"      ].s_out ["U_K"     ](duplicator0                         .s_in           );
		duplicator0                             .s_out1            (monitor        .sc["check_errors"  ].s_in["U"      ]);
		duplicator0                             .s_out2            (crc            .sc["build"         ].s_in["U_K1"   ]);
		crc                .sc["build"         ].s_out ["U_K2"    ](duplicator2                         .s_in           );
		duplicator2                             .s_out1            (coset_bit      .sc["apply"         ].s_in["ref"    ]);
		duplicator2                             .s_out2            (encoder        .sc["encode"        ].s_in["U_K"    ]);
		encoder            .sc["encode"        ].s_out ["X_N"     ](duplicator3                         .s_in           );
		duplicator3                             .s_out1            (duplicator4                         .s_in           );
		duplicator4                             .s_out1            (coset_real     .sc["apply"         ].s_in["ref"    ]);
		duplicator4                             .s_out2            (coset_real_i   .sc["apply"         ].s_in["ref"    ]);
		duplicator3                             .s_out2            (interleaver_bit.sc["interleave"    ].s_in["nat"    ]);
		interleaver_bit    .sc["interleave"    ].s_out ["itl"     ](modem          .sc["modulate"      ].s_in["X_N1"   ]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) {
			modem          .sc["modulate"      ].s_out ["X_N2"    ](channel        .sc["add_noise_wg"  ].s_in["X_N"    ]);
			channel        .sc["add_noise_wg"  ].s_out ["H_N"     ](modem          .sc["tdemodulate_wg"].s_in["H_N"    ]);
			channel        .sc["add_noise_wg"  ].s_out ["Y_N"     ](modem          .sc["filter"        ].s_in["Y_N1"   ]);
			modem          .sc["filter"        ].s_out ["Y_N2"    ](quantizer      .sc["process"       ].s_in["Y_N1"   ]);
			quantizer      .sc["process"       ].s_out ["Y_N2"    ](modem          .sc["tdemodulate_wg"].s_in["Y_N1"   ]);
			modem          .sc["tdemodulate_wg"].s_out ["Y_N3"    ](interleaver_llr.sc["deinterleave"  ].s_in["itl"    ]);
		} else {
			modem          .sc["modulate"      ].s_out ["X_N2"    ](channel        .sc["add_noise"     ].s_in["X_N"    ]);
			channel        .sc["add_noise"     ].s_out ["Y_N"     ](modem          .sc["filter"        ].s_in["Y_N1"   ]);
			modem          .sc["filter"        ].s_out ["Y_N2"    ](quantizer      .sc["process"       ].s_in["Y_N1"   ]);
			quantizer      .sc["process"       ].s_out ["Y_N2"    ](modem          .sc["tdemodulate"   ].s_in["Y_N1"   ]);
			modem          .sc["tdemodulate"   ].s_out ["Y_N3"    ](interleaver_llr.sc["deinterleave"  ].s_in["itl"    ]);
		}
		interleaver_llr    .sc["deinterleave"  ].s_out ["nat"     ](coset_real     .sc["apply"         ].s_in["in_data"]);
		coset_real         .sc["apply"         ].s_out ["out_data"](router                              .s_in           );
		router                                  .s_out1            (decoder_siso   .sc["decode_siso"   ].s_in["Y_N1"   ]);
		router                                  .s_out2            (decoder_siho   .sc["decode_siho"   ].s_in["Y_N"    ]);
		decoder_siso       .sc["decode_siso"   ].s_out ["Y_N2"    ](coset_real_i   .sc["apply"         ].s_in["in_data"]);
		coset_real_i       .sc["apply"         ].s_out ["out_data"](interleaver_llr.sc["interleave"    ].s_in["nat"    ]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) {
			interleaver_llr.sc["interleave"    ].s_out ["itl"     ](modem          .sc["tdemodulate_wg"].s_in["Y_N2"   ]);
		} else {
			interleaver_llr.sc["interleave"    ].s_out ["itl"     ](modem          .sc["tdemodulate"   ].s_in["Y_N2"   ]);
		}
		decoder_siho       .sc["decode_siho"   ].s_out ["V_K"     ](coset_bit      .sc["apply"         ].s_in["in_data"]);
		coset_bit          .sc["apply"         ].s_out ["out_data"](crc            .sc["extract"       ].s_in["V_K1"   ]);
		crc                .sc["extract"       ].s_out ["V_K2"    ](duplicator1                         .s_in           );
		duplicator1                             .s_out1            (monitor        .sc["check_errors"  ].s_in["V"      ]);
		duplicator1                             .s_out2            (predicate                           .s_in           );
	}
	else // standard simulation
	{
		source             .sc["generate"      ].s_out ["U_K" ](duplicator0                         .s_in        );
		duplicator0                             .s_out1        (monitor        .sc["check_errors"  ].s_in["U"   ]);
		duplicator0                             .s_out2        (crc            .sc["build"         ].s_in["U_K1"]);
		crc                .sc["build"         ].s_out ["U_K2"](encoder        .sc["encode"        ].s_in["U_K" ]);
		encoder            .sc["encode"        ].s_out ["X_N" ](interleaver_bit.sc["interleave"    ].s_in["nat" ]);
		interleaver_bit    .sc["interleave"    ].s_out ["itl" ](modem          .sc["modulate"      ].s_in["X_N1"]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) {
			modem          .sc["modulate"      ].s_out ["X_N2"](channel        .sc["add_noise_wg"  ].s_in["X_N" ]);
			channel        .sc["add_noise_wg"  ].s_out ["H_N" ](modem          .sc["tdemodulate_wg"].s_in["H_N" ]);
			channel        .sc["add_noise_wg"  ].s_out ["Y_N" ](modem          .sc["filter"        ].s_in["Y_N1"]);
			modem          .sc["filter"        ].s_out ["Y_N2"](quantizer      .sc["process"       ].s_in["Y_N1"]);
			quantizer      .sc["process"       ].s_out ["Y_N2"](modem          .sc["tdemodulate_wg"].s_in["Y_N1"]);
			modem          .sc["tdemodulate_wg"].s_out ["Y_N3"](interleaver_llr.sc["deinterleave"  ].s_in["itl" ]);
		} else {
			modem          .sc["modulate"      ].s_out ["X_N2"](channel        .sc["add_noise"     ].s_in["X_N" ]);
			channel        .sc["add_noise"     ].s_out ["Y_N" ](modem          .sc["filter"        ].s_in["Y_N" ]);
			modem          .sc["filter"        ].s_out ["Y_N2"](quantizer      .sc["process"       ].s_in["Y_N1"]);
			quantizer      .sc["process"       ].s_out ["Y_N2"](modem          .sc["tdemodulate"   ].s_in["Y_N1"]);
			modem          .sc["tdemodulate"   ].s_out ["Y_N3"](interleaver_llr.sc["deinterleave"  ].s_in["itl" ]);
		}
		interleaver_llr    .sc["deinterleave"  ].s_out ["nat" ](router                              .s_in        );
		router                                  .s_out1        (decoder_siso   .sc["decode_siso"   ].s_in["Y_N1"]);
		router                                  .s_out2        (decoder_siho   .sc["decode_siho"   ].s_in["Y_N" ]);
		decoder_siso       .sc["decode_siso"   ].s_out ["Y_N2"](interleaver_llr.sc["interleave"    ].s_in["nat" ]);
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos) {
			interleaver_llr.sc["interleave"    ].s_out ["itl" ](modem          .sc["tdemodulate_wg"].s_in["Y_N2"]);
		} else {
			interleaver_llr.sc["interleave"    ].s_out ["itl" ](modem          .sc["tdemodulate"   ].s_in["Y_N2"]);
		}
		decoder_siho       .sc["decode_siho"   ].s_out ["V_K" ](crc            .sc["extract"       ].s_in["V_K1"]);
		crc                .sc["extract"       ].s_out ["V_K2"](duplicator1                         .s_in        );
		duplicator1                             .s_out1        (monitor        .sc["check_errors"  ].s_in["V"   ]);
		duplicator1                             .s_out2        (predicate                           .s_in        );
	}
}

template <typename B, typename R, typename Q>
module::Coset<B,Q>* SC_BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size = this->params.cdc->N_cw;
	cst_params.n_frames = this->params.src->n_frames;

	this->coset_real_i = cst_params.template build_real<B,Q>();
	this->coset_real_i->rename("Coset_real_i");

	return cst_params.template build_real<B,Q>();
}

// ==================================================================================== explicit template instantiation 
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::SC_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::SC_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::SC_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::SC_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::SC_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif
