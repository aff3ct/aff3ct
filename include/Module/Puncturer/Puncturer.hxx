/*!
 * \file
 * \brief Punctures a codeword to match a frame size.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef PUNCTURER_HXX_
#define PUNCTURER_HXX_

#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Puncturer<B,Q>::
Puncturer(const int K, const int N, const int N_cw, const int n_frames)
: Module(n_frames), K(K), N(N), N_cw(N_cw)
{
	const std::string name = "Puncturer";
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

	if (N_cw <= 0)
	{
		std::stringstream message;
		message << "'N_cw' has to be greater than 0 ('N_cw' = " << N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N > N_cw)
	{
		std::stringstream message;
		message << "'N' has to be smaller or equal to 'N_cw' ('N' = " << N << ", 'N_cw' = " << N_cw << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("puncture");
	auto &p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N_cw * this->n_frames);
	auto &p1s_X_N2 = this->template create_socket_out<B>(p1, "X_N2", this->N      * this->n_frames);
	this->create_codelet(p1, [this, &p1s_X_N1, &p1s_X_N2]() -> int
	{
		this->puncture(static_cast<B*>(p1s_X_N1.get_dataptr()),
		               static_cast<B*>(p1s_X_N2.get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("depuncture");
	auto &p2s_Y_N1 = this->template create_socket_in <Q>(p2, "Y_N1", this->N      * this->n_frames);
	auto &p2s_Y_N2 = this->template create_socket_out<Q>(p2, "Y_N2", this->N_cw * this->n_frames);
	this->create_codelet(p2, [this, &p2s_Y_N1, &p2s_Y_N2]() -> int
	{
		this->depuncture(static_cast<Q*>(p2s_Y_N1.get_dataptr()),
		                 static_cast<Q*>(p2s_Y_N2.get_dataptr()));

		return 0;
	});
}

template <typename B, typename Q>
int Puncturer<B,Q>::
get_K() const
{
	return K;
}

template <typename B, typename Q>
int Puncturer<B,Q>::
get_N() const
{
	return N;
}

template <typename B, typename Q>
int Puncturer<B,Q>::
get_N_cw() const
{
	return N_cw;
}

template <typename B, typename Q>
template <class A>
void Puncturer<B,Q>::
puncture(const std::vector<B,A>& X_N1, std::vector<B,A>& X_N2, const int frame_id) const
{
	if (this->N_cw * this->n_frames != (int)X_N1.size())
	{
		std::stringstream message;
		message << "'X_N1.size()' has to be equal to 'N_cw' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
		        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)X_N2.size())
	{
		std::stringstream message;
		message << "'X_N2.size()' has to be equal to 'N' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
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

	this->puncture(X_N1.data(), X_N2.data(), frame_id);
}

template <typename B, typename Q>
void Puncturer<B,Q>::
puncture(const B *X_N1, B *X_N2, const int frame_id) const
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_puncture(X_N1 + f * this->N_cw,
		                X_N2 + f * this->N,
		                f);
}

template <typename B, typename Q>
template <class A>
void Puncturer<B,Q>::
depuncture(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2, const int frame_id) const
{
	if (this->N * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_cw * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->depuncture(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename B, typename Q>
void Puncturer<B,Q>::
depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_depuncture(Y_N1 + f * this->N,
		                  Y_N2 + f * this->N_cw,
		                  f);
}

template <typename B, typename Q>
void Puncturer<B,Q>::
_puncture(const B *X_N1, B *X_N2, const int frame_id) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Puncturer<B,Q>::
_depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}

#endif
