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
#include <stdexcept>

#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class SISO_i
 *
 * \brief A SISO (Soft Input Soft Output) is a type of decoder which takes a soft input and return a soft output.
 *
 * \tparam R: type of the reals (floating-point or fixed-point representation) in the SISO.
 *
 * Please use SISO for inheritance (instead of SISO_i).
 */
template <typename R = float>
class SISO_i : public Module
{
private:
	const int n_dec_waves_siso;
	const int n_inter_frame_rest_siso;

	mipp::vector<R> Y_N1;
	mipp::vector<R> Y_N2;

protected:
	const int K_siso; /*!< Number of information bits in one frame */
	const int N_siso; /*!< Size of one frame (= number of bits in one frame) */
	const int simd_inter_frame_level_siso;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:                      number of information bits in the frame.
	 * \param N:                      size of one frame.
	 * \param n_frames:               number of frames to process in the SISO.
	 * \param simd_inter_frame_level: number of frames absorbed by the SIMD instructions.
	 * \param name:                   decoder name.
	 */
	SISO_i(const int K, const int N, const int n_frames = 1, const int simd_inter_frame_level = 1,
	       std::string name = "SISO_i")
	: Module(n_frames, name),
	  n_dec_waves_siso((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
	  n_inter_frame_rest_siso(this->n_frames % simd_inter_frame_level),
	  Y_N1(simd_inter_frame_level * N, (R)0),
	  Y_N2(simd_inter_frame_level * N, (R)0),
	  K_siso(K), N_siso(N), simd_inter_frame_level_siso(simd_inter_frame_level)
	{
		if (K <= 0)
			throw std::invalid_argument("aff3ct::module::SISO: \"K\" has to be greater than 0.");
		if (N <= 0)
			throw std::invalid_argument("aff3ct::module::SISO: \"N\" has to be greater than 0.");
		if (simd_inter_frame_level <= 0)
			throw std::invalid_argument("aff3ct::module::SISO: \"simd_inter_frame_level\" has to be greater than 0.");
		if (K > N)
			throw std::invalid_argument("aff3ct::module::SISO: \"K\" has to be smaller than \"N\".");
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
	virtual void soft_decode(const mipp::vector<R> &sys, const mipp::vector<R> &par, mipp::vector<R> &ext,
	                         const int n_frames = -1)
	{
		if (n_frames != -1 && n_frames <= 0)
			throw std::invalid_argument("aff3ct::module::SISO: \"n_frames\" has to be greater than 0 "
			                            "(or equal to -1).");

		const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

		if ((this->K_siso + this->tail_length() / 2) * real_n_frames != (int)sys.size())
			throw std::length_error("aff3ct::module::SISO: \"sys.size()\" has to be equal to "
			                        "\"(K_siso + tail_length() / 2)\" * \"real_n_frames\".");

		if (((this->N_siso - this->K_siso) - this->tail_length() / 2) * real_n_frames != (int)par.size())
			throw std::length_error("aff3ct::module::SISO: \"par.size()\" has to be equal to "
			                        "\"((N_siso - K_siso) - tail_length() / 2)\" * \"real_n_frames\".");

		if (this->K_siso * real_n_frames != (int)ext.size())
			throw std::length_error("aff3ct::module::SISO: \"ext.size()\" has to be equal to "
			                        "\"K_siso\" * \"real_n_frames\".");

		this->soft_decode(sys.data(), par.data(), ext.data(), real_n_frames);
	}

	virtual void soft_decode(const R *sys, const R *par, R *ext, const int n_frames = -1)
	{
		const int real_n_frames = (n_frames != -1) ? n_frames : this->n_frames;

		const auto n_dec_waves_siso = real_n_frames / this->simd_inter_frame_level_siso;
		for (auto w = 0; w < n_dec_waves_siso; w++)
			this->_soft_decode(sys + w * (               this->K_siso) * this->simd_inter_frame_level_siso,
			                   par + w * (this->N_siso - this->K_siso) * this->simd_inter_frame_level_siso,
			                   ext + w * (               this->K_siso) * this->simd_inter_frame_level_siso);
	}

	/*!
	 * \brief Decodes a given noisy codeword.
	 *
	 * \param Y_N1: a completely noisy codeword from the channel.
	 * \param Y_N2: an extrinsic information about all the bits in the frame.
	 */
	void soft_decode(const mipp::vector<R> &Y_N1, mipp::vector<R> &Y_N2)
	{
		if (this->N_siso * this->n_frames != (int)Y_N1.size())
			throw std::length_error("aff3ct::module::SISO: \"Y_N1.size()\" has to be equal to "
			                        "\"N_siso\" * \"n_frames\".");

		if (this->N_siso * this->n_frames != (int)Y_N2.size())
			throw std::length_error("aff3ct::module::SISO: \"Y_N2.size()\" has to be equal to "
			                        "\"N_siso\" * \"n_frames\".");

		this->soft_decode(Y_N1.data(), Y_N2.data());
	}

	virtual void soft_decode(const R *Y_N1, R *Y_N2)
	{
		auto w = 0;
		for (w = 0; w < this->n_dec_waves_siso -1; w++)
			this->_soft_decode(Y_N1 + w * this->N_siso * this->simd_inter_frame_level_siso,
			                   Y_N2 + w * this->N_siso * this->simd_inter_frame_level_siso);

		if (this->n_inter_frame_rest_siso == 0)
			this->_soft_decode(Y_N1 + w * this->N_siso * this->simd_inter_frame_level_siso,
			                   Y_N2 + w * this->N_siso * this->simd_inter_frame_level_siso);
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level_siso * this->N_siso;
			std::copy(Y_N1 + waves_off1,
			          Y_N1 + waves_off1 + this->n_inter_frame_rest_siso * this->N_siso,
			          this->Y_N1.begin());

			this->_soft_decode(this->Y_N1.data(), this->Y_N2.data());

			const auto waves_off2 = w * this->simd_inter_frame_level_siso * this->N_siso;
			std::copy(this->Y_N2.begin(),
			          this->Y_N2.begin() + this->n_inter_frame_rest_siso * this->N_siso,
			          Y_N2 + waves_off2);
		}
	}

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const
	{
		return 0;
	}

	/*!
	 * \brief Gets the number of frames absorbed by the SIMD instructions.
	 *
	 * \return the number of frames absorbed by the SIMD instructions.
	 */
	int get_simd_inter_frame_level() const
	{
		return this->simd_inter_frame_level_siso;
	}

protected:
	virtual void _soft_decode(const R *sys, const R *par, R *ext)
	{
		throw std::runtime_error("aff3ct::module::SISO: \"_soft_decode\" is unimplemented.");
	}

	virtual void _soft_decode(const R *Y_N1, R *Y_N2)
	{
		throw std::runtime_error("aff3ct::module::SISO: \"_soft_decode\" is unimplemented.");
	}
};
}
}

#include "SC_SISO.hpp"

#endif /* SISO_HPP_ */
