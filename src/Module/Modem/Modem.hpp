/*!
 * \file
 * \brief Modulates, filters and/or demodulates a signal.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef MODEM_HPP_
#define MODEM_HPP_

#include <stdexcept>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

#include "Tools/Exception/exception.hpp"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Modem
 *
 * \brief Modulates, filters and/or demodulates a signal.
 *
 * \tparam B: type of the bits or symbols to modulate.
 * \tparam R: type of the reals (floating-point representation) in the modulation and in the filtering process.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the demodulation.
 *
 * Please use Modem for inheritance (instead of Modem)
 */
template <typename B = int, typename R = float, typename Q = R>
class Modem : public Module
{
protected:
	const int N;     /*!< Size of one frame (= number of bits in one frame) */
	const int N_mod; /*!< Number of transmitted elements after the modulation (could be smaller, bigger or equal to N) */
	const int N_fil; /*!< Number of transmitted elements after the filtering process */
	      R   sigma; /*!< Sigma^2, the noise variance */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param N_mod:    number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 * \param N_fil:    number of transmitted elements after the filtering process.
	 * \param n_frames: number of frames to process in the Modem.
	 * \param name:     Modem's name.
	 */
	Modem(const int N, const int N_mod, const int N_fil, const R sigma = -1.f, const int n_frames = 1,
	      const std::string name = "Modem")
	: Module(n_frames, name, "Modem"), N(N), N_mod(N_mod), N_fil(N_fil), sigma(sigma)
	{
		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N_mod <= 0)
		{
			std::stringstream message;
			message << "'N_mod' has to be greater than 0 ('N_mod' = " << N_mod  << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N_fil <= 0)
		{
			std::stringstream message;
			message << "'N_fil' has to be greater than 0 ('N_fil' = " << N_fil << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->init_processes();
	}

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param N_mod:    number of transmitted elements after the modulation (could be smaller, bigger or equal to N).
	 * \param n_frames: number of frames to process in the Modem.
	 * \param name:     Modem's name.
	 */
	Modem(const int N, const int N_mod, const R sigma = -1.f, const int n_frames = 1,
	        const std::string name = "Modem")
	: Module(n_frames, name, "Modem"), N(N), N_mod(N_mod), N_fil(N_mod), sigma(sigma)
	{
		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (N_mod <= 0)
		{
			std::stringstream message;
			message << "'N_mod' has to be greater than 0 ('N_mod' = " << N_mod  << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->init_processes();
	}

	/*!
	 * \brief Constructor (assumes that nothing is done in the filtering process).
	 *
	 * \param N:        size of one frame (= number of bits in one frame).
	 * \param n_frames: number of frames to process in the Modem.
	 * \param name:     Modem's name.
	 */
	Modem(const int N, const R sigma = -1.f, const int n_frames = 1, const std::string name = "Modem")
	: Module(n_frames, name, "Modem"), N(N), N_mod(N), N_fil(N), sigma(sigma)
	{
		if (N <= 0)
		{
			std::stringstream message;
			message << "'N' has to be greater than 0 ('N' = " << N << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->init_processes();
	}

	void init_processes()
	{
		auto &p1 = this->create_task("modulate");
		auto &p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N     * this->n_frames);
		auto &p1s_X_N2 = this->template create_socket_out<R>(p1, "X_N2", this->N_mod * this->n_frames);
		this->create_codelet(p1, [this, &p1s_X_N1, &p1s_X_N2]() -> int
		{
			this->modulate(static_cast<B*>(p1s_X_N1.get_dataptr()),
			               static_cast<R*>(p1s_X_N2.get_dataptr()));

			return 0;
		});

		auto &p2 = this->create_task("filter");
		auto &p2s_Y_N1 = this->template create_socket_in <R>(p2, "Y_N1", this->N_mod * this->n_frames);
		auto &p2s_Y_N2 = this->template create_socket_out<R>(p2, "Y_N2", this->N_fil * this->n_frames);
		this->create_codelet(p2, [this, &p2s_Y_N1, &p2s_Y_N2]() -> int
		{
			this->filter(static_cast<R*>(p2s_Y_N1.get_dataptr()),
			             static_cast<R*>(p2s_Y_N2.get_dataptr()));

			return 0;
		});

		auto &p3 = this->create_task("demodulate");
		auto &p3s_Y_N1 = this->template create_socket_in <Q>(p3, "Y_N1", this->N_fil * this->n_frames);
		auto &p3s_Y_N2 = this->template create_socket_out<Q>(p3, "Y_N2", this->N     * this->n_frames);
		this->create_codelet(p3, [this, &p3s_Y_N1, &p3s_Y_N2]() -> int
		{
			this->demodulate(static_cast<Q*>(p3s_Y_N1.get_dataptr()),
			                 static_cast<Q*>(p3s_Y_N2.get_dataptr()));

			return 0;
		});

		auto &p4 = this->create_task("tdemodulate");
		auto &p4s_Y_N1 = this->template create_socket_in <Q>(p4, "Y_N1", this->N_fil * this->n_frames);
		auto &p4s_Y_N2 = this->template create_socket_in <Q>(p4, "Y_N2", this->N     * this->n_frames);
		auto &p4s_Y_N3 = this->template create_socket_out<Q>(p4, "Y_N3", this->N     * this->n_frames);
		this->create_codelet(p4, [this, &p4s_Y_N1, &p4s_Y_N2, &p4s_Y_N3]() -> int
		{
			this->tdemodulate(static_cast<Q*>(p4s_Y_N1.get_dataptr()),
			                  static_cast<Q*>(p4s_Y_N2.get_dataptr()),
			                  static_cast<Q*>(p4s_Y_N3.get_dataptr()));

			return 0;
		});

		auto &p5 = this->create_task("demodulate_wg");
		auto &p5s_H_N  = this->template create_socket_in <R>(p5, "H_N",  this->N_fil * this->n_frames);
		auto &p5s_Y_N1 = this->template create_socket_in <Q>(p5, "Y_N1", this->N_fil * this->n_frames);
		auto &p5s_Y_N2 = this->template create_socket_out<Q>(p5, "Y_N2", this->N     * this->n_frames);
		this->create_codelet(p5, [this, &p5s_H_N, &p5s_Y_N1, &p5s_Y_N2]() -> int
		{
			this->demodulate_wg(static_cast<R*>(p5s_H_N .get_dataptr()),
			                    static_cast<Q*>(p5s_Y_N1.get_dataptr()),
			                    static_cast<Q*>(p5s_Y_N2.get_dataptr()));

			return 0;
		});

		auto &p6 = this->create_task("tdemodulate_wg");
		auto &p6s_H_N  = this->template create_socket_in <R>(p6, "H_N",  this->N_fil * this->n_frames);
		auto &p6s_Y_N1 = this->template create_socket_in <Q>(p6, "Y_N1", this->N_fil * this->n_frames);
		auto &p6s_Y_N2 = this->template create_socket_in <Q>(p6, "Y_N2", this->N     * this->n_frames);
		auto &p6s_Y_N3 = this->template create_socket_out<Q>(p6, "Y_N3", this->N     * this->n_frames);
		this->create_codelet(p6, [this, &p6s_H_N, &p6s_Y_N1, &p6s_Y_N2, &p6s_Y_N3]() -> int
		{
			this->tdemodulate_wg(static_cast<R*>(p6s_H_N .get_dataptr()),
			                     static_cast<Q*>(p6s_Y_N1.get_dataptr()),
			                     static_cast<Q*>(p6s_Y_N2.get_dataptr()),
			                     static_cast<Q*>(p6s_Y_N3.get_dataptr()));

			return 0;
		});
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Modem()
	{
	}

	int get_N() const
	{
		return this->N;
	}

	int get_N_mod() const
	{
		return this->N_mod;
	}

	int get_N_fil() const
	{
		return this->N_fil;
	}

	R get_sigma() const
	{
		return this->sigma;
	}

	virtual void set_sigma(const R sigma)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->sigma = sigma;
	}

	/*!
	 * \brief Modulates a vector of bits or symbols.
	 *
	 * \param X_N1: a vector of bits or symbols.
	 * \param X_N2: a vector of modulated bits or symbols.
	 */
	template <class AB = std::allocator<B>, class AR = std::allocator<R>>
	void modulate(const std::vector<B,AB>& X_N1, std::vector<R,AR>& X_N2)
	{
		if (this->N * this->n_frames != (int)X_N1.size())
		{
			std::stringstream message;
			message << "'X_N1.size()' has to be equal to 'N' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_mod * this->n_frames != (int)X_N2.size())
		{
			std::stringstream message;
			message << "'X_N2.size()' has to be equal to 'N_mod' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
			        << ", 'N_mod' = " << this->N_mod << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->modulate(X_N1.data(), X_N2.data());
	}

	virtual void modulate(const B *X_N1, R *X_N2)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_modulate(X_N1 + f * this->N,
			                X_N2 + f * this->N_mod,
			                f);
	}

	/*!
	 * \brief Filters a vector of noised and modulated bits/symbols.
	 *
	 * By default this method does nothing.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a filtered vector.
	 */
	template <class A = std::allocator<R>>
	void filter(const std::vector<R,A>& Y_N1, std::vector<R,A>& Y_N2)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_mod * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N_mod' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N_mod' = " << this->N_mod << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->filter(Y_N1.data(), Y_N2.data());
	}

	virtual void filter(const R *Y_N1, R *Y_N2)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_filter(Y_N1 + f * this->N_mod,
			              Y_N2 + f * this->N_fil,
			              f);
	}

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a demodulated vector.
	 */
	template <class A = std::allocator<Q>>
	void demodulate(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->demodulate(Y_N1.data(), Y_N2.data());
	}

