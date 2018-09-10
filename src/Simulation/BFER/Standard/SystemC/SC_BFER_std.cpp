#ifdef SYSTEMC

#include <iostream>

#include "Tools/Exception/exception.hpp"

#include "SC_BFER_std.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
SC_BFER_std<B,R,Q>
::SC_BFER_std(const factory::BFER_std::parameters &params_BFER_std)
: BFER_std<B,R,Q>(params_BFER_std),
  duplicator(3)
{
	if (this->params_BFER_std.n_threads > 1)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "BFER SystemC simulation does not support "
		                                                            "multi-threading.");

	if (params_BFER_std.coded_monitoring)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "BFER SystemC simulation does not support the coded "
		                                                            "monitoring.");

	if (params_BFER_std.mutinfo)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "BFER SystemC simulation does not support the mututal"
		                                                            "information computation.");
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

	this->monitor_er[tid]->add_handler_check([&]() -> void
	{
		if (!this->keep_looping_noise_point()) // will make the MPI communication
			sc_core::sc_stop();
	});
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::create_sc_modules()
{
	using namespace module;

	const auto tid = 0;

	// create the sc_module inside the objects of the communication chain
	this->source   [tid]                 ->sc.create_module(+src::tsk::generate  );
	this->crc      [tid]                 ->sc.create_module(+crc::tsk::build     );
	this->codec    [tid]->get_encoder()  ->sc.create_module(+enc::tsk::encode    );
	this->codec    [tid]->get_puncturer()->sc.create_module(+pct::tsk::puncture  );
	this->codec    [tid]->get_puncturer()->sc.create_module(+pct::tsk::depuncture);
	this->modem    [tid]                 ->sc.create_module(+mdm::tsk::modulate  );
	this->modem    [tid]                 ->sc.create_module(+mdm::tsk::filter    );
	if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos)
	{
		this->channel[tid]->sc.create_module(+chn::tsk::add_noise_wg );
		this->modem  [tid]->sc.create_module(+mdm::tsk::demodulate_wg);
	}
	else
	{
		this->channel[tid]->sc.create_module(+chn::tsk::add_noise );
		this->modem  [tid]->sc.create_module(+mdm::tsk::demodulate);
	}
	this->quantizer [tid]                    ->sc.create_module(+qnt::tsk::process        );
	this->codec     [tid]->get_decoder_siho()->sc.create_module(+dec::tsk::decode_siho    );
	this->monitor_er[tid]                    ->sc.create_module(+mnt::tsk::check_errors   );
	if (this->params_BFER_std.mutinfo) // this->monitor_mi[tid] != nullptr
	this->monitor_mi[tid]                    ->sc.create_module(+mnt::tsk::get_mutual_info);
	if (this->params_BFER_std.coset)
	{
		this->coset_real[tid]->sc.create_module(+cst::tsk::apply);
		this->coset_bit [tid]->sc.create_module(+cst::tsk::apply);
	}
	this->crc[tid]->sc.create_module(+crc::tsk::extract);

	this->duplicator[0] = std::make_shared<tools::SC_Duplicator>("Duplicator_src");
	if (this->params_BFER_std.coset)
	{
		this->duplicator[1] = std::make_shared<tools::SC_Duplicator>("Duplicator1");
		this->duplicator[2] = std::make_shared<tools::SC_Duplicator>("Duplicator2");
	}
}

