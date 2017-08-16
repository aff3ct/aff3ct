#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

#include "Tools/Exception/exception.hpp"
#include "Tools/Display/Frame_trace/Frame_trace.hpp"

#include "Factory/Tools/Display/Terminal/BFER/Terminal_BFER.hpp"

#include "BFER_ite_threads.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

template <typename B, typename R, typename Q>
BFER_ite_threads<B,R,Q>
::BFER_ite_threads(const factory::BFER_ite::parameters &chain_params, tools::Codec_SISO<B,Q> &codec)
: BFER_ite<B,R,Q>(chain_params,codec),

  U_K1(this->params.n_threads, mipp::vector<B>(this->params.src->K     * this->params.src->n_frames)),
  U_K2(this->params.n_threads, mipp::vector<B>(this->params.enc->K     * this->params.enc->n_frames)),
  X_N1(this->params.n_threads, mipp::vector<B>(this->params.enc->N_cw  * this->params.enc->n_frames)),
  X_N2(this->params.n_threads, mipp::vector<B>(this->params.itl->size  * this->params.itl->n_frames)),
  X_N3(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_mod * this->params.mdm->n_frames)),
  H_N (this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N1(this->params.n_threads, mipp::vector<R>(this->params.chn->N     * this->params.chn->n_frames)),
  Y_N2(this->params.n_threads, mipp::vector<R>(this->params.mdm->N_fil * this->params.mdm->n_frames)),
  Y_N3(this->params.n_threads, mipp::vector<Q>(this->params.qnt->size  * this->params.qnt->n_frames)),
  Y_N4(this->params.n_threads, mipp::vector<Q>(this->params.mdm->N     * this->params.mdm->n_frames)),
  Y_N5(this->params.n_threads, mipp::vector<Q>(this->params.itl->size  * this->params.itl->n_frames)),
  Y_N6(this->params.n_threads, mipp::vector<Q>(this->params.dec->N_cw  * this->params.dec->n_frames)),
  Y_N7(this->params.n_threads, mipp::vector<Q>(this->params.itl->size  * this->params.itl->n_frames)),
  V_N1(this->params.n_threads, mipp::vector<B>(this->params.dec->N_cw  * this->params.crc->n_frames)),
  V_K1(this->params.n_threads, mipp::vector<B>(this->params.dec->K     * this->params.dec->n_frames)),
  V_K2(this->params.n_threads, mipp::vector<B>(this->params.src->K     * this->params.src->n_frames))
{
	if (this->params.n_threads > 1 && this->params.debug)
		std::clog << tools::format_warning("Debug mode will be disabled because you launched the simulation with more"
		                                   "than 1 thread!") << std::endl;

	if (this->params.benchs)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The bench mode is not supported.");

#ifdef ENABLE_MPI
	if (this->params.debug || this->params.benchs)
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, "The debug and bench modes are unavailable in MPI.");
#endif

	if (this->params.err_track_revert)
	{
		if (this->params.n_threads != 1)
			std::clog << tools::format_warning("Multi-threading detected with error tracking revert feature! "
			                                   "Each thread will play the same frames. Please run one thread.")
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
::_build_communication_chain(const int tid)
{
	BFER_ite<B,R,Q>::_build_communication_chain(tid);

	if (this->params.src->type == "AZCW")
	{
		std::fill(this->U_K1[tid].begin(), this->U_K1[tid].end(), (B)0);
		std::fill(this->U_K2[tid].begin(), this->U_K2[tid].end(), (B)0);
		std::fill(this->X_N1[tid].begin(), this->X_N1[tid].end(), (B)0);
		std::fill(this->X_N2[tid].begin(), this->X_N2[tid].end(), (B)0);
		this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
	}

	if (this->params.err_track_enable)
	{
		if (this->params.src->type != "AZCW")
			this->dumper[tid]->register_data(U_K1[tid], "src", false, {});

		if (this->params.coset)
			this->dumper[tid]->register_data(X_N1[tid], "enc", false, {(unsigned)this->params.enc->K});

		this->dumper[tid]->register_data(this->channel[tid]->get_noise(), "chn", true, {});

		if (this->interleaver[tid]->is_uniform())
			this->dumper[tid]->register_data(this->interleaver[tid]->get_lut(), "itl", false, {});
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::_launch()
{
	std::vector<std::thread> threads(this->params.n_threads -1);
	// launch a group of slave threads (there is "n_threads -1" slave threads)
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1] = std::thread(BFER_ite_threads<B,R,Q>::start_thread, this, tid);

	// launch the master thread
	BFER_ite_threads<B,R,Q>::start_thread(this, 0);

	// join the slave threads with the master thread
	for (auto tid = 1; tid < this->params.n_threads; tid++)
		threads[tid -1].join();
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::start_thread(BFER_ite_threads<B,R,Q> *simu, const int tid)
{
	try
	{
		simu->Monte_Carlo_method(tid);
	}
	catch (std::exception const& e)
	{
		module::Monitor<B>::stop();

		simu->mutex_exception.lock();
		if (simu->prev_err_message != e.what())
		{
			std::cerr << tools::apply_on_each_line(e.what(), &tools::format_error) << std::endl;
			simu->prev_err_message = e.what();
		}
		simu->mutex_exception.unlock();
	}
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::Monte_Carlo_method(const int tid)
{
	this->simulation_loop(tid);
}

template <typename B, typename R, typename Q>
void BFER_ite_threads<B,R,Q>
::simulation_loop(const int tid)
{
	using namespace std::chrono;
	auto t_snr = steady_clock::now();

	// simulation loop
	while ((!this->monitor_red->fe_limit_achieved()) && // while max frame error count has not been reached
	        (this->params.stop_time == seconds(0) || (steady_clock::now() - t_snr) < this->params.stop_time))
	{
		if (this->params.src->type != "AZCW")
		{
			this->source[tid]->generate(this->U_K1[tid]);
			this->crc[tid]->build(this->U_K1[tid], this->U_K2[tid]);
			this->encoder[tid]->encode(this->U_K2[tid], this->X_N1[tid]);
			this->interleaver[tid]->interleave(this->X_N1[tid], this->X_N2[tid]);
			this->modem[tid]->modulate(this->X_N2[tid], this->X_N3[tid]);
		}

		// Rayleigh channel
		if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
			this->channel[tid]->add_noise_wg(this->X_N3[tid], this->Y_N1[tid], this->H_N[tid]);
		else // additive channel (AWGN, USER, NO)
			this->channel[tid]->add_noise(this->X_N3[tid], this->Y_N1[tid]);

		this->modem[tid]->filter(this->Y_N1[tid], this->Y_N2[tid]);
		this->quantizer[tid]->process(this->Y_N2[tid], this->Y_N3[tid]);

		std::fill(this->Y_N7[tid].begin(), this->Y_N7[tid].end(), (Q)0);
		for (auto ite = 0; ite <= this->params.n_ite; ite++)
		{
			// Rayleigh channel
			if (this->params.chn->type.find("RAYLEIGH") != std::string::npos)
				this->modem[tid]->tdemodulate_wg(this->Y_N3[tid], this->H_N[tid], this->Y_N7[tid], this->Y_N4[tid]);
			else // additive channel (AWGN, USER, NO)
				this->modem[tid]->tdemodulate(this->Y_N3[tid], this->Y_N7[tid], this->Y_N4[tid]);

			this->interleaver[tid]->deinterleave(this->Y_N4[tid], this->Y_N5[tid]);

			if (this->params.coset)
				this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N5[tid], this->Y_N5[tid]);

			// soft decode
			if (ite != this->params.n_ite)
			{
				this->siso[tid]->decode_siso(this->Y_N5[tid], this->Y_N6[tid]);

				// apply the coset to recover the extrinsic information
				if (this->params.coset)
					this->coset_real[tid]->apply(this->X_N1[tid], this->Y_N6[tid], this->Y_N6[tid]);

				this->interleaver[tid]->interleave(this->Y_N6[tid], this->Y_N7[tid]);
			}
			// hard decode
			else
			{
				if (this->params.coded_monitoring)
					this->decoder[tid]->decode_siho_coded(this->Y_N5[tid], this->V_N1[tid]);
				else
					this->decoder[tid]->decode_siho(this->Y_N5[tid], this->V_K1[tid]);
			}
		}

		if (this->params.coded_monitoring)
		{
			if (this->params.coset)
				this->coset_bit[tid]->apply(this->X_N1[tid], this->V_N1[tid], this->V_N1[tid]);

			this->monitor[tid]->check_errors(this->X_N1[tid], this->V_N1[tid]);
		}
		else
		{
			if (this->params.coset)
				this->coset_bit[tid]->apply(this->U_K2[tid], this->V_K1[tid], this->V_K1[tid]);

			this->crc[tid]->extract(this->V_K1[tid], this->V_K2[tid]);
			this->monitor[tid]->check_errors(this->U_K1[tid], this->V_K2[tid]);
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
