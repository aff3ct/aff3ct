/*!
 * \file
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which takes a soft input and return a soft output.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SISO_HPP_
#define SISO_HPP_

#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

/*!
 * \class SISO_i
 *
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which takes a soft input and return a soft output.
 *
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the SISO.
 *
 * Please use SISO for inheritance (instead of SISO_i).
 */
template <typename R>
class SISO_i : public Module
{
private:
	const int n_dec_waves_siso;
	const int n_inter_frame_rest_siso;

	mipp::vector<mipp::vector<R>> Y_N1;
	mipp::vector<mipp::vector<R>> Y_N2;

protected:
	const int K_siso; /*!< Number of information bits in one frame */
	const int N_siso; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level_siso;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K       : number of information bits in the frame.
	 * \param N       : size of one frame.
	 * \param n_frames: number of frames to process in the SISO.
	 * \param name    : decoder name.
	 */
	SISO_i(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	       std::string name = "SISO_i")
	: Module(n_frames, name),
	  n_dec_waves_siso(std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
	  n_inter_frame_rest_siso(this->n_frames % simd_inter_frame_level),
	  Y_N1(n_dec_waves_siso, mipp::vector<R>(simd_inter_frame_level * N + mipp::nElReg<R>(), 0)),
	  Y_N2(n_dec_waves_siso, mipp::vector<R>(simd_inter_frame_level * N + mipp::nElReg<R>(), 0)),
	  K_siso(K), N_siso(N), simd_inter_frame_level_siso(simd_inter_frame_level)
	{
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~SISO_i() {};

	/*!
	 * \brief Decodes a given noisy codeword. This prototype supposes that the encoded frame is systematic, can't be
	 *        used otherwise.
	 *
	 * \param sys: systematic LLRs (original noised information bits) from the channel.
	 * \param par: parity LLRs from the channel.
	 * \param ext: extrinsic information about the systematic bits.
	 */
	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext) = 0;

	/*!
	 * \brief Decodes a given noisy codeword.
	 *
	 * \param Y_N1: a completely noisy codeword from the channel.
	 * \param Y_N2: an extrinsic information about all the bits in the frame.
	 */
	void soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
	{
		if (this->n_dec_waves_siso == 1 && this->n_inter_frame_rest_siso == 0)
		{
			_soft_decode(Y_N1, Y_N2);
		}
		else
		{
			for (auto w = 0; w < this->n_dec_waves_siso; w++)
			{
				const int n_frames_per_wave = (w == this->n_dec_waves_siso -1 && this->n_inter_frame_rest_siso != 0) ?
				                              this->n_inter_frame_rest_siso :
				                              this->simd_inter_frame_level_siso;

				const auto waves_off = w * this->simd_inter_frame_level_siso * this->N_siso;
				std::copy(Y_N1.begin() + waves_off,
				          Y_N1.begin() + waves_off + n_frames_per_wave * this->N_siso,
				          this->Y_N1[w].begin());

				_soft_decode(this->Y_N1[w], this->Y_N2[w]);

				std::copy(this->Y_N2[w].begin(),
				          this->Y_N2[w].begin() + n_frames_per_wave * this->N_siso,
				          Y_N2.begin() + waves_off);
			}
		}
	}

	/*!
	 * \brief Decodes a given noisy codeword.
	 *
	 * \param Y_N1: a completely noisy codeword from the channel.
	 * \param Y_N2: an extrinsic information about all the bits in the frame.
	 */
	virtual void _soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2) = 0;
	
	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const
	{
		return 0;
	}

	int get_simd_inter_frame_level() const
	{
		return this->simd_inter_frame_level_siso;
	}
};

#include "SC_SISO.hpp"

#endif /* SISO_HPP_ */
