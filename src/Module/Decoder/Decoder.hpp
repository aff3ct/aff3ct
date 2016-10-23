/*!
 * \file
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef DECODER_HPP_
#define DECODER_HPP_

#include <chrono>
#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Display/bash_tools.h"

#include "Module/Module.hpp"

/*!
 * \class Decoder_i
 *
 * \brief A Decoder is an algorithm dedicated to find the initial sequence of information bits (before the noise).
 *
 * \tparam B: type of the bits in the Decoder.
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the Decoder.
 *
 * The Decoder takes a soft input (real numbers) and return a hard output (bits).
 * Please use Decoder for inheritance (instead of Decoder_i).
 */
template <typename B, typename R>
class Decoder_i : public Module
{
private:
	const int n_dec_waves;
	const int n_inter_frame_rest;

	mipp::vector<mipp::vector<R>> Y_N;
	mipp::vector<mipp::vector<B>> V_N;

	std::chrono::nanoseconds d_load_total;
	std::chrono::nanoseconds d_decod_total;
	std::chrono::nanoseconds d_store_total;

protected:
	const int K; /*!< Number of information bits in one frame */
	const int N; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the Decoder.
	 * \param simd_inter_frame_level: number of frame absorbed by the SIMD instructions.
	 * \param name:                   Decoder's name.
	 */
	Decoder_i(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	          std::string name = "Decoder_i")
	: Module(n_frames, name),
	  n_dec_waves(std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
	  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
	  Y_N(n_dec_waves, mipp::vector<R>(simd_inter_frame_level * N + mipp::nElReg<R>(), 0)),
	  V_N(n_dec_waves, mipp::vector<B>(simd_inter_frame_level * N + mipp::nElReg<B>(), 0)),
	  K(K), N(N), simd_inter_frame_level(simd_inter_frame_level)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Decoder_i()
	{
	}

	/*!
	 * \brief Decodes the noisy frame.
	 *
	 * \param Y_N:        a noisy frame.
	 * \param V_K:        an decoded codeword (only the information bits).
	 * \param load:       true = performs the data loading, false = do not load the data in the decoder.
	 * \param store:      true = performs the data storing, false = do not store the data in the decoder.
	 * \param store_fast: true = enables the fast data storage.
	 * \param unpack:     true = unpacks the data after the fast storage.
	 */
	void hard_decode(const mipp::vector<R>& Y_N, mipp::vector<B>& V_K,
	                 const bool load       = true,
	                 const bool store      = true,
	                 const bool store_fast = false,
	                 const bool unpack     = false)
	{
		using namespace std::chrono;

		this->d_load_total  = std::chrono::nanoseconds(0);
		this->d_decod_total = std::chrono::nanoseconds(0);
		this->d_store_total = std::chrono::nanoseconds(0);

		if (this->n_dec_waves == 1 && this->n_inter_frame_rest == 0)
		{
			_hard_decode(Y_N, V_K, load, store, store_fast, unpack);
		}
		else
		{
			for (auto w = 0; w < this->n_dec_waves; w++)
			{
				auto t_load = steady_clock::now();
				const int n_frames_per_wave = (w == this->n_dec_waves -1 && this->n_inter_frame_rest != 0) ?
				                              this->n_inter_frame_rest :
				                              this->simd_inter_frame_level;

				if (load)
				{
					const auto waves_off1 = w * this->simd_inter_frame_level * this->N;
					std::copy(Y_N.begin() + waves_off1,
					          Y_N.begin() + waves_off1 + n_frames_per_wave * this->N,
					          this->Y_N[w].begin());
				}
				this->d_load_total += steady_clock::now() - t_load;

				_hard_decode(this->Y_N[w], this->V_N[w], load, store, store_fast, unpack);

				auto t_store = steady_clock::now();
				if (store)
				{
					if (this->K * this->n_frames == (int)V_K.size())
					{
						const auto waves_off2 = w * this->simd_inter_frame_level * this->K;
						std::copy(this->V_N[w].begin(),
						          this->V_N[w].begin() + n_frames_per_wave * this->K,
						          V_K.begin() + waves_off2);
					}
					else if (this->N * this->n_frames == (int)V_K.size())
					{
						const auto waves_off3 = w * this->simd_inter_frame_level * this->N;
						std::copy(this->V_N[w].begin(),
						          this->V_N[w].begin() + n_frames_per_wave * this->N,
						          V_K.begin() + waves_off3);
					}
					else
					{
						std::cerr << bold_red("(EE) This should never happen, V_K is not a multiple of K or of N.")
						          << std::endl;
						std::exit(-1);
					}
				}
				this->d_store_total += steady_clock::now() - t_store;
			}
		}
	}

	std::chrono::nanoseconds get_load_duration() const
	{
		return this->d_load_total;
	}

	std::chrono::nanoseconds get_decode_duration() const
	{
		return this->d_decod_total;
	}

	std::chrono::nanoseconds get_store_duration() const
	{
		return this->d_store_total;
	}

	int get_simd_inter_frame_level() const
	{
		return this->simd_inter_frame_level;
	}

protected:
	/*!
	 * \brief Loads the noisy frame into the Decoder.
	 *
	 * \param Y_N: a noisy frame.
	 */
	virtual void load(const mipp::vector<R>& Y_N) = 0;

	/*!
	 * \brief Decodes the noisy frame (have to be called after the load method).
	 */
	virtual void hard_decode() = 0;

	/*!
	 * \brief Stores the decoded information bits (have to be called after the decode method).
	 *
	 * \param V_K: an decoded codeword (only the information bits).
	 */
	virtual void store(mipp::vector<B>& V_K) const = 0;

	/*!
	 * \brief Stores the decoded codeword, may or may not contain the redundancy bits (parity) (should be called over
	 *        the standard store method).
	 *
	 * \param V: the decoded codeword.
	 */
	virtual void store_fast(mipp::vector<B>& V) const { store(V); }

	/*!
	 * \brief Can be called after the store_fast method if store_fast return the bits in a non-standard format. The
	 *        unpack method converts those bits into a standard format.
	 *
	 * \param V: the decoded and unpacked codeword.
	 */
	virtual void unpack(mipp::vector<B>& V) const {}

private:
	inline void _hard_decode(const mipp::vector<R>& Y_N, mipp::vector<B>& V_K,
	                         const bool load       = true,
	                         const bool store      = true,
	                         const bool store_fast = false,
	                         const bool unpack     = false)
	{
		using namespace std::chrono;

		auto t_load = steady_clock::now();
		if (load)
			this->load(Y_N);
		this->d_load_total += steady_clock::now() - t_load;

		auto t_decod = steady_clock::now();
		this->hard_decode();
		this->d_decod_total += steady_clock::now() - t_decod;

		auto t_store = steady_clock::now();
		if (store)
		{
			if (store_fast)
			{
				this->store_fast(V_K);
				if (unpack)
					this->unpack(V_K);
			}
			else
				this->store(V_K);
		}
		this->d_store_total += steady_clock::now() - t_store;
	}
};

#include "SC_Decoder.hpp"

#endif /* DECODER_HPP_ */
