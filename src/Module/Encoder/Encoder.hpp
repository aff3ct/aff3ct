/*!
 * \file
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef ENCODER_HPP_
#define ENCODER_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
	namespace enc
	{
		namespace tsk
		{
			enum list { encode, SIZE };
		}

		namespace sck
		{
			namespace encode { enum list { U_K, X_N, SIZE }; }
		}
	}


/*!
 * \class Encoder
 *
 * \brief Encodes a vector of information bits (a message) and adds some redundancy (parity bits).
 *
 * \tparam B: type of the bits in the encoder.
 *
 * Please use Encoder for inheritance (instead of Encoder)
 */
template <typename B = int>
class Encoder : public Module
{
protected:
	const int             K;             /*!< Number of information bits in one frame */
	const int             N;             /*!< Size of one frame (= number of bits in one frame) */
	      bool            sys;           /*!< Is the generated codeworde are systematics ? */
	      bool            memorizing;    /*!< If true, keep the last encoded frame(s) in memory */
	std::vector<uint32_t> info_bits_pos; /*!< Positions of the information bits in the codeword */

	std::vector<std::vector<B>> U_K_mem;
	std::vector<std::vector<B>> X_N_mem;

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param n_frames: number of frames to process in the Encoder.
	 * \param name:     Encoder's name.
	 */
	Encoder(const int K, const int N, const int n_frames = 1)
	: Module(n_frames), 
	  K(K), 
	  N(N), 
	  sys(true), 
	  memorizing(false), 
	  info_bits_pos(this->K),
	  U_K_mem(n_frames),
	  X_N_mem(n_frames)
	{
		const std::string name = "Encoder";
		this->set_name(name);
		this->set_short_name(name);

		if (K <= 0)
		{
			std::stringstream message;
			message << "'K' has to be greater than 0 ('K' = " << K << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (K > N)
		{
			std::stringstream message;
			message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		auto &p = this->create_task("encode");
		auto &ps_U_K = this->template create_socket_in <B>(p, "U_K", this->K * this->n_frames);
		auto &ps_X_N = this->template create_socket_out<B>(p, "X_N", this->N * this->n_frames);
		this->create_codelet(p, [this, &ps_U_K, &ps_X_N]() -> int
		{
			this->encode(static_cast<B*>(ps_U_K.get_dataptr()),
			             static_cast<B*>(ps_X_N.get_dataptr()));

			return 0;
		});

		std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Encoder()
	{
	}

	int get_K() const
	{
		return this->K;
	}

	int get_N() const
	{
		return this->N;
	}

	virtual bool is_sys() const
	{
		return this->sys;
	}

	bool is_memorizing() const
	{
		return this->memorizing;
	}

	void set_memorizing(const bool memorizing)
	{
		this->memorizing = memorizing;

		if (this->memorizing)
		{
			if (!this->U_K_mem[0].size())
			{
				for (auto f = 0; f < this->n_frames; f++)
				{
					this->U_K_mem[f].resize(this->K);
					std::fill(this->U_K_mem[f].begin(), this->U_K_mem[f].end(), (B)0);
				}
			}
			if (!this->X_N_mem[0].size())
			{
				for (auto f = 0; f < this->n_frames; f++)
				{
					this->X_N_mem[f].resize(this->N);
					std::fill(this->X_N_mem[f].begin(), this->X_N_mem[f].end(), (B)0);
				}
			}
		}
	}

	const std::vector<B>& get_U_K(const int frame_id = 0) const
	{
		return this->U_K_mem[frame_id % this->n_frames];
	}

	const std::vector<B>& get_X_N(const int frame_id = 0) const
	{
		return this->X_N_mem[frame_id % this->n_frames];
	}

	/*!
	 * \brief Encodes a vector of information bits (a message).
	 *
	 * \param U_K: a vector of information bits (a message).
	 * \param X_N: an encoded frame with redundancy added (parity bits).
	 */
	template <class A = std::allocator<B>>
	void encode(const std::vector<B,A>& U_K, std::vector<B,A>& X_N, const int frame_id = -1)
	{
		if (this->K * this->n_frames != (int)U_K.size())
		{
			std::stringstream message;
			message << "'U_K.size()' has to be equal to 'K' * 'n_frames' ('U_K.size()' = " << U_K.size()
			        << ", 'K' = " << this->K
			        << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)X_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'N' * 'n_frames' ('X_N.size()' = " << X_N.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (frame_id != -1 && frame_id >= this->n_frames)
		{
			std::stringstream message;
			message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = " 
			        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->encode(U_K.data(), X_N.data(), frame_id);
	}

	virtual void encode(const B *U_K, B *X_N, const int frame_id = -1)
	{
		const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
		const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

		if (this->is_memorizing())
			for (auto f = f_start; f < f_stop; f++)
				std::copy(U_K + (f +0) * this->K,
				          U_K + (f +1) * this->K,
				          U_K_mem[f].begin());

		for (auto f = f_start; f < f_stop; f++)
			this->_encode(U_K + f * this->K,
			              X_N + f * this->N,
			              f);

		if (this->is_memorizing())
			for (auto f = f_start; f < f_stop; f++)
				std::copy(X_N + (f +0) * this->N,
				          X_N + (f +1) * this->N,
				          X_N_mem[f].begin());
	}

	template <class A = std::allocator<B>>
	bool is_codeword(const std::vector<B,A>& X_N)
	{
		if (this->N != (int)X_N.size())
		{
			std::stringstream message;
			message << "'X_N.size()' has to be equal to 'N' ('X_N.size()' = " << X_N.size()
			        << ", 'N' = " << this->N << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->is_codeword(X_N.data());
	}

	virtual bool is_codeword(const B *X_N)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual const std::vector<uint32_t>& get_info_bits_pos()
	{
		return info_bits_pos;
	}

	/*!
	 * \brief Gets the number of tail bits.
	 *
	 * Some encoders need to add some additional bits in order to work. This is often the case for the cyclic codes.
	 * Other codes do not require this feature and so, the tail bits length is 0.
	 *
	 * \return the number of tail bits.
	 */
	virtual int tail_length() const { return 0; }

protected:
	virtual void _encode(const B *U_K, B *X_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	void set_sys(const bool sys)
	{
		this->sys = sys;
	}
};
}
}

#endif /* ENCODER_HPP_ */