	virtual void demodulate(const Q *Y_N1, Q *Y_N2)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_demodulate(Y_N1 + f * this->N_fil,
			                  Y_N2 + f * this->N,
			                  f);
	}

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * \param H_N:  channel gains.
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a demodulated vector.
	 */
	template <class AQ = std::allocator<Q>, class AR = std::allocator<R>>
	void demodulate_wg(const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1, std::vector<Q,AQ>& Y_N2)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)H_N.size())
		{
			std::stringstream message;
			message << "'H_N.size()' has to be equal to 'N_fil' * 'n_frames' ('H_N.size()' = " << H_N.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->demodulate_wg(H_N.data(), Y_N1.data(), Y_N2.data());
	}

	virtual void demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_demodulate_wg(H_N  + f * this->N_fil,
			                     Y_N1 + f * this->N_fil,
			                     Y_N2 + f * this->N,
			                     f);
	}

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * Used for the iterative turbo demodulation technique, this type of demodulation takes the decoder information
	 * into account.
	 *
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a vector of extrinsic information coming from a SISO decoder (used in the iterative turbo
	 *              demodulation technique).
	 * \param Y_N3: a demodulated vector.
	 */
	template <class A = std::allocator<Q>>
	void tdemodulate(const std::vector<Q,A>& Y_N1, const std::vector<Q,A>& Y_N2, std::vector<Q,A>& Y_N3)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N3.size())
		{
			std::stringstream message;
			message << "'Y_N3.size()' has to be equal to 'N' * 'n_frames' ('Y_N3.size()' = " << Y_N3.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ".";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->tdemodulate(Y_N1.data(), Y_N2.data(), Y_N3.data());
	}

	virtual void tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_tdemodulate(Y_N1 + f * this->N_fil,
			                   Y_N2 + f * this->N,
			                   Y_N3 + f * this->N,
			                   f);
	}

	/*!
	 * \brief Demodulates a vector of noised and modulated bits/symbols (after the filtering process if required).
	 *
	 * Used for the iterative turbo demodulation technique, this type of demodulation takes the decoder information
	 * into account.
	 *
	 * \param H_N:  channel gains.
	 * \param Y_N1: a vector of noised and modulated bits/symbols.
	 * \param Y_N2: a vector of extrinsic information coming from a SISO decoder (used in the iterative turbo
	 *              demodulation technique).
	 * \param Y_N3: a demodulated vector.
	 */
	template <class AQ = std::allocator<Q>, class AR = std::allocator<R>>
	void tdemodulate_wg(const std::vector<R,AR>& H_N,  const std::vector<Q,AQ>& Y_N1,
	                    const std::vector<Q,AQ>& Y_N2,       std::vector<Q,AQ>& Y_N3)
	{
		if (sigma <= 0)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)Y_N1.size())
		{
			std::stringstream message;
			message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N_fil * this->n_frames != (int)H_N.size())
		{
			std::stringstream message;
			message << "'H_N.size()' has to be equal to 'N_fil' * 'n_frames' ('H_N.size()' = " << H_N.size()
			        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N2.size())
		{
			std::stringstream message;
			message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		if (this->N * this->n_frames != (int)Y_N3.size())
		{
			std::stringstream message;
			message << "'Y_N3.size()' has to be equal to 'N' * 'n_frames' ('Y_N3.size()' = " << Y_N3.size()
			        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
			throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
		}

		this->tdemodulate_wg(H_N.data(), Y_N1.data(), Y_N2.data(), Y_N3.data());
	}

	virtual void tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_tdemodulate_wg(H_N  + f * this->N_fil,
			                      Y_N1 + f * this->N_fil,
			                      Y_N2 + f * this->N,
			                      Y_N3 + f * this->N,
			                      f);
	}

	/*!
	 * \brief Gets the vector size after the modulation (considering a given frame size).
	 *
	 * \param N        : a frame size.
	 * \param n_b_per_s: number of bits per symbols
	 * \param tl       : tail length of the modulation (in number of symbols)
	 * \param s_factor : the sampling factor (number of samples per symbol)
	 * \param complex  : if true, the samples are in a complex form (so the size is twice longer)
	 *
	 * \return the vector size after the modulation.
	 */
	static int get_buffer_size_after_modulation(const int N, const int n_b_per_s, const int tl,
	                                            const int s_factor, const bool complex)
	{
		return ((int)(std::ceil((float)N / (float)n_b_per_s)) + tl) * s_factor * (complex ? 2 : 1);
	}

	/*!
	 * \brief Gets the vector size after the filtering process
	 *
	 * \param N        : a frame size.
	 * \param n_b_per_s: number of bits per symbols
	 * \param tl       : tail length of the modulation (in number of symbols)
	 * \param max_wa_id: the maximum number of possible wave forms
	 * \param complex  : if true, the samples are in a complex form (so the size is twice longer)
	 *
	 * \return the vector size after the modulation.
	 */
	static int get_buffer_size_after_filtering(const int N, const int n_b_per_s, const int tl,
	                                            const int max_wa_id, const bool complex)
	{
		return ((int)(std::ceil((float)N / (float)n_b_per_s)) + tl) * max_wa_id * (complex ? 2 : 1);
	}

protected:
	virtual void _modulate(const B *X_N1, R *X_N2, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _filter(const R *Y_N1, R *Y_N2, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}
};
}
}

#endif /* MODEM_HPP_ */
