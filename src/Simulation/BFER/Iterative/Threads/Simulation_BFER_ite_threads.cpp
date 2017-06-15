#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Simulation_BFER_ite_threads.hpp"

using namespace aff3ct::module;
using namespace aff3ct::tools;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
Simulation_BFER_ite_threads<B,R,Q>
::Simulation_BFER_ite_threads(const parameters& params, Codec_SISO<B,Q> &codec)
: Simulation_BFER_ite<B,R,Q>(params,codec),

  U_K1(this->params.simulation.n_threads, mipp::vector<B>(params.code.K_info * params.simulation.inter_frame_level)),
  U_K2(this->params.simulation.n_threads, mipp::vector<B>(params.code.K      * params.simulation.inter_frame_level)),
  X_N1(this->params.simulation.n_threads, mipp::vector<B>(params.code.N      * params.simulation.inter_frame_level)),
  X_N2(this->params.simulation.n_threads, mipp::vector<B>(params.code.N      * params.simulation.inter_frame_level)),
  X_N3(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  H_N (this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  Y_N1(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_mod  * params.simulation.inter_frame_level)),
  Y_N2(this->params.simulation.n_threads, mipp::vector<R>(params.code.N_fil  * params.simulation.inter_frame_level)),
  Y_N3(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N_fil  * params.simulation.inter_frame_level)),
  Y_N4(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N      * params.simulation.inter_frame_level)),
  Y_N5(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N      * params.simulation.inter_frame_level)),
  Y_N6(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N      * params.simulation.inter_frame_level)),
  Y_N7(this->params.simulation.n_threads, mipp::vector<Q>(params.code.N      * params.simulation.inter_frame_level)),
  V_K1(this->params.simulation.n_threads, mipp::vector<B>(params.code.K      * params.simulation.inter_frame_level)),
  V_K2(this->params.simulation.n_threads, mipp::vector<B>(params.code.K_info * params.simulation.inter_frame_level))
{
	if (params.simulation.n_threads > 1 && params.simulation.debug)
		std::clog << bold_yellow("(WW) Debug mode will be disabled ")
		          << bold_yellow("because you launched the simulation with more than 1 thread!")
		          << std::endl;

	if (params.simulation.benchs)
		throw std::invalid_argument("aff3ct::simulation::Simulation_BFER_ite_threads: the bench mode is not "
		                            "supported.");

#ifdef ENABLE_MPI
	if (params.simulation.debug || params.simulation.benchs)
		throw std::invalid_argument("aff3ct::simulation::Simulation_BFER_ite_threads: debug and bench modes are "
		                            "unavailable in MPI.");
#endif

	if (this->params.monitor.err_track_revert)
	{
		if (this->params.simulation.n_threads != 1)
			std::clog << bold_yellow("(WW) Multi-threading detected with error tracking revert feature!")
			          << bold_yellow(" Each thread will play the same frames. Please run one thread.")
			          << std::endl;
	}
}

