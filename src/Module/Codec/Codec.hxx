#include <sstream>
#include <string>

#include "Tools/Noise/noise_utils.h"
#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/common/hard_decide.h"
#include "Factory/Module/Interleaver/Interleaver.hpp"
#include "Module/Codec/Codec.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Codec<B,Q>
::Codec(const int K, const int N_cw, const int N, const int tail_length, const int n_frames)
: Module(n_frames),
  K(K), N_cw(N_cw), N(N), tail_length(tail_length),
  n(nullptr)
{
	const std::string name = "Codec";
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

	auto &p1 = this->create_task("extract_sys_llr");
	auto &p1s_Y_N = this->template create_socket_in <Q>(p1, "Y_N", this->N_cw * this->n_frames);
	auto &p1s_Y_K = this->template create_socket_out<Q>(p1, "Y_K", this->K    * this->n_frames);
	this->create_codelet(p1, [this, &p1s_Y_N, &p1s_Y_K]() -> int
	{
		this->extract_sys_llr(static_cast<Q*>(p1s_Y_N.get_dataptr()),
		                      static_cast<Q*>(p1s_Y_K.get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("extract_sys_bit");
	auto &p2s_Y_N = this->template create_socket_in <Q>(p2, "Y_N", this->N_cw * this->n_frames);
	auto &p2s_V_K = this->template create_socket_out<B>(p2, "V_K", this->K    * this->n_frames);
	this->create_codelet(p2, [this, &p2s_Y_N, &p2s_V_K]() -> int
	{
		this->extract_sys_bit(static_cast<Q*>(p2s_Y_N.get_dataptr()),
		                      static_cast<B*>(p2s_V_K.get_dataptr()));

		return 0;
	});

	const auto tb_2 = this->tail_length / 2;
	auto &p3 = this->create_task("extract_sys_par");
	auto &p3s_Y_N = this->template create_socket_in <Q>(p3, "Y_N",  this->N_cw                   * this->n_frames);
	auto &p3s_sys = this->template create_socket_out<Q>(p3, "sys", (this->K              + tb_2) * this->n_frames);
	auto &p3s_par = this->template create_socket_out<Q>(p3, "par", (this->N_cw - this->K - tb_2) * this->n_frames);
	this->create_codelet(p3, [this, &p3s_Y_N, &p3s_sys, &p3s_par]() -> int
	{
		this->extract_sys_par(static_cast<Q*>(p3s_Y_N.get_dataptr()),
		                      static_cast<Q*>(p3s_sys.get_dataptr()),
		                      static_cast<Q*>(p3s_par.get_dataptr()));

		return 0;
	});

	auto &p4 = this->create_task("add_sys_ext");
	auto &p4s_ext = this->template create_socket_in    <Q>(p4, "ext", this->K    * this->n_frames);
	auto &p4s_Y_N = this->template create_socket_in_out<Q>(p4, "Y_N", this->N_cw * this->n_frames);
	this->create_codelet(p4, [this, &p4s_ext, &p4s_Y_N]() -> int
	{
		this->add_sys_ext(static_cast<Q*>(p4s_ext.get_dataptr()),
		                  static_cast<Q*>(p4s_Y_N.get_dataptr()));

		return 0;
	});
}

template <typename B, typename Q>
std::unique_ptr<tools::Interleaver_core<>>& Codec<B,Q>
::get_interleaver()
{
	if (this->interleaver_core == nullptr)
	{
		std::stringstream message;
		message << "'interleaver_core' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->interleaver_core;
}

template <typename B, typename Q>
std::unique_ptr<Encoder<B>>& Codec<B,Q>
::get_encoder()
{
	if (this->encoder == nullptr)
	{
		std::stringstream message;
		message << "'encoder' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->encoder;
}

template <typename B, typename Q>
std::unique_ptr<Puncturer<B,Q>>& Codec<B,Q>
::get_puncturer()
{
	if (this->puncturer == nullptr)
	{
		std::stringstream message;
		message << "'puncturer' is NULL.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->puncturer;
}

template <typename B, typename Q>
const tools::Noise<float>& Codec<B,Q>
::current_noise() const
{
	return *this->n;
}

template <typename B, typename Q>
void Codec<B,Q>
::set_noise(const tools::Noise<float>& noise)
{
	this->n.reset(tools::cast<float>(noise));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_noise(const tools::Noise<double>& noise)
{
	this->n.reset(tools::cast<float>(noise));
}

template <typename B, typename Q>
template <class A>
void Codec<B,Q>
::extract_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K, const int frame_id)
{
	if (this->N_cw * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K * this->n_frames != (int)Y_K.size())
	{
		std::stringstream message;
		message << "'Y_K.size()' has to be equal to 'K' * 'n_frames' ('Y_K.size()' = " << Y_K.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->extract_sys_llr(Y_N.data(), Y_K.data(), frame_id);
}

template <typename B, typename Q>
void Codec<B,Q>
::extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_extract_sys_llr(Y_N + f * this->N_cw,
		                       Y_K + f * this->K,
		                       f);
}

template <typename B, typename Q>
template <class AQ, class AB>
void Codec<B,Q>
::extract_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K, const int frame_id)
{
	if (this->N_cw * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->K * this->n_frames != (int)V_K.size())
	{
		std::stringstream message;
		message << "'V_K.size()' has to be equal to 'K' * 'n_frames' ('V_K.size()' = " << V_K.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->extract_sys_bit(Y_N.data(), V_K.data(), frame_id);
}

template <typename B, typename Q>
void Codec<B,Q>
::extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_extract_sys_bit(Y_N + f * this->N_cw,
		                       V_K + f * this->K,
		                       f);
}

template <typename B, typename Q>
template <class A>
void Codec<B,Q>
::extract_sys_par(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par, const int frame_id)
{
	const auto tb_2 = this->tail_length / 2;

	if (this->N_cw * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N_cw' = " << this->N_cw << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->K + tb_2) * this->n_frames != (int)sys.size())
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to ('K' + 'tb_2') * 'n_frames' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << this->K << ", 'tb_2' = " << tb_2 << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->N_cw - this->K - tb_2) * this->n_frames != (int)par.size())
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to ('N_cw' - 'K' - 'tb_2') * 'n_frames' ('par.size()' = "
		        << par.size() << ", 'N_cw' = " << this->N_cw << ", 'K' = " << this->K << ", 'tb_2' = " << tb_2
		        << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (frame_id != -1 && frame_id >= this->n_frames)
	{
		std::stringstream message;
		message << "'frame_id' has to be equal to '-1' or to be smaller than 'n_frames' ('frame_id' = "
		        << frame_id << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->extract_sys_par(Y_N.data(), sys.data(), par.data(), frame_id);
}

template <typename B, typename Q>
void Codec<B,Q>
::extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id)
{
	const auto tb_2 = this->tail_length / 2;

	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_extract_sys_par(Y_N + f *  this->N_cw,
		                       sys + f * (this->K              + tb_2),
		                       par + f * (this->N_cw - this->K - tb_2),
		                       f);
}

template <typename B, typename Q>
template <class A>
void Codec<B,Q>
::add_sys_ext(const std::vector<Q,A> &ext, std::vector<Q,A> &Y_N, const int frame_id)
{
	if (this->K * this->n_frames != (int)ext.size())
	{
		std::stringstream message;
		message << "'ext.size()' has to be equal to 'K' * 'n_frames' ('ext.size()' = " << ext.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_cw * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N_cw' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
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

	this->add_sys_ext(ext.data(), Y_N.data(), frame_id);
}

template <typename B, typename Q>
void Codec<B,Q>
::add_sys_ext(const Q *ext, Q *Y_N, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_add_sys_ext(ext + f * this->K,
		                   Y_N + f * this->N_cw,
		                   f);
}

template <typename B, typename Q>
void Codec<B,Q>
::reset()
{
}

template <typename B, typename Q>
void Codec<B,Q>
::_extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec<B,Q>
::_extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec<B,Q>
::_extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec<B,Q>
::_add_sys_ext(const Q *ext, Q *Y_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(tools::Interleaver_core<>* itl)
{
	this->set_interleaver(std::unique_ptr<tools::Interleaver_core<>>(itl));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(Encoder<B>* enc)
{
	this->set_encoder(std::unique_ptr<Encoder<B>>(enc));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(Puncturer<B,Q>* pct)
{
	this->set_puncturer(std::unique_ptr<Puncturer<B,Q>>(pct));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_interleaver(std::unique_ptr<tools::Interleaver_core<>>&& itl)
{
	this->interleaver_core = std::move(itl);
	this->interleaver_bit.reset(factory::Interleaver::build<B>(*this->interleaver_core));
	this->interleaver_llr.reset(factory::Interleaver::build<Q>(*this->interleaver_core));
}

template <typename B, typename Q>
void Codec<B,Q>
::set_encoder(std::unique_ptr<Encoder<B>>&& enc)
{
	this->encoder = std::move(enc);
}

template <typename B, typename Q>
void Codec<B,Q>
::set_puncturer(std::unique_ptr<Puncturer<B,Q>>&& pct)
{
	this->puncturer = std::move(pct);
}

template <typename B, typename Q>
const Interleaver<B>& Codec<B,Q>
::get_interleaver_bit()
{
	return *this->interleaver_bit;
}

template <typename B, typename Q>
const Interleaver<Q>& Codec<B,Q>
::get_interleaver_llr()
{
	return *this->interleaver_llr;
}

}
}
