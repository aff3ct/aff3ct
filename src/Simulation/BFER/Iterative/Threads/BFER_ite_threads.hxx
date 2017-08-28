#include <cstring>
#include <string>
#include <vector>
#include <chrono>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"
#include "Tools/Display/bash_tools.h"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_ite_threads.hpp"

namespace aff3ct
{
namespace simulation
{
template <class C, typename B, typename R, typename Q, int CRC>
BFER_ite_threads<C,B,R,Q,CRC>
::BFER_ite_threads(const factory::BFER_ite::parameters<C> &params)
: BFER_ite<C,B,R,Q,CRC>(params)
{
	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << tools::format_warning("Multi-threading detected with error tracking revert feature! "
			                                   "Each thread will play the same frames. Please run with one thread.")
			          << std::endl;
	}
}

template <class C, typename B, typename R, typename Q, int CRC>
BFER_ite_threads<C,B,R,Q,CRC>
::~BFER_ite_threads()
{
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite_threads<C,B,R,Q,CRC>
::_build_communication_chain(const int tid)
{
	BFER_ite<C,B,R,Q,CRC>::_build_communication_chain(tid);

	auto &source          = *this->source         [tid];
	auto &crc             = *this->crc            [tid];
	auto &encoder         = *this->codec          [tid]->get_encoder();
	auto &interleaver_bit = *this->interleaver_bit[tid];
	auto &modem           = *this->modem          [tid];

	if (this->params.src.type == "AZCW")
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

		interleaver_bit["interleave"]["itl"].bind(modem["modulate"]["X_N1"]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src.type != "AZCW")
		{
			auto src_data = (B*)(source["generate"]["U_K"].get_dataptr());
			auto src_size = source["generate"]["U_K"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(src_data, src_size, "src", false, {});
		}

		if (this->params.coset)
		{
			auto enc_data = (B*)(encoder["encode"]["X_N"].get_dataptr());
			auto enc_size = encoder["encode"]["X_N"].get_databytes() / sizeof(B);
			this->dumper[tid]->register_data(enc_data, enc_size, "enc", false, {(unsigned)this->params.cdc.enc.K});
		}
	}
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite_threads<C,B,R,Q,CRC>
::_launch()
{
	BFER_ite<C,B,R,Q,CRC>::_launch();

	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_ite_threads<C,B,R,Q,CRC>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<C,B,R,Q,CRC>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();

	if (!this->prev_err_message.empty())
		throw std::runtime_error(this->prev_err_message);
}

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite_threads<C,B,R,Q,CRC>
::start_thread(BFER_ite_threads<C,B,R,Q,CRC> *simu, const int tid)
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

template <class C, typename B, typename R, typename Q, int CRC>
void BFER_ite_threads<C,B,R,Q,CRC>
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

		if (this->params.src.type != "AZCW")
		{
			source         ["generate"  ].exec();
			source         ["generate"  ]["U_K" ].bind(crc            ["build"     ]["U_K1"]);
			crc            ["build"     ]["U_K2"].bind(encoder        ["encode"    ]["U_K" ]);
			encoder        ["encode"    ]["X_N" ].bind(interleaver_bit["interleave"]["nat" ]);
			interleaver_bit["interleave"]["itl" ].bind(modem          ["modulate"  ]["X_N1"]);
		}

		if (this->params.coset)
		{
			if (this->params.coded_monitoring) encoder["encode"]["X_N" ].bind(coset_bit["apply"]["ref"]);
			else                               crc    ["build" ]["U_K2"].bind(coset_bit["apply"]["ref"]);

			encoder["encode"]["X_N"].bind(coset_real["apply"]["ref"]);
		}

		if (this->params.coded_monitoring) encoder["encode"  ]["X_N"].bind(monitor["check_errors"]["U"]);
		else                               source ["generate"]["U_K"].bind(monitor["check_errors"]["U"]);

		if (this->params.chn.type.find("RAYLEIGH") != std::string::npos)
		{
			modem    ["modulate"     ]["X_N2"].bind(channel  ["add_noise_wg" ]["X_N" ]);
			channel  ["add_noise_wg" ]["Y_N" ].bind(modem    ["filter"       ]["Y_N1"]);
			modem    ["filter"       ]["Y_N2"].bind(quantizer["process"      ]["Y_N1"]);
			quantizer["process"      ]["Y_N2"].bind(modem    ["demodulate_wg"]["Y_N1"]);
			channel  ["add_noise_wg" ]["H_N" ].bind(modem    ["demodulate_wg"]["H_N" ]);
		}
		else
		{
			modem    ["modulate" ]["X_N2"].bind(channel  ["add_noise" ]["X_N" ]);
			channel  ["add_noise"]["Y_N" ].bind(modem    ["filter"    ]["Y_N1"]);
			modem    ["filter"   ]["Y_N2"].bind(quantizer["process"   ]["Y_N1"]);
			quantizer["process"  ]["Y_N2"].bind(modem    ["demodulate"]["Y_N1"]);
		}

		modem["demodulate"]["Y_N2"].bind(interleaver_llr["deinterleave"]["itl"]);

		// ------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------ turbo demodulation loop
		// ------------------------------------------------------------------------------------------------------------
		for (auto ite = 1; ite <= this->params.n_ite; ite++)
		{
			// ------------------------------------------------------------------------------------------- CRC checking
			if (this->params.crc.type != "NO" && ite >= this->params.crc_start)
			{
				interleaver_llr["deinterleave"   ]["nat"].bind(codec["extract_sys_bit"]["Y_N"]);
				if (codec      ["extract_sys_bit"]["V_K"].bind(crc  ["check"          ]["V_K"]))
					break;
			}

			// ----------------------------------------------------------------------------------------------- decoding
			if (this->params.coset)
			{
				interleaver_llr["deinterleave"]["nat"     ].bind(coset_real  ["apply"      ]["in_data"]);
				coset_real     ["apply"       ]["out_data"].bind(decoder_siso["decode_siso"]["Y_N1"   ]);
				decoder_siso   ["decode_siso" ]["Y_N2"    ].bind(coset_real  ["apply"      ]["in_data"]);
			}
			else
			{
				interleaver_llr["deinterleave"]["nat"].bind(decoder_siso["decode_siso"]["Y_N1"]);
			}

			// ------------------------------------------------------------------------------------------- interleaving
			if (this->params.coset) coset_real  ["apply"      ]["out_data"].bind(interleaver_llr["interleave"]["nat"]);
			else                    decoder_siso["decode_siso"]["Y_N2"    ].bind(interleaver_llr["interleave"]["nat"]);

			// ------------------------------------------------------------------------------------------- demodulation
			if (this->params.chn.type.find("RAYLEIGH") != std::string::npos)
			{
				quantizer      ["process"     ]["Y_N2"].bind(modem["tdemodulate_wg"]["Y_N1"]);
				channel        ["add_noise_wg"]["H_N" ].bind(modem["tdemodulate_wg"]["H_N" ]);
				interleaver_llr["interleave"  ]["itl" ].bind(modem["tdemodulate_wg"]["Y_N2"]);
			}
			else
			{
				quantizer      ["process"   ]["Y_N2"].bind(modem["tdemodulate"]["Y_N1"]);
				interleaver_llr["interleave"]["itl" ].bind(modem["tdemodulate"]["Y_N2"]);
			}

			// ----------------------------------------------------------------------------------------- deinterleaving
			if (this->params.chn.type.find("RAYLEIGH") != std::string::npos)
				modem["tdemodulate_wg"]["Y_N3"].bind(interleaver_llr["deinterleave"]["itl"]);
			else
				modem["tdemodulate"   ]["Y_N3"].bind(interleaver_llr["deinterleave"]["itl"]);
		}

		if (this->params.coset)
		{
			interleaver_llr["deinterleave"]["nat"].bind(coset_real["apply"]["in_data"]);

			if (this->params.coded_monitoring)
			{
				coset_real  ["apply"            ]["out_data"].bind(decoder_siho["decode_siho_coded"]["Y_N"    ]);
				decoder_siho["decode_siho_coded"]["V_N"     ].bind(coset_bit   ["apply"            ]["in_data"]);
			}
			else
			{
				coset_real  ["apply"      ]["out_data"].bind(decoder_siho["decode_siho"]["Y_N"    ]);
				decoder_siho["decode_siho"]["V_K"     ].bind(coset_bit   ["apply"      ]["in_data"]);
				coset_bit   ["apply"      ]["out_data"].bind(crc         ["extract"    ]["V_K1"   ]);
			}
		}
		else
		{
			if (this->params.coded_monitoring)
			{
				interleaver_llr["deinterleave"]["nat"].bind(decoder_siho["decode_siho_coded"]["Y_N"]);
			}
			else
			{
				interleaver_llr["deinterleave"]["nat"].bind(decoder_siho["decode_siho"]["Y_N" ]);
				decoder_siho   ["decode_siho" ]["V_K"].bind(crc         ["extract"    ]["V_K1"]);
			}
		}

		if (this->params.coded_monitoring)
		{
			if (this->params.coset) coset_bit   ["apply"            ]["out_data"].bind(monitor["check_errors"]["V"]);
			else                    decoder_siho["decode_siho_coded"]["V_N"     ].bind(monitor["check_errors"]["V"]);
		}
		else
		{
			crc["extract"]["V_K2"].bind(monitor["check_errors"]["V"]);
		}
	}
}
}
}
