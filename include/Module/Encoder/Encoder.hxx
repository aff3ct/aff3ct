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
::Encoder(const int K, const int N)
: Module(),
  K(K),
  N(N),
  sys(true),
  memorizing(false),
  info_bits_pos(this->K),
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

	if (K > N)
	{
		std::stringstream message;
		message << "'K' has to be smaller or equal to 'N' ('K' = " << K << ", 'N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("encode");
	auto ps_U_K = this->template create_socket_in <B>(p, "U_K", this->K);
	auto ps_X_N = this->template create_socket_out<B>(p, "X_N", this->N);
	this->create_codelet(p, [ps_U_K, ps_X_N](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &enc = static_cast<Encoder<B>&>(m);
		if (enc.is_memorizing())
			for (size_t f = 0; f < enc.get_n_frames_per_wave(); f++)
				std::copy(static_cast<B*>(t[ps_U_K].get_dataptr()) + (f +0) * enc.K,
				          static_cast<B*>(t[ps_U_K].get_dataptr()) + (f +1) * enc.K,
				          enc.U_K_mem[frame_id + f].begin());

		enc._encode(static_cast<B*>(t[ps_U_K].get_dataptr()),
		            static_cast<B*>(t[ps_X_N].get_dataptr()),
		            frame_id);

		if (enc.is_memorizing())
			for (size_t f = 0; f < enc.get_n_frames_per_wave(); f++)
				std::copy(static_cast<B*>(t[ps_X_N].get_dataptr()) + (f +0) * enc.N,
				          static_cast<B*>(t[ps_X_N].get_dataptr()) + (f +1) * enc.N,
				          enc.X_N_mem[frame_id + f].begin());

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
			for (size_t f = 0; f < this->n_frames; f++)
			{
				this->U_K_mem[f].resize(this->K);
				std::fill(this->U_K_mem[f].begin(), this->U_K_mem[f].end(), (B)0);
			}
		}
		if (!this->X_N_mem[0].size())
		{
			for (size_t f = 0; f < this->n_frames; f++)
			{
				this->X_N_mem[f].resize(this->N);
				std::fill(this->X_N_mem[f].begin(), this->X_N_mem[f].end(), (B)0);
			}
		}
	}
}

template <typename B>
const std::vector<B>& Encoder<B>
::get_U_K(const size_t frame_id) const
{
	return this->U_K_mem[frame_id % this->n_frames];
}

template <typename B>
const std::vector<B>& Encoder<B>
::get_X_N(const size_t frame_id) const
{
	return this->X_N_mem[frame_id % this->n_frames];
}

template <typename B>
template <class A>
void Encoder<B>
::encode(const std::vector<B,A>& U_K, std::vector<B,A>& X_N, const int frame_id, const bool managed_memory)
{
	(*this)[enc::sck::encode::U_K].bind(U_K);
	(*this)[enc::sck::encode::X_N].bind(X_N);
	(*this)[enc::tsk::encode].exec(frame_id, managed_memory);
}

template <typename B>
void Encoder<B>
::encode(const B *U_K, B *X_N, const int frame_id, const bool managed_memory)
{
	(*this)[enc::sck::encode::U_K].bind(U_K);
	(*this)[enc::sck::encode::X_N].bind(X_N);
	(*this)[enc::tsk::encode].exec(frame_id, managed_memory);
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
::_encode(const B *U_K, B *X_N, const size_t frame_id)
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
::set_n_frames(const size_t n_frames)
{
	const auto old_n_frames = this->get_n_frames();
	if (old_n_frames != n_frames)
	{
		Module::set_n_frames(n_frames);

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