template <typename B, typename R, typename Q>
Simulation_BFER_ite_threads<B,R,Q>
::~Simulation_BFER_ite_threads()
{
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::_build_communication_chain(const int tid)
{
	Simulation_BFER_ite<B,R,Q>::_build_communication_chain(tid);

	if (this->params.source.type == "AZCW")
	{
		std::fill(this->U_K1[tid].begin(), this->U_K1[tid].end(), (B)0);
		std::fill(this->U_K2[tid].begin(), this->U_K2[tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	if (this->params.monitor.err_track_enable)
	{
		this->dumper[tid]->register_data(U_K1[tid], "src", false, {});
		this->dumper[tid]->register_data(X_N1[tid], "enc", false, {(unsigned)this->params.code.K});
		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});
		if (this->interleaver[tid]->is_uniform())
			this->dumper[tid]->register_data(this->interleaver[tid]->get_lut(), "itl", false, {});
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::_launch()
{
	std::vector<std::thread> threads(this->params.simulation.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1] = std::thread(Simulation_BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	Simulation_BFER_ite_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.simulation.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::start_thread(Simulation_BFER_ite_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->Monte_Carlo_method(tid);
	}
	catch (std::exception const& e)
	{
		Monitor<B>::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
		{
			std::cerr << bold_red("(EE) ") << bold_red("An issue was encountered during the simulation loop (tid = ")
			          << bold_red(std::to_string(tid) + ").") << std::endl
			          << bold_red("(EE) ") << bold_red(e.what()) << std::endl;
			simu->prev_err_message = e.what();
		}
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::Monte_Carlo_method(const int tid)
{
	if (this->params.simulation.n_threads == 1 && this->params.simulation.debug)
		this->simulation_loop_debug();
	else
		this->simulation_loop(tid);
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	// simulation loop
	while ((!this->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (this->params.simulation.stop_time == seconds(0) ||
	         (steady_clock::now() - t_snr) < this->params.simulation.stop_time))
	{
		if (this->params.source.type != "AZCW")
		{
			// generate a random K bits vector U_K1
			auto t_sourc = steady_clock::now();
			this->source[tid]->generate(this->U_K1[tid]);
			this->durations[tid][std::make_pair(0, "Source")] += steady_clock::now() - t_sourc;

			// build the CRC from U_K1 into U_K2
			auto t_crcbd = steady_clock::now();
			this->crc[tid]->build(this->U_K1[tid], this->U_K2[tid]);
			this->durations[tid][std::make_pair(1, "CRC build")] += steady_clock::now() - t_crcbd;

			// encode U_K2 into a N bits vector X_N
			auto t_encod = steady_clock::now();
			this->encoder[tid]->encode(this->U_K2[tid], this->X_N1[tid]);
			this->durations[tid][std::make_pair(2, "Encoder")] += steady_clock::now() - t_encod;

			auto t_inter = steady_clock::now();
			this->interleaver[tid]->interleave(this->X_N1[tid], this->X_N2[tid]);
			this->durations[tid][std::make_pair(3, "Interleaver")] += steady_clock::now() - t_inter;

			// modulate
			auto t_modul = steady_clock::now();
			this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
			this->durations[tid][std::make_pair(4, "Modulator")] += steady_clock::now() - t_modul;
		}

		// Rayleigh channel
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid], this->H_N[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			auto t_chann = steady_clock::now();
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid]);
			this->durations[tid][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;
		}

		// filtering
		auto t_filte = steady_clock::now();
		this->modem[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
		this->durations[tid][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

		// make the quantization
		auto t_quant = steady_clock::now();
		this->quantizer[tid]->process(this->Y_N2[tid], this->Y_N3[tid]);
		this->durations[tid][std::make_pair(7, "Quantizer")] += steady_clock::now() - t_quant;

		std::fill(this->Y_N7[tid].begin(), this->Y_N7[tid].end(), (Q)0);
		for (auto ite = 0; ite <= this->params.simulation.n_ite; ite++)
		{
			// Rayleigh channel
			if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
			{
				// demodulation
				auto t_demod = steady_clock::now();
				this->modem[tid]->demodulate_with_gains(this->Y_N3[tid], this->H_N[tid], this->Y_N7[tid],
				                                        this->Y_N4[tid]);
				this->durations[tid][std::make_pair(8, "Demodulator")] += steady_clock::now() - t_demod;
			}
			else // additive channel (AWGN, USER, NO)
			{
				// demodulation
				auto t_demod = steady_clock::now();
				this->modem[tid]->demodulate(this->Y_N3[tid], this->Y_N7[tid], this->Y_N4[tid]);
				this->durations[tid][std::make_pair(8, "Demodulator")] += steady_clock::now() - t_demod;
			}

			// deinterleaving
			auto t_deint = steady_clock::now();
			this->interleaver[tid]->deinterleave(this->Y_N4[tid], this->Y_N5[tid]);
			this->durations[tid][std::make_pair(9, "Deinterlever")] += steady_clock::now() - t_deint;

			// apply the coset: the decoder will believe to a AZCW
			if (this->params.code.coset)
			{
				auto t_corea = steady_clock::now();
				this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N5[tid], this->Y_N5[tid]);
				this->durations[tid][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;
			}

			// soft decode
			if (ite != this->params.simulation.n_ite)
			{
				// decode
				auto t_decod = steady_clock::now();
				this->siso[tid]->soft_decode(this->Y_N5[tid], this->Y_N6[tid]);
				this->durations[tid][std::make_pair(11, "Decoder")] += steady_clock::now() - t_decod;

				// apply the coset to recover the extrinsic information
				if (this->params.code.coset)
				{
					auto t_corea = steady_clock::now();
					this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N6[tid], this->Y_N6[tid]);
					this->durations[tid][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;
				}

				// interleaving
				auto t_inter = steady_clock::now();
				this->interleaver[tid]->interleave(this->Y_N6[tid], this->Y_N7[tid]);
				this->durations[tid][std::make_pair(3, "Interleaver")] += steady_clock::now() - t_inter;
			}
			// hard decode
			else
			{
				// decode
				auto t_decod = steady_clock::now();
				this->decoder[tid]->hard_decode(this->Y_N5[tid], this->V_K1[tid]);
				this->durations[tid][std::make_pair(11, "Decoder")] += steady_clock::now() - t_decod;
			}
		}

		// apply the coset to recover the real bits
		if (this->params.code.coset)
		{
			auto t_cobit = steady_clock::now();
			this->coset_bit[tid]->apply(this->U_K2[tid], this->V_K1[tid], this->V_K1[tid]);
			this->durations[tid][std::make_pair(12, "Coset bit")] += steady_clock::now() - t_cobit;
		}

		// extract the CRC bits and keep only the information bits
		auto t_crcex = steady_clock::now();
		this->crc[tid]->extract(this->V_K1[tid], this->V_K2[tid]);
		this->durations[tid][std::make_pair(13, "CRC extract")] += steady_clock::now() - t_crcex;

		// check errors in the frame
		auto t_check = steady_clock::now();
		this->monitor[tid]->check_errors(this->U_K1[tid], this->V_K2[tid]);
		this->durations[tid][std::make_pair(14, "Check errors")] += steady_clock::now() - t_check;
	}
}

template <typename B, typename R, typename Q>
void Simulation_BFER_ite_threads<B,R,Q>
::simulation_loop_debug()
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	Frame_trace<B> ft(this->params.simulation.debug_limit, this->params.simulation.debug_precision); // frame trace to display the vectors

	// simulation loop
	while (!this->monitor_red->fe_limit_achieved() && // while max frame error count has not been reached
	       (this->params.simulation.stop_time == seconds(0) ||
	        (steady_clock::now() - t_snr) < this->params.simulation.stop_time))
	{
		std::cout << "-------------------------------" << std::endl;
		std::cout << "New encoding/decoding session !" << std::endl;
		std::cout << "Frame n°" << this->monitor_red->get_n_analyzed_fra() << std::endl;
		std::cout << "-------------------------------" << std::endl;

		if (this->params.source.type != "AZCW")
		{
			// generate a random K bits vector U_K1
			std::cout << "Generate random bits U_K1..." << std::endl;
			auto t_sourc = steady_clock::now();
			this->source[0]->generate(this->U_K1[0]);
			this->durations[0][std::make_pair(0, "Source")] += steady_clock::now() - t_sourc;

			// display U_K1
			std::cout << "U_K1:" << std::endl;
			ft.display_bit_vector(this->U_K1[0]);
			std::cout << std::endl;

			// add the CRC to U_K
			std::cout << "Build the CRC from U_K1 into U_K2..." << std::endl;
			auto t_crcbd = steady_clock::now();
			this->crc[0]->build(this->U_K1[0], this->U_K2[0]);
			this->durations[0][std::make_pair(1, "CRC build")] += steady_clock::now() - t_crcbd;

			// display U_K2
			std::cout << "U_K2:" << std::endl;
			ft.display_bit_vector(this->U_K2[0]);
			std::cout << std::endl;

			// encode U_K2 into a N bits vector X_N1
			std::cout << "Encode U_K2 in X_N1..." << std::endl;
			auto t_encod = steady_clock::now();
			this->encoder[0]->encode(this->U_K2[0], this->X_N1[0]);
			this->durations[0][std::make_pair(2, "Encoder")] += steady_clock::now() - t_encod;

			// display X_N1
			std::cout << "X_N1:" << std::endl;
			ft.display_bit_vector(this->X_N1[0]);
			std::cout << std::endl;

			// puncture X_N1 into X_N2
			std::cout << "Interleaver X_N1 in X_N2..." << std::endl;
			auto t_inter = steady_clock::now();
			this->interleaver[0]->interleave(this->X_N1[0], this->X_N2[0]);
			this->durations[0][std::make_pair(3, "Interleaver")] += steady_clock::now() - t_inter;

			// display X_N2
			std::cout << "X_N2:" << std::endl;
			ft.display_bit_vector(this->X_N2[0]);
			std::cout << std::endl;

			// modulate
			std::cout << "Modulate X_N2 in X_N3..." << std::endl;
			auto t_modul = steady_clock::now();
			this->modem[0]->modulate(this->X_N2[0], this->X_N3[0]);
			this->durations[0][std::make_pair(4, "Modulator")] += steady_clock::now() - t_modul;

			// display X_N3
			std::cout << "X_N3:" << std::endl;
			ft.display_real_vector(this->X_N3[0]);
			std::cout << std::endl;
		}
		else
		{
			// display U_K1
			std::cout << "U_K1:" << std::endl;
			ft.display_bit_vector(this->U_K1[0]);
			std::cout << std::endl;

			// display U_K2
			std::cout << "U_K2:" << std::endl;
			ft.display_bit_vector(this->U_K2[0]);
			std::cout << std::endl;

			// display X_N2
			std::cout << "X_N2:" << std::endl;
			ft.display_bit_vector(this->X_N2[0]);
			std::cout << std::endl;

			// display X_N3
			std::cout << "X_N3:" << std::endl;
			ft.display_real_vector(this->X_N3[0]);
			std::cout << std::endl;
		}

		// Rayleigh channel
		if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
		{
			// add noise
			std::cout << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			this->channel[0]->add_noise(this->X_N3[0], this->Y_N1[0], this->H_N[0]);
			this->durations[0][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// display Y_N1
			std::cout << "Y_N1:" << std::endl;
			ft.display_real_vector(this->Y_N1[0]);
			std::cout << std::endl;

			// display channel gains
			std::cout << "H_N:" << std::endl;
			ft.display_real_vector(this->H_N[0]);
			std::cout << std::endl;
		}
		else // additive channel (AWGN, USER, NO)
		{
			// add noise
			std::cout << "Add noise from X_N3 to Y_N1..." << std::endl;
			auto t_chann = steady_clock::now();
			this->channel[0]->add_noise(this->X_N3[0], this->Y_N1[0]);
			this->durations[0][std::make_pair(5, "Channel")] += steady_clock::now() - t_chann;

			// display Y_N1
			std::cout << "Y_N1:" << std::endl;
			ft.display_real_vector(this->Y_N1[0]);
			std::cout << std::endl;
		}

		// filtering
		std::cout << "Apply the filtering from Y_N1 to Y_N2..." << std::endl;
		auto t_filte = steady_clock::now();
		this->modem[0]->filter(this->Y_N1[0], this->Y_N2[0]);
		this->durations[0][std::make_pair(6, "Filter")] += steady_clock::now() - t_filte;

		// display Y_N2
		std::cout << "Y_N2:" << std::endl;
		ft.display_real_vector(this->Y_N2[0]);
		std::cout << std::endl;

		// make the quantization
		std::cout << "Make the quantization from Y_N2 to Y_N3..." << std::endl;
		auto t_quant = steady_clock::now();
		this->quantizer[0]->process(this->Y_N2[0], this->Y_N3[0]);
		this->durations[0][std::make_pair(7, "Quantizer")] += steady_clock::now() - t_quant;

		// display Y_N3
		std::cout << "Y_N3:" << std::endl;
		ft.display_real_vector(this->Y_N3[0]);
		std::cout << std::endl;

		std::fill(this->Y_N7[0].begin(), this->Y_N7[0].end(), (Q)0);
		for (auto ite = 0; ite <= this->params.simulation.n_ite; ite++)
		{
			std::cout << "*** Turbo demodulation iteration n°" << ite << " ***" << std::endl << std::endl;

			// Rayleigh channel
			if (this->params.channel.type.find("RAYLEIGH") != std::string::npos)
			{
				// demodulation
				std::cout << "Demodulate from Y_N3 to Y_N4..." << std::endl;
				auto t_demod = steady_clock::now();
				this->modem[0]->demodulate_with_gains(this->Y_N3[0], this->H_N[0], this->Y_N7[0], this->Y_N4[0]);
				this->durations[0][std::make_pair(8, "Demodulator")] += steady_clock::now() - t_demod;

				// display Y_N5
				std::cout << "Y_N4:" << std::endl;
				ft.display_real_vector(this->Y_N4[0]);
				std::cout << std::endl;
			}
			else
			{
				// demodulation
				std::cout << "Demodulate from Y_N3 to Y_N4..." << std::endl;
				auto t_demod = steady_clock::now();
				this->modem[0]->demodulate(this->Y_N3[0], this->Y_N7[0], this->Y_N4[0]);
				this->durations[0][std::make_pair(8, "Demodulator")] += steady_clock::now() - t_demod;

				// display Y_N5
				std::cout << "Y_N4:" << std::endl;
				ft.display_real_vector(this->Y_N4[0]);
				std::cout << std::endl;
			}

			// deinterleaving
			std::cout << "Deinterleave from Y_N4 to Y_N5..." << std::endl;
			auto t_deint = steady_clock::now();
			this->interleaver[0]->deinterleave(this->Y_N4[0], this->Y_N5[0]);
			this->durations[0][std::make_pair(9, "Deinterlever")] += steady_clock::now() - t_deint;

			// display Y_N5
			std::cout << "Y_N5:" << std::endl;
			ft.display_real_vector(this->Y_N5[0]);
			std::cout << std::endl;

			// apply the coset: the decoder will believe to a AZCW
			if (this->params.code.coset)
			{
				std::cout << "Apply the coset approach on Y_N5..." << std::endl;
				auto t_corea = steady_clock::now();
				this->coset_real[0]->apply(this->X_N1[0], this->Y_N5[0], this->Y_N5[0]);
				this->durations[0][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;

				// display Y_N5
				std::cout << "Y_N5:" << std::endl;
				ft.display_real_vector(this->Y_N5[0]);
				std::cout << std::endl;
			}

			// soft decode
			if (ite != this->params.simulation.n_ite)
			{
				// decode
				std::cout << "Soft decode from Y_N5 to Y_N6..." << std::endl;
				auto t_decod = steady_clock::now();
				this->siso[0]->soft_decode(this->Y_N5[0], this->Y_N6[0]);
				this->durations[0][std::make_pair(11, "Decoder")] += steady_clock::now() - t_decod;

				// display Y_N6
				std::cout << "Y_N6:" << std::endl;
				ft.display_real_vector(this->Y_N6[0]);
				std::cout << std::endl;

				// apply the coset to recover the extrinsic information
				if (this->params.code.coset)
				{
					std::cout << "Reverse the coset approach on Y_N6..." << std::endl;
					auto t_corea = steady_clock::now();
					this->coset_real[0]->apply(this->X_N1[0], this->Y_N6[0], this->Y_N6[0]);
					this->durations[0][std::make_pair(10, "Coset real")] += steady_clock::now() - t_corea;

					// display Y_N6
					std::cout << "Y_N6:" << std::endl;
					ft.display_real_vector(this->Y_N6[0]);
					std::cout << std::endl;
				}

				// interleaving
				std::cout << "Interleave from Y_N6 to Y_N7..." << std::endl;
				auto t_inter = steady_clock::now();
				this->interleaver[0]->interleave(this->Y_N6[0], this->Y_N7[0]);
				this->durations[0][std::make_pair(3, "Interleaver")] += steady_clock::now() - t_inter;

				// display Y_N7
				std::cout << "Y_N7:" << std::endl;
				ft.display_real_vector(this->Y_N7[0]);
				std::cout << std::endl;
			}
			// hard decode
			else
			{
				// decode
				std::cout << "Hard decode from Y_N5 to V_K1..." << std::endl;
				auto t_decod = steady_clock::now();
				this->decoder[0]->hard_decode(this->Y_N5[0], this->V_K1[0]);
				this->durations[0][std::make_pair(11, "Decoder")] += steady_clock::now() - t_decod;

				// display V_K1
				std::cout << "V_K1:" << std::endl;
				ft.display_real_vector(this->V_K1[0], this->U_K2[0]);
				std::cout << std::endl;
			}
		}

		// apply the coset to recover the real bits
		if (this->params.code.coset)
		{
			std::cout << "Apply the coset approach on V_K1..." << std::endl;
			auto t_cobit = steady_clock::now();
			this->coset_bit[0]->apply(this->U_K2[0], this->V_K1[0], this->V_K1[0]);
			this->durations[0][std::make_pair(12, "Coset bit")] += steady_clock::now() - t_cobit;

			// display V_K1
			std::cout << "V_K1:" << std::endl;
			ft.display_real_vector(this->V_K1[0], this->U_K2[0]);
			std::cout << std::endl;
		}

		// extract the CRC bits and keep only the information bits
		std::cout << "Extract the CRC bits from V_K1 and keep only the info. bits in V_K2..." << std::endl;
		auto t_crcex = steady_clock::now();
		this->crc[0]->extract(this->V_K1[0], this->V_K2[0]);
		this->durations[0][std::make_pair(13, "CRC extract")] += steady_clock::now() - t_crcex;

		// display V_K2
		std::cout << "V_K2:" << std::endl;
		ft.display_real_vector(this->V_K2[0], this->U_K1[0]);
		std::cout << std::endl;

		// check errors in the frame
		auto t_check = steady_clock::now();
		this->monitor[0]->check_errors(this->U_K1[0], this->V_K2[0]);
		this->durations[0][std::make_pair(14, "Check errors")] += steady_clock::now() - t_check;
	}
}

template <typename B, typename R, typename Q>
Terminal_BFER<B>* Simulation_BFER_ite_threads<B,R,Q>
::build_terminal()
{
#ifdef ENABLE_MPI
	return Simulation_BFER<B,R,Q>::build_terminal();
#else
	this->durations_red[std::make_pair(11, "Decoder")] = std::chrono::nanoseconds(0);
	const auto &d_dec = this->durations_red[std::make_pair(11, "Decoder")];

	return new Terminal_BFER<B>(this->params.code.K_info,
	                            this->params.code.N_code,
	                            *this->monitor_red,
	                            &d_dec);
#endif
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
#ifdef MULTI_PREC
template class aff3ct::simulation::Simulation_BFER_ite_threads<B_8,R_8,Q_8>;
template class aff3ct::simulation::Simulation_BFER_ite_threads<B_16,R_16,Q_16>;
template class aff3ct::simulation::Simulation_BFER_ite_threads<B_32,R_32,Q_32>;
template class aff3ct::simulation::Simulation_BFER_ite_threads<B_64,R_64,Q_64>;
#else
template class aff3ct::simulation::Simulation_BFER_ite_threads<B,R,Q>;
#endif
// ==================================================================================== explicit template instantiation
