#ifndef CODEC_HPP_
#define CODEC_HPP_

#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Tools/Perf/hard_decision.h"
#include "Tools/Interleaver/Interleaver_core.hpp"

#include "Factory/Module/Interleaver/Interleaver.hpp"

#include "Module/Interleaver/Interleaver.hpp"
#include "Module/Encoder/Encoder.hpp"
#include "Module/Puncturer/Puncturer.hpp"

#include "../Module.hpp"

namespace aff3ct
{
namespace module
{
template <typename B = int, typename Q = float>
class Codec : public Module
{
private:
	tools::Interleaver_core< > *interleaver_core;
	       Interleaver     <B> *interleaver_bit;
	       Interleaver     <Q> *interleaver_llr;

	Encoder  <B  > *encoder;
	Puncturer<B,Q> *puncturer;

protected :
	const int K;
	const int N_cw;
	const int N;
	const int tail_length;
	float sigma;

public:
	Codec(const int K, const int N_cw, const int N, const int tail_length = 0, const int n_frames = 1,
	      const std::string name = "Codec")
	: Module(n_frames, name, "Codec"),
	  interleaver_core(nullptr),
	  interleaver_bit (nullptr),
	  interleaver_llr (nullptr),
	  encoder         (nullptr),
	  puncturer       (nullptr),
	  K(K), N_cw(N_cw), N(N), tail_length(tail_length), sigma(-1.f)
	{
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

	virtual ~Codec()
	{
		if (encoder          != nullptr) { delete encoder;          encoder          = nullptr; }
		if (puncturer        != nullptr) { delete puncturer;        puncturer        = nullptr; }
		if (interleaver_bit  != nullptr) { delete interleaver_bit;  interleaver_bit  = nullptr; }
		if (interleaver_llr  != nullptr) { delete interleaver_llr;  interleaver_llr  = nullptr; }
		if (interleaver_core != nullptr) { delete interleaver_core; interleaver_core = nullptr; }
	}

	virtual tools::Interleaver_core<>* get_interleaver()
	{
		if (this->interleaver_core == nullptr)
		{
			std::stringstream message;
			message << "'interleaver_core' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->interleaver_core;
	}

	virtual Encoder<B>* get_encoder()
	{
		if (this->encoder == nullptr)
		{
			std::stringstream message;
			message << "'encoder' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->encoder;
	}

	virtual Puncturer<B,Q>* get_puncturer()
	{
		if (this->puncturer == nullptr)
		{
			std::stringstream message;
			message << "'puncturer' is NULL.";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		return this->puncturer;
	}

	virtual float get_sigma()
	{
		return this->sigma;
	}

	virtual void set_sigma(const float sigma)
	{
		if (sigma <= 0.f)
		{
			std::stringstream message;
			message << "'sigma' has to be greater than 0 ('sigma' = " << sigma << ").";
			throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
		}

		this->sigma = sigma;
	}

	template <class A = std::allocator<Q>>
	void extract_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K)
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

		this->extract_sys_llr(Y_N.data(), Y_K.data());
	}

	virtual void extract_sys_llr(const Q *Y_N, Q *Y_K)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_extract_sys_llr(Y_N + f * this->N_cw,
			                       Y_K + f * this->K,
			                       f);
	}

	template <class AQ = std::allocator<Q>, class AB = std::allocator<B>>
	void extract_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K)
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

		this->extract_sys_bit(Y_N.data(), V_K.data());
	}

	virtual void extract_sys_bit(const Q *Y_N, B *V_K)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_extract_sys_bit(Y_N + f * this->N_cw,
			                       V_K + f * this->K,
			                       f);
	}

	template <class A = std::allocator<Q>>
	void extract_sys_par(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par)
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

		this->extract_sys_par(Y_N.data(), sys.data(), par.data());
	}

	virtual void extract_sys_par(const Q *Y_N, Q *sys, Q *par)
	{
		const auto tb_2 = this->tail_length / 2;

		for (auto f = 0; f < this->n_frames; f++)
			this->_extract_sys_par(Y_N + f *  this->N_cw,
			                       sys + f * (this->K              + tb_2),
			                       par + f * (this->N_cw - this->K - tb_2),
			                       f);
	}

	template <class A = std::allocator<Q>>
	void add_sys_ext(const std::vector<Q,A> &ext, std::vector<Q,A> &Y_N)
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

		this->add_sys_ext(ext.data(), Y_N.data());
	}

	virtual void add_sys_ext(const Q *ext, Q *Y_N)
	{
		for (auto f = 0; f < this->n_frames; f++)
			this->_add_sys_ext(ext + f * this->K,
			                   Y_N + f * this->N_cw,
			                   f);
	}

	virtual void reset() {}

protected:
	virtual void _extract_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _extract_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _extract_sys_par(const Q *Y_N, Q *sys, Q *par, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void _add_sys_ext(const Q *ext, Q *Y_N, const int frame_id)
	{
		throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
	}

	virtual void set_interleaver(tools::Interleaver_core<>* itl)
	{
		this->interleaver_core = itl;
		this->interleaver_bit  = factory::Interleaver::build<B>(*itl);
		this->interleaver_llr  = factory::Interleaver::build<Q>(*itl);
	}

	virtual void set_encoder(Encoder<B>* enc)
	{
		this->encoder = enc;
	}

	virtual void set_puncturer(Puncturer<B,Q>* pct)
	{
		this->puncturer = pct;
	}

	virtual const Interleaver<B>& get_interleaver_bit()
	{
		return *this->interleaver_bit;
	}

	virtual const Interleaver<Q>& get_interleaver_llr()
	{
		return *this->interleaver_llr;
	}
};
}
}

#endif /* CODEC_SIHO_HPP_ */