template <typename B, typename R, typename Q>
void SC_BFER_std<B,R,Q>
::_launch()
{
	BFER_std<B,R,Q>::_launch();

	this->create_sc_modules();

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
void SC_BFER_std<B,R,Q>
::bind_sockets()
{
	using namespace module;

	auto &dp_src = *this->duplicator[0];

	auto &src = *this->source    [0];
	auto &crc = *this->crc       [0];
	auto &enc = *this->codec     [0]->get_encoder();
	auto &pct = *this->codec     [0]->get_puncturer();
	auto &mdm = *this->modem     [0];
	auto &chn = *this->channel   [0];
	auto &qnt = *this->quantizer [0];
	auto &csr = *this->coset_real[0];
	auto &dec = *this->codec     [0]->get_decoder_siho();
	auto &csb = *this->coset_bit [0];
	auto &mnt = *this->monitor_er[0];

	if (this->params_BFER_std.coset)
	{
		auto &dp1 = *this->duplicator[1];
		auto &dp2 = *this->duplicator[2];

		src.sc    [+src::tsk::generate     ].s_out [+src::sck::generate     ::U_K ](dp_src                          .s_in                                );
		dp_src                              .s_out1                                (mnt.sc[+mnt::tsk::check_errors ].s_in[+mnt::sck::check_errors ::U   ]);
		dp_src                              .s_out2                                (crc.sc[+crc::tsk::build        ].s_in[+crc::sck::build        ::U_K1]);
		crc.sc    [+crc::tsk::build        ].s_out [+crc::sck::build        ::U_K2](dp1                             .s_in                                );
		dp1                                 .s_out1                                (csb.sc[+cst::tsk::apply        ].s_in[+cst::sck::apply        ::ref ]);
		dp1                                 .s_out2                                (enc.sc[+enc::tsk::encode       ].s_in[+enc::sck::encode       ::U_K ]);
		enc.sc    [+enc::tsk::encode       ].s_out [+enc::sck::encode       ::X_N ](dp2                             .s_in                                );
		dp2                                 .s_out1                                (csr.sc[+cst::tsk::apply        ].s_in[+cst::sck::apply        ::ref ]);
		dp2                                 .s_out2                                (pct.sc[+pct::tsk::puncture     ].s_in[+pct::sck::puncture     ::X_N1]);
		pct.sc    [+pct::tsk::puncture     ].s_out [+pct::sck::puncture     ::X_N2](mdm.sc[+mdm::tsk::modulate     ].s_in[+mdm::sck::modulate     ::X_N1]);
		if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos) { // Rayleigh chn
			mdm.sc[+mdm::tsk::modulate     ].s_out [+mdm::sck::modulate     ::X_N2](chn.sc[+chn::tsk::add_noise_wg ].s_in[+chn::sck::add_noise_wg ::X_N ]);
			chn.sc[+chn::tsk::add_noise_wg ].s_out [+chn::sck::add_noise_wg ::H_N ](mdm.sc[+mdm::tsk::demodulate_wg].s_in[+mdm::sck::demodulate_wg::H_N ]);
			chn.sc[+chn::tsk::add_noise_wg ].s_out [+chn::sck::add_noise_wg ::Y_N ](mdm.sc[+mdm::tsk::filter       ].s_in[+mdm::sck::filter       ::Y_N1]);
			mdm.sc[+mdm::tsk::filter       ].s_out [+mdm::sck::filter       ::Y_N2](mdm.sc[+mdm::tsk::demodulate_wg].s_in[+mdm::sck::demodulate_wg::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate_wg].s_out [+mdm::sck::demodulate_wg::Y_N2](qnt.sc[+qnt::tsk::process      ].s_in[+qnt::sck::process      ::Y_N1]);
		} else { // additive channel (AWGN, USER, NO)
			mdm.sc[+mdm::tsk::modulate     ].s_out [+mdm::sck::modulate     ::X_N2](chn.sc[+chn::tsk::add_noise    ].s_in[+chn::sck::add_noise    ::X_N ]);
			chn.sc[+chn::tsk::add_noise    ].s_out [+chn::sck::add_noise    ::Y_N ](mdm.sc[+mdm::tsk::filter       ].s_in[+mdm::sck::filter       ::Y_N1]);
			mdm.sc[+mdm::tsk::filter       ].s_out [+mdm::sck::filter       ::Y_N2](mdm.sc[+mdm::tsk::demodulate   ].s_in[+mdm::sck::demodulate   ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate   ].s_out [+mdm::sck::demodulate   ::Y_N2](qnt.sc[+qnt::tsk::process      ].s_in[+qnt::sck::process      ::Y_N1]);
		}
		qnt.sc    [+qnt::tsk::process      ].s_out [+qnt::sck::process      ::Y_N2](pct.sc[+pct::tsk::depuncture   ].s_in[+pct::sck::depuncture   ::Y_N1]);
		pct.sc    [+pct::tsk::depuncture   ].s_out [+pct::sck::depuncture   ::Y_N2](csr.sc[+cst::tsk::apply        ].s_in[+cst::sck::apply        ::in  ]);
		csr.sc    [+cst::tsk::apply        ].s_out [+cst::sck::apply        ::out ](dec.sc[+dec::tsk::decode_siho  ].s_in[+dec::sck::decode_siho  ::Y_N ]);
		dec.sc    [+dec::tsk::decode_siho  ].s_out [+dec::sck::decode_siho  ::V_K ](csb.sc[+cst::tsk::apply        ].s_in[+cst::sck::apply        ::in  ]);
		csb.sc    [+cst::tsk::apply        ].s_out [+cst::sck::apply        ::out ](crc.sc[+crc::tsk::extract      ].s_in[+crc::sck::extract      ::V_K1]);
		crc.sc    [+crc::tsk::extract      ].s_out [+crc::sck::extract      ::V_K2](mnt.sc[+mnt::tsk::check_errors ].s_in[+mnt::sck::check_errors ::V   ]);
	}
	else // standard simulation
	{
		src.sc    [+src::tsk::generate     ].s_out [+src::sck::generate     ::U_K ](dp_src                          .s_in                                );
		dp_src                              .s_out1                                (mnt.sc[+mnt::tsk::check_errors ].s_in[+mnt::sck::check_errors ::U   ]);
		dp_src                              .s_out2                                (crc.sc[+crc::tsk::build        ].s_in[+crc::sck::build        ::U_K1]);
		crc.sc    [+crc::tsk::build        ].s_out [+crc::sck::build        ::U_K2](enc.sc[+enc::tsk::encode       ].s_in[+enc::sck::encode       ::U_K ]);
		enc.sc    [+enc::tsk::encode       ].s_out [+enc::sck::encode       ::X_N ](pct.sc[+pct::tsk::puncture     ].s_in[+pct::sck::puncture     ::X_N1]);
		pct.sc    [+pct::tsk::puncture     ].s_out [+pct::sck::puncture     ::X_N2](mdm.sc[+mdm::tsk::modulate     ].s_in[+mdm::sck::modulate     ::X_N1]);
		if (this->params_BFER_std.chn->type.find("RAYLEIGH") != std::string::npos) { // Rayleigh chn
			mdm.sc[+mdm::tsk::modulate     ].s_out [+mdm::sck::modulate     ::X_N2](chn.sc[+chn::tsk::add_noise_wg ].s_in[+chn::sck::add_noise_wg ::X_N ]);
			chn.sc[+chn::tsk::add_noise_wg ].s_out [+chn::sck::add_noise_wg ::H_N ](mdm.sc[+mdm::tsk::demodulate_wg].s_in[+mdm::sck::demodulate_wg::H_N ]);
			chn.sc[+chn::tsk::add_noise_wg ].s_out [+chn::sck::add_noise_wg ::Y_N ](mdm.sc[+mdm::tsk::filter       ].s_in[+mdm::sck::filter       ::Y_N1]);
			mdm.sc[+mdm::tsk::filter       ].s_out [+mdm::sck::filter       ::Y_N2](mdm.sc[+mdm::tsk::demodulate_wg].s_in[+mdm::sck::demodulate_wg::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate_wg].s_out [+mdm::sck::demodulate_wg::Y_N2](qnt.sc[+qnt::tsk::process      ].s_in[+qnt::sck::process      ::Y_N1]);
		} else { // additive channel (AWGN, USER, NO)
			mdm.sc[+mdm::tsk::modulate     ].s_out [+mdm::sck::modulate     ::X_N2](chn.sc[+chn::tsk::add_noise    ].s_in[+chn::sck::add_noise    ::X_N ]);
			chn.sc[+chn::tsk::add_noise    ].s_out [+chn::sck::add_noise    ::Y_N ](mdm.sc[+mdm::tsk::filter       ].s_in[+mdm::sck::filter       ::Y_N1]);
			mdm.sc[+mdm::tsk::filter       ].s_out [+mdm::sck::filter       ::Y_N2](mdm.sc[+mdm::tsk::demodulate   ].s_in[+mdm::sck::demodulate   ::Y_N1]);
			mdm.sc[+mdm::tsk::demodulate   ].s_out [+mdm::sck::demodulate   ::Y_N2](qnt.sc[+qnt::tsk::process      ].s_in[+qnt::sck::process      ::Y_N1]);
		}
		qnt.sc    [+qnt::tsk::process      ].s_out [+qnt::sck::process      ::Y_N2](pct.sc[+pct::tsk::depuncture   ].s_in[+pct::sck::depuncture   ::Y_N1]);
		pct.sc    [+pct::tsk::depuncture   ].s_out [+pct::sck::depuncture   ::Y_N2](dec.sc[+dec::tsk::decode_siho  ].s_in[+dec::sck::decode_siho  ::Y_N ]);
		dec.sc    [+dec::tsk::decode_siho  ].s_out [+dec::sck::decode_siho  ::V_K ](crc.sc[+crc::tsk::extract      ].s_in[+crc::sck::extract      ::V_K1]);
		crc.sc    [+crc::tsk::extract      ].s_out [+crc::sck::extract      ::V_K2](mnt.sc[+mnt::tsk::check_errors ].s_in[+mnt::sck::check_errors ::V   ]);
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
