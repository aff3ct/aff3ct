#ifdef AFF3CT_SYSTEMC

#include "Factory/Module/Coset/Coset.hpp"

#include "Tools/Exception/exception.hpp"
#include "Tools/Algo/Predicate_ite.hpp"

#include "SC_BFER_ite.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SC_BFER_ite<B,R,Q>
::SC_BFER_ite(const factory::BFER_ite::parameters &params_BFER_ite)
: BFER_ite<B,R,Q>(params_BFER_ite),
  duplicator(7)
{
	if (this->params_BFER_ite.n_threads > 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support "
		                                                            "multi-threading.");

	if (params_BFER_ite.coded_monitoring)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "SystemC simulation does not support the coded "
		                                                            "monitoring.");

	this->add_module("coset_real_i", params_BFER_ite.n_threads);
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::__build_communication_chain(const int tid)
{
	BFER_ite<B,R,Q>::__build_communication_chain(tid);

	this->set_module("coset_real_i", tid, coset_real_i);

	this->interleaver_bit[tid]->set_name(this->interleaver_llr[tid]->get_name() + "_bit");
	this->interleaver_llr[tid]->set_name(this->interleaver_llr[tid]->get_name() + "_llr");

	this->monitor_er[tid]->add_handler_check([&]() -> void
	{
		if (!this->keep_looping_noise_point()) // will make the MPI communication
			sc_core::sc_stop();
	});
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::create_sc_modules()
{
	using namespace module;
	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source         [tid]               ->sc.create_module(+src::tsk::generate  );
	this->crc            [tid]               ->sc.create_module(+crc::tsk::build     );
	this->codec          [tid]->get_encoder()->sc.create_module(+enc::tsk::encode    );
	this->interleaver_bit[tid]               ->sc.create_module(+itl::tsk::interleave);
	this->modem          [tid]               ->sc.create_module(+mdm::tsk::modulate  );
	this->modem          [tid]               ->sc.create_module(+mdm::tsk::filter    );
	if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.create_module(+chn::tsk::add_noise_wg  );
		this->modem  [tid]->sc.create_module(+mdm::tsk::demodulate_wg );
		this->modem  [tid]->sc.create_module(+mdm::tsk::tdemodulate_wg);
	}
	else
	{
		this->channel[tid]->sc.create_module(+chn::tsk::add_noise  );
		this->modem  [tid]->sc.create_module(+mdm::tsk::demodulate );
		this->modem  [tid]->sc.create_module(+mdm::tsk::tdemodulate);
	}
	this->interleaver_llr[tid]                    ->sc.create_module(+itl::tsk::interleave  );
	this->quantizer      [tid]                    ->sc.create_module(+qnt::tsk::process     );
	this->interleaver_llr[tid]                    ->sc.create_module(+itl::tsk::deinterleave);
	this->codec          [tid]->get_decoder_siho()->sc.create_module(+dec::tsk::decode_siho );
	this->codec          [tid]->get_decoder_siso()->sc.create_module(+dec::tsk::decode_siso );
	this->monitor_er     [tid]                    ->sc.create_module(+mnt::tsk::check_errors);

	if (this->params_BFER_ite.coset)
	{
		this->coset_real[tid]->sc.create_module(+cst::tsk::apply);
		this->coset_real_i   ->sc.create_module(+cst::tsk::apply);
		this->coset_bit [tid]->sc.create_module(+cst::tsk::apply);
	}
	this->crc[tid]->sc.create_module(+crc::tsk::extract);
}

