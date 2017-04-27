/*!
 * \file
 * \brief Punctures a codeword to match a frame size.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PUNCTURER_HPP_
#define PUNCTURER_HPP_

#include <stdexcept>
#include <string>
#include <vector>
#include "Tools/Perf/MIPP/mipp.h"

#include "Module/Module.hpp"

namespace aff3ct
{
namespace module
{
/*!
 * \class Puncturer_i
 *
 * \brief Punctures a codeword to match a frame size.
 *
 * \tparam B: type of the bits in the frames.
 * \tparam Q: type of the reals (floating-point or fixed-point representation) in the Puncturer.
 *
 * Please use Puncturer for inheritance (instead of Puncturer_i)
 */
template <typename B = int, typename Q = float>
class Puncturer_i : public Module
{
protected:
	const int K;      /*!< Number of information bits in one frame */
	const int N;      /*!< Size of one frame (= number of bits in one frame) */
	const int N_code; /*!< Real size of the codeword (Puncturer_i::N_code >= Puncturer_i::N) */

public:
	/*!
	 * \brief Constructor.
	 *
	 * \param K:        number of information bits in the frame.
	 * \param N:        size of one frame.
	 * \param N_code:   real size of the codeword (Puncturer_i::N_code >= Puncturer_i::N).
	 * \param n_frames: number of frames to process in the Puncturer.
	 * \param name:     Puncturer's name.
	 */
	Puncturer_i(const int K, const int N, const int N_code, const int n_frames = 1,
	            const std::string name = "Puncturer_i")
	: Module(n_frames, name), K(K), N(N), N_code(N_code)
	{
		if (K <= 0)
			throw std::invalid_argument("aff3ct::module::Puncturer: \"K\" has to be greater than 0.");
		if (N <= 0)
			throw std::invalid_argument("aff3ct::module::Puncturer: \"N\" has to be greater than 0.");
		if (N_code <= 0)
			throw std::invalid_argument("aff3ct::module::Puncturer: \"N_code\" has to be greater than 0.");
		if (K > N)
			throw std::invalid_argument("aff3ct::module::Puncturer: \"K\" has to be smaller than \"N\".");
		if (N > N_code)
			throw std::invalid_argument("aff3ct::module::Puncturer: \"N_code\" has to be equal or greater than \"N\".");
	}

	/*!
	 * \brief Destructor.
	 */
	virtual ~Puncturer_i() {}

	int get_K() const
	{
		return K;
	}

	int get_N() const
	{
		return N;
	}

	int get_N_code() const
	{
		return N_code;
	}

	/*!
	 * \brief Punctures a codeword.
	 *
	 * \param X_N1: a complete/valid codeword..
	 * \param X_N2: a punctured codeword (corresponding to the frame size).
	 */
	void puncture(const mipp::vector<B>& X_N1, mipp::vector<B>& X_N2) const
	{
		if (this->N_code * this->n_frames != (int)X_N1.size())
			throw std::length_error("aff3ct::module::Puncturer: \"X_N1.size()\" has to be equal to "
			                        "\"N_code\" * \"n_frames\".");

		if (this->N * this->n_frames != (int)X_N2.size())
			throw std::length_error("aff3ct::module::Puncturer: \"X_N2.size()\" has to be equal to "
			                        "\"N\" * \"n_frames\".");

		this->puncture(X_N1.data(), X_N2.data());
	}

	virtual void puncture(const B *X_N1, B *X_N2) const
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_puncture(X_N1 + f * this->N_code,
			                X_N2 + f * this->N);
	}

	/*!
	 * \brief Depunctures a codeword.
	 *
	 * \param Y_N1: a noised and punctured codeword (corresponding to the frame size).
	 * \param Y_N2: a noised and complete/valid codeword.
	 */
	void depuncture(const mipp::vector<Q>& Y_N1, mipp::vector<Q>& Y_N2) const
	{
		if (this->N * this->n_frames != (int)Y_N1.size())
			throw std::length_error("aff3ct::module::Puncturer: \"Y_N1.size()\" has to be equal to "
			                        "\"N\" * \"n_frames\".");

		if (this->N_code * this->n_frames != (int)Y_N2.size())
			throw std::length_error("aff3ct::module::Puncturer: \"Y_N2.size()\" has to be equal to "
			                        "\"N_code\" * \"n_frames\".");

		this->depuncture(Y_N1.data(), Y_N2.data());
	}

	virtual void depuncture(const Q *Y_N1, Q *Y_N2) const
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_depuncture(Y_N1 + f * this->N,
			                  Y_N2 + f * this->N_code);
	}

protected:
	virtual void _puncture(const B *X_N1, B *X_N2) const
	{
		throw std::runtime_error("aff3ct::module::Puncturer: \"_puncture\" is unimplemented.");
	}

	virtual void _depuncture(const Q *Y_N1, Q *Y_N2) const
	{
		throw std::runtime_error("aff3ct::module::Puncturer: \"_depuncture\" is unimplemented.");
	}
};
}
}

#include "SC_Puncturer.hpp"

#endif /* PUNCTURER_HPP_ */
