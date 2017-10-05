#include <cstring>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_ite_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::BFER_ite_threads(const factory::BFER_ite::parameters &params)
: BFER_ite<B,R,Q>(params)
{
	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << tools::format_warning("Multi-threading detected with error tracking revert feature! "
			                                   "Each thread will play the same frames. Please run with one thread.")
			          << std::endl;
	}
}

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::~BFER_ite_threads()
{
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::__build_communication_chain(const int tid)
{
	BFER_ite<B,R,Q>::__build_communication_chain(tid);

	auto &source          = *this->source         [tid];
	auto &crc             = *this->crc            [tid];
	auto &encoder         = *this->codec          [tid]->get_encoder();
	auto &interleaver_bit = *this->interleaver_bit[tid];
	auto &modem           = *this->modem          [tid];

	if (this->params.src->type == "AZCW")
	{
		auto src_data = (uint8_t*)(source         ["generate"  ]["U_K" ].get_dataptr());
		auto crc_data = (uint8_t*)(crc            ["build"     ]["U_K2"].get_dataptr());
		auto enc_data = (uint8_t*)(encoder        ["encode"    ]["X_N" ].get_dataptr());
		auto itl_data = (uint8_t*)(interleaver_bit["interleave"]["itl" ].get_dataptr());

		auto src_bytes = source         ["generate"  ]["U_K" ].get_databytes();
		auto crc_bytes = crc            ["build"     ]["U_K2"].get_databytes();
		auto enc_bytes = encoder        ["encode"    ]["X_N" ].get_databytes();
		auto itl_bytes = interleaver_bit["interleave"]["itl" ].get_databytes();

		std::fill(src_data, src_data + src_bytes, 0);
		std::fill(crc_data, crc_data + crc_bytes, 0);
		std::fill(enc_data, enc_data + enc_bytes, 0);
		std::fill(itl_data, itl_data + itl_bytes, 0);

		modem["modulate"]["X_N1"].bind(interleaver_bit["interleave"]["itl"]);
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::_launch()
{
	BFER_ite<B,R,Q>::_launch();

	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_message.empty())
		throw std::runtime_error(this->prev_err_message);
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::start_thread(BFER_ite_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->simulation_loop(tid);
	}
	catch (std::exception const& e)
	{
		module::Monitor::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
			if (std::strlen(e.what()) > simu->prev_err_message.size())
				simu->prev_err_message = e.what();
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	auto &source          = *this->source         [tid];
	auto &crc             = *this->crc            [tid];
	auto &codec           = *this->codec          [tid];
	auto &interleaver_bit = *this->interleaver_bit[tid];
	auto &modem           = *this->modem          [tid];
	auto &channel         = *this->channel        [tid];
	auto &quantizer       = *this->quantizer      [tid];
	auto &interleaver_llr = *this->interleaver_llr[tid];
	auto &coset_real      = *this->coset_real     [tid];
	auto &coset_bit       = *this->coset_bit      [tid];
	auto &monitor         = *this->monitor        [tid];

	auto &encoder      = *codec.get_encoder();
	auto &decoder_siso = *codec.get_decoder_siso();
	auto &decoder_siho = *codec.get_decoder_siho();

	while ((!this->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time) &&
	        (this->monitor_red->get_n_analyzed_fra() < this->max_fra || this->max_fra == 0))
	{
		if (this->params.debug)
		{
			if (!monitor["check_errors"].get_n_calls())
				std::cout << "#" << std::endl;

			std::cout << "# -------------------------------" << std::endl;
			std::cout << "# New communication (n°" << monitor["check_errors"].get_n_calls() << ")" << std::endl;
			std::cout << "# -------------------------------" << std::endl;
			std::cout << "#" << std::endl;
		}

		if (this->params.src->type != "AZCW")
		{
			source         ["generate"  ].exec();
			crc            ["build"     ]["U_K1"].bind(source         ["generate"  ]["U_K" ]);
			encoder        ["encode"    ]["U_K" ].bind(crc            ["build"     ]["U_K2"]);
			interleaver_bit["interleave"]["nat" ].bind(encoder        ["encode"    ]["X_N" ]);
			modem          ["modulate"  ]["X_N1"].bind(interleaver_bit["interleave"]["itl" ]);
		}

		if (this->params.coset)
		{
			if (this->params.coded_monitoring) coset_bit["apply"]["ref"].bind(encoder["encode"]["X_N" ]);
			else                               coset_bit["apply"]["ref"].bind(crc    ["build" ]["U_K2"]);

			coset_real["apply"]["ref"].bind(encoder["encode"]["X_N"]);
		}

		if (this->params.coded_monitoring) monitor["check_errors"]["U"].bind(encoder["encode"  ]["X_N"]);
		else                               monitor["check_errors"]["U"].bind(source ["generate"]["U_K"]);

		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
		{
			channel  ["add_noise_wg" ]["X_N" ].bind(modem    ["modulate"     ]["X_N2"]);
			modem    ["demodulate_wg"]["H_N" ].bind(channel  ["add_noise_wg" ]["H_N" ]);
			modem    ["filter"       ]["Y_N1"].bind(channel  ["add_noise_wg" ]["Y_N" ]);
			quantizer["process"      ]["Y_N1"].bind(modem    ["filter"       ]["Y_N2"]);
			modem    ["demodulate_wg"]["Y_N1"].bind(quantizer["process"      ]["Y_N2"]);
		}
		else
		{
			channel  ["add_noise" ]["X_N" ].bind(modem    ["modulate" ]["X_N2"]);
			modem    ["filter"    ]["Y_N1"].bind(channel  ["add_noise"]["Y_N" ]);
			quantizer["process"   ]["Y_N1"].bind(modem    ["filter"   ]["Y_N2"]);
			modem    ["demodulate"]["Y_N1"].bind(quantizer["process"  ]["Y_N2"]);
		}

		interleaver_llr["deinterleave"]["itl"].bind(modem["demodulate"]["Y_N2"]);

		// ------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------ turbo demodulation loop
		// ------------------------------------------------------------------------------------------------------------
		for (auto ite = 1; ite <= this->params.n_ite; ite++)
		{
			// ------------------------------------------------------------------------------------------- CRC checking
			if (this->params.crc->type != "NO" && ite >= this->params.crc_start)
			{
				codec  ["extract_sys_bit"]["Y_N"].bind(interleaver_llr["deinterleave"   ]["nat"]);
				if (crc["check"          ]["V_K"].bind(codec          ["extract_sys_bit"]["V_K"]))
					break;
			}

			// ----------------------------------------------------------------------------------------------- decoding
			if (this->params.coset)
			{
				coset_real  ["apply"      ]["in_data"].bind(interleaver_llr["deinterleave"]["nat"     ]);
				decoder_siso["decode_siso"]["Y_N1"   ].bind(coset_real     ["apply"       ]["out_data"]);
				coset_real  ["apply"      ]["in_data"].bind(decoder_siso   ["decode_siso" ]["Y_N2"    ]);
			}
			else
			{
				decoder_siso["decode_siso"]["Y_N1"].bind(interleaver_llr["deinterleave"]["nat"]);
			}

			// ------------------------------------------------------------------------------------------- interleaving
			if (this->params.coset) interleaver_llr["interleave"]["nat"].bind(coset_real  ["apply"      ]["out_data"]);
			else                    interleaver_llr["interleave"]["nat"].bind(decoder_siso["decode_siso"]["Y_N2"    ]);

			// ------------------------------------------------------------------------------------------- demodulation
			if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
			{
				modem["tdemodulate_wg"]["Y_N1"].bind(quantizer      ["process"     ]["Y_N2"]);
				modem["tdemodulate_wg"]["H_N" ].bind(channel        ["add_noise_wg"]["H_N" ]);
				modem["tdemodulate_wg"]["Y_N2"].bind(interleaver_llr["interleave"  ]["itl" ]);
			}
			else
			{
				modem["tdemodulate"]["Y_N1"].bind(quantizer      ["process"   ]["Y_N2"]);
				modem["tdemodulate"]["Y_N2"].bind(interleaver_llr["interleave"]["itl" ]);
			}

			// ----------------------------------------------------------------------------------------- deinterleaving
			if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
				interleaver_llr["deinterleave"]["itl"].bind(modem["tdemodulate_wg"]["Y_N3"]);
			else
				interleaver_llr["deinterleave"]["itl"].bind(modem["tdemodulate"   ]["Y_N3"]);
		}

		if (this->params.coset)
		{
			coset_real["apply"]["in_data"].bind(interleaver_llr["deinterleave"]["nat"]);

			if (this->params.coded_monitoring)
			{
				decoder_siho["decode_siho_coded"]["Y_N"    ].bind(coset_real  ["apply"            ]["out_data"]);
				coset_bit   ["apply"            ]["in_data"].bind(decoder_siho["decode_siho_coded"]["V_N"     ]);
			}
			else
			{
				decoder_siho["decode_siho"]["Y_N"    ].bind(coset_real  ["apply"      ]["out_data"]);
				coset_bit   ["apply"      ]["in_data"].bind(decoder_siho["decode_siho"]["V_K"     ]);
				crc         ["extract"    ]["V_K1"   ].bind(coset_bit   ["apply"      ]["out_data"]);
			}
		}
		else
		{
			if (this->params.coded_monitoring)
			{
				decoder_siho["decode_siho_coded"]["Y_N"].bind(interleaver_llr["deinterleave"]["nat"]);
			}
			else
			{
				decoder_siho["decode_siho"]["Y_N" ].bind(interleaver_llr["deinterleave"]["nat"]);
				crc         ["extract"    ]["V_K1"].bind(decoder_siho   ["decode_siho" ]["V_K"]);
			}
		}

		if (this->params.coded_monitoring)
		{
			if (this->params.coset) monitor["check_errors"]["V"].bind(coset_bit   ["apply"            ]["out_data"]);
			else                    monitor["check_errors"]["V"].bind(decoder_siho["decode_siho_coded"]["V_N"     ]);
		}
		else
		{
			monitor["check_errors"]["V"].bind(crc["extract"]["V_K2"]);
		}
	}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::BFER_ite_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::BFER_ite_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::BFER_ite_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::BFER_ite_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::BFER_ite_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