template <typename B, typename R, typename Q>
void SC_BFER_ite<B,R,Q>
::_launch()
{
	BFER_ite<B,R,Q>::_launch();

	this->create_sc_modules();

	tools::Predicate_ite p(this->params_BFER_ite.n_ite);

	this->duplicator[0].reset( new tools::SC_Duplicator("Duplicator0"));
	this->duplicator[1].reset( new tools::SC_Duplicator("Duplicator1"));
	this->duplicator[5].reset( new tools::SC_Duplicator("Duplicator5"));
	if (this->params_BFER_ite.coset)
	{
		this->duplicator[2].reset( new tools::SC_Duplicator("Duplicator2"));
		this->duplicator[3].reset( new tools::SC_Duplicator("Duplicator3"));
		this->duplicator[4].reset( new tools::SC_Duplicator("Duplicator4"));
	}
	if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->duplicator[6].reset( new tools::SC_Duplicator("Duplicator6"));
	}


	this->router   .reset( new tools::SC_Router   (p, "Router"   ));
	this->funnel   .reset( new tools::SC_Funnel   (   "Funnel"   ));
	this->predicate.reset( new tools::SC_Predicate(p, "Predicate"));

	this->bind_sockets();
	sc_core::sc_report_handler::set_actions(sc_core::SC_INFO, sc_core::SC_DO_NOTHING);
	sc_core::sc_start(); // start simulation


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
	auto &dp0 = *this->duplicator[0];
	auto &dp1 = *this->duplicator[1];
	auto &dp5 = *this->duplicator[5];

	auto &rtr = *this->router;
	auto &fnl = *this->funnel;
	auto &prd = *this->predicate;

	auto &src = *this->source         [0];
	auto &crc = *this->crc            [0];
	auto &enc = *this->codec          [0]->get_encoder();
	auto &itb = *this->interleaver_bit[0];
	auto &itl = *this->interleaver_llr[0];
	auto &mdm = *this->modem          [0];
	auto &chn = *this->channel        [0];
	auto &qnt = *this->quantizer      [0];
	auto &csr = *this->coset_real     [0];
	auto &csi = *this->coset_real_i;
	auto &dch = *this->codec          [0]->get_decoder_siho();
	auto &dcs = *this->codec          [0]->get_decoder_siso();
	auto &csb = *this->coset_bit      [0];
	auto &mnt = *this->monitor_er     [0];

	using namespace module;
	if (this->params_BFER_ite.coset)
	{
		auto &dp2 = *this->duplicator[2];
		auto &dp3 = *this->duplicator[3];
		auto &dp4 = *this->duplicator[4];

		src.sc    [+src::tsk::generate      ].s_out [+src::sck::generate      ::U_K ](dp0                              .s_in                                  );
		dp0                                  .s_out1                                 (mnt.sc[+mnt::tsk::check_errors  ].s_in [+mnt::sck::check_errors  ::U   ]);
		dp0                                  .s_out2                                 (crc.sc[+crc::tsk::build         ].s_in [+crc::sck::build         ::U_K1]);
		crc.sc    [+crc::tsk::build         ].s_out [+crc::sck::build         ::U_K2](dp2                              .s_in                                  );
		dp2                                  .s_out1                                 (csb.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::ref ]);
		dp2                                  .s_out2                                 (enc.sc[+enc::tsk::encode        ].s_in [+enc::sck::encode        ::U_K ]);
		enc.sc    [+enc::tsk::encode        ].s_out [+enc::sck::encode        ::X_N ](dp3                              .s_in                                  );
		dp3                                  .s_out1                                 (dp4                              .s_in                                  );
		dp4                                  .s_out1                                 (csr.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::ref ]);
		dp4                                  .s_out2                                 (csi.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::ref ]);
		dp3                                  .s_out2                                 (itb.sc[+itl::tsk::interleave    ].s_in [+itl::sck::interleave    ::nat ]);
		itb.sc    [+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::modulate      ].s_in [+mdm::sck::modulate      ::X_N1]);
		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos) {
			auto &dp6 = *this->duplicator[6];
			mdm.sc[+mdm::tsk::modulate      ].s_out [+mdm::sck::modulate      ::X_N2](chn.sc[+chn::tsk::add_noise_wg  ].s_in [+chn::sck::add_noise_wg  ::X_N ]);
			chn.sc[+chn::tsk::add_noise_wg  ].s_out [+chn::sck::add_noise_wg  ::H_N ](dp6                              .s_in                                  );
			dp6                              .s_out1                                 (mdm.sc[+mdm::tsk::demodulate_wg ].s_in [+mdm::sck::demodulate_wg ::H_N ]);
			dp6                              .s_out2                                 (mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::H_N ]);
			chn.sc[+chn::tsk::add_noise_wg  ].s_out [+chn::sck::add_noise_wg  ::Y_N ](mdm.sc[+mdm::tsk::filter        ].s_in [+mdm::sck::filter        ::Y_N1]);
			mdm.sc[+mdm::tsk::filter        ].s_out [+mdm::sck::filter        ::Y_N2](qnt.sc[+qnt::tsk::process       ].s_in [+qnt::sck::process       ::Y_N1]);
			qnt.sc[+qnt::tsk::process       ].s_out [+qnt::sck::process       ::Y_N2](dp5                              .s_in                                  );
			dp5                              .s_out1                                 (mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::Y_N1]);
			dp5                              .s_out2                                 (mdm.sc[+mdm::tsk::demodulate_wg ].s_in [+mdm::sck::demodulate_wg ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate_wg ].s_out [+mdm::sck::demodulate_wg ::Y_N2](fnl                              .s_in1                                 );
		} else {
			mdm.sc[+mdm::tsk::modulate      ].s_out [+mdm::sck::modulate      ::X_N2](chn.sc[+chn::tsk::add_noise     ].s_in [+chn::sck::add_noise     ::X_N ]);
			chn.sc[+chn::tsk::add_noise     ].s_out [+chn::sck::add_noise     ::Y_N ](mdm.sc[+mdm::tsk::filter        ].s_in [+mdm::sck::filter        ::Y_N1]);
			mdm.sc[+mdm::tsk::filter        ].s_out [+mdm::sck::filter        ::Y_N2](qnt.sc[+qnt::tsk::process       ].s_in [+qnt::sck::process       ::Y_N1]);
			qnt.sc[+qnt::tsk::process       ].s_out [+qnt::sck::process       ::Y_N2](dp5                              .s_in                                 );
			dp5                              .s_out1                                 (mdm.sc[+mdm::tsk::tdemodulate   ].s_in [+mdm::sck::tdemodulate   ::Y_N1]);
			dp5                              .s_out2                                 (mdm.sc[+mdm::tsk::demodulate    ].s_in [+mdm::sck::demodulate    ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate    ].s_out [+mdm::sck::demodulate    ::Y_N2](fnl                              .s_in1                                 );
		}
		fnl                                  .s_out                                  (itl.sc[+itl::tsk::deinterleave  ].s_in [+itl::sck::deinterleave  ::itl ]);
		itl.sc    [+itl::tsk::deinterleave  ].s_out [+itl::sck::deinterleave  ::nat ](csr.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::in  ]);
		csr.sc    [+cst::tsk::apply         ].s_out [+cst::sck::apply         ::out ](rtr                              .s_in                                  );
		rtr                                  .s_out1                                 (dcs.sc[+dec::tsk::decode_siso   ].s_in [+dec::sck::decode_siso   ::Y_N1]);
		rtr                                  .s_out2                                 (dch.sc[+dec::tsk::decode_siho   ].s_in [+dec::sck::decode_siho   ::Y_N ]);
		dcs.sc    [+dec::tsk::decode_siso   ].s_out [+dec::sck::decode_siso   ::Y_N2](csi.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::in  ]);
		csi.sc    [+cst::tsk::apply         ].s_out [+cst::sck::apply         ::out ](itl.sc[+itl::tsk::interleave    ].s_in [+itl::sck::interleave    ::nat ]);
		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos) {
			itl.sc[+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::Y_N2]);
			mdm.sc[+mdm::tsk::tdemodulate_wg].s_out [+mdm::sck::tdemodulate_wg::Y_N3](fnl                              .s_in2                                 );
		} else {
			itl.sc[+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::tdemodulate   ].s_in [+mdm::sck::tdemodulate   ::Y_N2]);
			mdm.sc[+mdm::tsk::tdemodulate   ].s_out [+mdm::sck::tdemodulate   ::Y_N3](fnl                              .s_in2                                 );
		}
		dch.sc    [+dec::tsk::decode_siho   ].s_out [+dec::sck::decode_siho   ::V_K ](csb.sc[+cst::tsk::apply         ].s_in [+cst::sck::apply         ::in  ]);
		csb.sc    [+cst::tsk::apply         ].s_out [+cst::sck::apply         ::out ](crc.sc[+crc::tsk::extract       ].s_in [+crc::sck::extract       ::V_K1]);
		crc.sc    [+crc::tsk::extract       ].s_out [+crc::sck::extract       ::V_K2](dp1                              .s_in                                  );
		dp1                                  .s_out1                                 (mnt.sc[+mnt::tsk::check_errors  ].s_in [+mnt::sck::check_errors  ::V   ]);
		dp1                                  .s_out2                                 (prd                              .s_in                                  );
	}
	else // standard simulation
	{
		src.sc    [+src::tsk::generate      ].s_out [+src::sck::generate      ::U_K ](dp0                              .s_in                                  );
		dp0                                  .s_out1                                 (mnt.sc[+mnt::tsk::check_errors  ].s_in [+mnt::sck::check_errors  ::U   ]);
		dp0                                  .s_out2                                 (crc.sc[+crc::tsk::build         ].s_in [+crc::sck::build         ::U_K1]);
		crc.sc    [+crc::tsk::build         ].s_out [+crc::sck::build         ::U_K2](enc.sc[+enc::tsk::encode        ].s_in [+enc::sck::encode        ::U_K ]);
		enc.sc    [+enc::tsk::encode        ].s_out [+enc::sck::encode        ::X_N ](itb.sc[+itl::tsk::interleave    ].s_in [+itl::sck::interleave    ::nat ]);
		itb.sc    [+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::modulate      ].s_in [+mdm::sck::modulate      ::X_N1]);
		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos) {
			auto &dp6 = *this->duplicator[6];
			mdm.sc[+mdm::tsk::modulate      ].s_out [+mdm::sck::modulate      ::X_N2](chn.sc[+chn::tsk::add_noise_wg  ].s_in [+chn::sck::add_noise_wg  ::X_N ]);
			chn.sc[+chn::tsk::add_noise_wg  ].s_out [+chn::sck::add_noise_wg  ::H_N ](dp6                              .s_in                                  );
			dp6                              .s_out1                                 (mdm.sc[+mdm::tsk::demodulate_wg ].s_in [+mdm::sck::demodulate_wg ::H_N ]);
			dp6                              .s_out2                                 (mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::H_N ]);
			chn.sc[+chn::tsk::add_noise_wg  ].s_out [+chn::sck::add_noise_wg  ::Y_N ](mdm.sc[+mdm::tsk::filter        ].s_in [+mdm::sck::filter        ::Y_N1]);
			mdm.sc[+mdm::tsk::filter        ].s_out [+mdm::sck::filter        ::Y_N2](qnt.sc[+qnt::tsk::process       ].s_in [+qnt::sck::process       ::Y_N1]);
			qnt.sc[+qnt::tsk::process       ].s_out [+qnt::sck::process       ::Y_N2](dp5                              .s_in                                  );
			dp5                              .s_out1                                 (mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::Y_N1]);
			dp5                              .s_out2                                 (mdm.sc[+mdm::tsk::demodulate_wg ].s_in [+mdm::sck::demodulate_wg ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate_wg ].s_out [+mdm::sck::demodulate_wg ::Y_N2](fnl                              .s_in1                                 );
		} else {
			mdm.sc[+mdm::tsk::modulate      ].s_out [+mdm::sck::modulate      ::X_N2](chn.sc[+chn::tsk::add_noise     ].s_in [+chn::sck::add_noise     ::X_N ]);
			chn.sc[+chn::tsk::add_noise     ].s_out [+chn::sck::add_noise     ::Y_N ](mdm.sc[+mdm::tsk::filter        ].s_in [+mdm::sck::filter        ::Y_N1]);
			mdm.sc[+mdm::tsk::filter        ].s_out [+mdm::sck::filter        ::Y_N2](qnt.sc[+qnt::tsk::process       ].s_in [+qnt::sck::process       ::Y_N1]);
			qnt.sc[+qnt::tsk::process       ].s_out [+qnt::sck::process       ::Y_N2](dp5                              .s_in                                  );
			dp5                              .s_out1                                 (mdm.sc[+mdm::tsk::tdemodulate   ].s_in [+mdm::sck::tdemodulate   ::Y_N1]);
			dp5                              .s_out2                                 (mdm.sc[+mdm::tsk::demodulate    ].s_in [+mdm::sck::demodulate    ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate    ].s_out [+mdm::sck::demodulate    ::Y_N2](fnl                              .s_in1                                 );
		}
		fnl                                  .s_out                                  (itl.sc[+itl::tsk::deinterleave  ].s_in [+itl::sck::deinterleave  ::itl ]);
		itl.sc    [+itl::tsk::deinterleave  ].s_out [+itl::sck::deinterleave  ::nat ](rtr                              .s_in                                  );
		rtr                                  .s_out1                                 (dcs.sc[+dec::tsk::decode_siso   ].s_in [+dec::sck::decode_siso   ::Y_N1]);
		rtr                                  .s_out2                                 (dch.sc[+dec::tsk::decode_siho   ].s_in [+dec::sck::decode_siho   ::Y_N ]);
		dcs.sc    [+dec::tsk::decode_siso   ].s_out [+dec::sck::decode_siso   ::Y_N2](itl.sc[+itl::tsk::interleave    ].s_in [+itl::sck::interleave    ::nat ]);
		if (this->params_BFER_ite.chn->type.find("RAYLEIGH") != std::string::npos) {
			itl.sc[+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::tdemodulate_wg].s_in [+mdm::sck::tdemodulate_wg::Y_N2]);
			mdm.sc[+mdm::tsk::tdemodulate_wg].s_out [+mdm::sck::tdemodulate_wg::Y_N3](fnl                              .s_in2                                 );
		} else {
			itl.sc[+itl::tsk::interleave    ].s_out [+itl::sck::interleave    ::itl ](mdm.sc[+mdm::tsk::tdemodulate   ].s_in [+mdm::sck::tdemodulate   ::Y_N2]);
			mdm.sc[+mdm::tsk::tdemodulate   ].s_out [+mdm::sck::tdemodulate   ::Y_N3](fnl                              .s_in2                                 );
		}
		dch.sc    [+dec::tsk::decode_siho   ].s_out [+dec::sck::decode_siho   ::V_K ](crc.sc[+crc::tsk::extract       ].s_in [+crc::sck::extract       ::V_K1]);
		crc.sc    [+crc::tsk::extract       ].s_out [+crc::sck::extract       ::V_K2](dp1                              .s_in                                  );
		dp1                                  .s_out1                                 (mnt.sc[+mnt::tsk::check_errors  ].s_in [+mnt::sck::check_errors  ::V   ]);
		dp1                                  .s_out2                                 (prd                              .s_in                                  );
	}
}

template <typename B, typename R, typename Q>
std::unique_ptr<module::Coset<B,Q>> SC_BFER_ite<B,R,Q>
::build_coset_real(const int tid)
{
	factory::Coset::parameters cst_params;
	cst_params.size     = this->params_BFER_ite.cdc->N_cw;
	cst_params.n_frames = this->params_BFER_ite.src->n_frames;

	this->coset_real_i.reset(cst_params.template build_real<B,Q>());
	this->coset_real_i->set_name("Coset_real_i");

	return std::unique_ptr<module::Coset<B,Q>>(cst_params.template build_real<B,Q>());
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef AFF3CT_MULTI_PREC
template class aff3ct::simulation::SC_BFER_ite<B_8,R_8,Q_8>;
template class aff3ct::simulation::SC_BFER_ite<B_16,R_16,Q_16>;
template class aff3ct::simulation::SC_BFER_ite<B_32,R_32,Q_32>;
template class aff3ct::simulation::SC_BFER_ite<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::SC_BFER_ite<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation

#endif
