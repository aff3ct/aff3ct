#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Encoder/Encoder.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Encoder<B>
::operator[](const enc::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Encoder<B>
::operator[](const enc::sck::encode s)
{
	return Module::operator[]((size_t)enc::tsk::encode)[(size_t)s];
}

template <typename B>
Encoder<B>
::Encoder(const int K, const int N, const int simd_inter_frame_level)
: Module(),
  n_inter_frame_rest(this->n_frames % simd_inter_frame_level),
  K(K),
  N(N),
  simd_inter_frame_level(simd_inter_frame_level),
  n_enc_waves((int)std::ceil((float)this->n_frames / (float)simd_inter_frame_level)),
  sys(true),
  memorizing(false),
  info_bits_pos(this->K),
  U_K(this->n_inter_frame_rest ? this->simd_inter_frame_level * this->K : 0),
  X_N(this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0),
  U_K_mem(this->n_frames),
  X_N_mem(this->n_frames)
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

	if (simd_inter_frame_level <= 0)
	{
		std::stringstream message;
		message << "'simd_inter_frame_level' has to be greater than 0 ('simd_inter_frame_level' = "
		        << simd_inter_frame_level << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("encode");
	auto ps_U_K = this->template create_socket_in <B>(p, "U_K", this->K);
	auto ps_X_N = this->template create_socket_out<B>(p, "X_N", this->N);
	this->create_codelet(p, [ps_U_K, ps_X_N](Module &m, Task &t) -> int
	{
		static_cast<Encoder<B>&>(m).encode(static_cast<B*>(t[ps_U_K].get_dataptr()),
		                                   static_cast<B*>(t[ps_X_N].get_dataptr()));

		return status_t::SUCCESS;
	});

	std::iota(info_bits_pos.begin(), info_bits_pos.end(), 0);
}

template <typename B>
Encoder<B>* Encoder<B>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
int Encoder<B>
::get_K() const
{
	return this->K;
}

template <typename B>
int Encoder<B>
::get_N() const
{
	return this->N;
}

template <typename B>
bool Encoder<B>
::is_sys() const
{
	return this->sys;
}

template <typename B>
bool Encoder<B>
::is_memorizing() const
{
	return this->memorizing;
}

template <typename B>
void Encoder<B>
::set_memorizing(const bool memorizing)
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

template <typename B>
const std::vector<B>& Encoder<B>
::get_U_K(const int frame_id) const
{
	return this->U_K_mem[frame_id % this->n_frames];
}

template <typename B>
const std::vector<B>& Encoder<B>
::get_X_N(const int frame_id) const
{
	return this->X_N_mem[frame_id % this->n_frames];
}

template <typename B>
template <class A>
void Encoder<B>
::encode(const std::vector<B,A>& U_K, std::vector<B,A>& X_N, const int frame_id)
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

template <typename B>
void Encoder<B>
::encode(const B *U_K, B *X_N, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	if (this->is_memorizing())
		for (auto f = f_start; f < f_stop; f++)
			std::copy(U_K + (f +0) * this->K,
			          U_K + (f +1) * this->K,
			          U_K_mem[f].begin());

	if (frame_id < 0 || this->simd_inter_frame_level == 1)
	{
		const auto w_start = (frame_id < 0) ? 0 : frame_id % this->n_enc_waves;
		const auto w_stop  = (frame_id < 0) ? this->n_enc_waves : w_start +1;

		auto w = 0;
		for (w = w_start; w < w_stop -1; w++)
		{
			this->_encode(U_K + w * this->K * this->simd_inter_frame_level,
			              X_N + w * this->N * this->simd_inter_frame_level,
			              w * this->simd_inter_frame_level);
		}

		if (this->n_inter_frame_rest == 0)
		{
			this->_encode(U_K + w * this->K * this->simd_inter_frame_level,
			              X_N + w * this->N * this->simd_inter_frame_level,
			              w * this->simd_inter_frame_level);
		}
		else
		{
			const auto waves_off1 = w * this->simd_inter_frame_level * this->K;
			std::copy(U_K + waves_off1,
			          U_K + waves_off1 + this->n_inter_frame_rest * this->K,
			          this->U_K.begin());

			this->_encode(this->U_K.data(), this->X_N.data(), w * this->simd_inter_frame_level);

			const auto waves_off2 = w * this->simd_inter_frame_level * this->N;
			std::copy(this->X_N.begin(),
			          this->X_N.begin() + this->n_inter_frame_rest * this->N,
			          X_N + waves_off2);
		}
	}
	else
	{
		const auto w = (frame_id % this->n_frames) / this->simd_inter_frame_level;
		const auto w_pos = frame_id % this->simd_inter_frame_level;

		// std::fill(this->U_K.begin(), this->U_K.end(), (B)0);
		std::copy(U_K + ((frame_id % this->n_frames) + 0) * this->K,
		          U_K + ((frame_id % this->n_frames) + 1) * this->K,
		          this->U_K.begin() + w_pos * this->K);

		this->_encode(this->U_K.data(), this->X_N.data(), w * this->simd_inter_frame_level);

		std::copy(this->X_N.begin() + (w_pos +0) * this->N,
		          this->X_N.begin() + (w_pos +1) * this->N,
		          X_N + (frame_id % this->n_frames) * this->N);
	}

	if (this->is_memorizing())
		for (auto f = f_start; f < f_stop; f++)
			std::copy(X_N + (f +0) * this->N,
			          X_N + (f +1) * this->N,
			          X_N_mem[f].begin());
}

template <typename B>
template <class A>
bool Encoder<B>
::is_codeword(const std::vector<B,A>& X_N)
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

template <typename B>
bool Encoder<B>
::is_codeword(const B *X_N)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
const std::vector<uint32_t>& Encoder<B>
::get_info_bits_pos() const
{
	return info_bits_pos;
}

template <typename B>
int Encoder<B>
::tail_length() const
{
	return 0;
}

template <typename B>
void Encoder<B>
::_encode(const B *U_K, B *X_N, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void Encoder<B>
::set_sys(const bool sys)
{
	this->sys = sys;
}

template <typename B>
void Encoder<B>
::set_seed(const int seed)
{
	// do nothing in the general case, this method has to be overrided
}

template <typename B>
void Encoder<B>
::set_n_frames(const int n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);

		this->n_inter_frame_rest = n_frames % this->simd_inter_frame_level;
		this->n_enc_waves = (int)std::ceil((float)n_frames / (float)this->simd_inter_frame_level),

		this->U_K.resize(this->n_inter_frame_rest ? this->simd_inter_frame_level * this->K : 0);
		this->X_N.resize(this->n_inter_frame_rest ? this->simd_inter_frame_level * this->N : 0);

		const auto vec_size = this->U_K_mem[0].size();
		const auto old_U_K_mem_size = this->U_K_mem.size();
		const auto new_U_K_mem_size = (old_U_K_mem_size / old_n_frames) * n_frames;
		this->U_K_mem.resize(new_U_K_mem_size, std::vector<B>(vec_size));

		const auto vec_size2 = this->X_N_mem[0].size();
		const auto old_X_N_mem_size = this->X_N_mem.size();
		const auto new_X_N_mem_size = (old_X_N_mem_size / old_n_frames) * n_frames;
		this->X_N_mem.resize(new_X_N_mem_size, std::vector<B>(vec_size2));
	}
}

}
}
