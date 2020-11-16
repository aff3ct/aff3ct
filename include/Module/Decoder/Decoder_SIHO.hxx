#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Decoder/Decoder_SIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Decoder_SIHO<B,R>
::operator[](const dec::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename R>
Socket& Decoder_SIHO<B,R>
::operator[](const dec::sck::decode_hiho s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO<B,R>
::operator[](const dec::sck::decode_hiho_cw s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho_cw)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO<B,R>
::operator[](const dec::sck::decode_siho s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siho)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SIHO<B,R>
::operator[](const dec::sck::decode_siho_cw s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siho_cw)[(size_t)s];
}

template <typename B, typename R>
Decoder_SIHO<B,R>
::Decoder_SIHO(const int K, const int N)
: Decoder_HIHO<B>(K, N),
  Y_N(N * this->get_n_frames_per_wave())
{
	const std::string name = "Decoder_SIHO";
	this->set_name(name);

	auto &p1 = this->create_task("decode_siho", (int)dec::tsk::decode_siho);
	auto p1s_Y_N = this->template create_socket_in <R     >(p1, "Y_N", this->N);
	auto p1s_CWD = this->template create_socket_out<int8_t>(p1, "CWD",       1);
	auto p1s_V_K = this->template create_socket_out<B     >(p1, "V_K", this->K);
	this->create_codelet(p1, [p1s_Y_N, p1s_CWD, p1s_V_K](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_SIHO<B,R>&>(m);

		auto ret = dec._decode_siho(static_cast<R     *>(t[p1s_Y_N].get_dataptr()),
		                            static_cast<int8_t*>(t[p1s_CWD].get_dataptr()),
		                            static_cast<B     *>(t[p1s_V_K].get_dataptr()),
		                            frame_id);

		if (dec.is_auto_reset())
			dec._reset(frame_id);

		return ret;
	});
	this->register_timer(p1, "load");
	this->register_timer(p1, "decode");
	this->register_timer(p1, "store");
	this->register_timer(p1, "total");

	auto &p2 = this->create_task("decode_siho_cw", (int)dec::tsk::decode_siho_cw);
	auto p2s_Y_N = this->template create_socket_in <R     >(p2, "Y_N", this->N);
	auto p2s_CWD = this->template create_socket_out<int8_t>(p2, "CWD",       1);
	auto p2s_V_N = this->template create_socket_out<B     >(p2, "V_N", this->N);
	this->create_codelet(p2, [p2s_Y_N, p2s_CWD, p2s_V_N](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_SIHO<B,R>&>(m);

		auto ret = dec._decode_siho_cw(static_cast<R     *>(t[p2s_Y_N].get_dataptr()),
		                               static_cast<int8_t*>(t[p2s_CWD].get_dataptr()),
		                               static_cast<B     *>(t[p2s_V_N].get_dataptr()),
		                               frame_id);

		if (dec.is_auto_reset())
			dec._reset(frame_id);

		return ret;
	});
	this->register_timer(p2, "load");
	this->register_timer(p2, "decode");
	this->register_timer(p2, "store");
	this->register_timer(p2, "total");
}

template <typename B, typename R>
Decoder_SIHO<B,R>* Decoder_SIHO<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
template <class AR, class AB>
int Decoder_SIHO<B,R>
::decode_siho(const std::vector<R,AR>& Y_N, std::vector<int8_t,AB>& CWD, std::vector<B,AB>& V_K,
              const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_siho::CWD].bind(CWD);
	(*this)[dec::sck::decode_siho::V_K].bind(V_K);
	const auto &status = (*this)[dec::tsk::decode_siho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class AR, class AB>
int Decoder_SIHO<B,R>
::decode_siho(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho::Y_N].bind(Y_N      );
	(*this)[dec::sck::decode_siho::CWD].bind(this->CWD);
	(*this)[dec::sck::decode_siho::V_K].bind(V_K      );
	const auto &status = (*this)[dec::tsk::decode_siho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_siho::CWD].bind(CWD);
	(*this)[dec::sck::decode_siho::V_K].bind(V_K);
	const auto &status = (*this)[dec::tsk::decode_siho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::decode_siho(const R *Y_N, B *V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho::Y_N].bind(Y_N             );
	(*this)[dec::sck::decode_siho::CWD].bind(this->CWD.data());
	(*this)[dec::sck::decode_siho::V_K].bind(V_K             );
	const auto &status = (*this)[dec::tsk::decode_siho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class AR, class AB>
int Decoder_SIHO<B,R>
::decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<int8_t,AB>& CWD, std::vector<B,AB>& V_N,
                 const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho_cw::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_siho_cw::CWD].bind(CWD);
	(*this)[dec::sck::decode_siho_cw::V_N].bind(V_N);
	const auto &status = (*this)[dec::tsk::decode_siho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class AR, class AB>
int Decoder_SIHO<B,R>
::decode_siho_cw(const std::vector<R,AR>& Y_N, std::vector<B,AB>& V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho_cw::Y_N].bind(Y_N      );
	(*this)[dec::sck::decode_siho_cw::CWD].bind(this->CWD);
	(*this)[dec::sck::decode_siho_cw::V_N].bind(V_N      );
	const auto &status = (*this)[dec::tsk::decode_siho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho_cw::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_siho_cw::CWD].bind(CWD);
	(*this)[dec::sck::decode_siho_cw::V_N].bind(V_N);
	const auto &status = (*this)[dec::tsk::decode_siho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::decode_siho_cw(const R *Y_N, B *V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siho_cw::Y_N].bind(Y_N             );
	(*this)[dec::sck::decode_siho_cw::CWD].bind(this->CWD.data());
	(*this)[dec::sck::decode_siho_cw::V_N].bind(V_N             );
	const auto &status = (*this)[dec::tsk::decode_siho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_siho(const R *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
	const auto status = this->_decode_siho(Y_N, V_K, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_siho(const R *Y_N, B *V_K, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_siho_cw(const R *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
	const auto status = this->_decode_siho_cw(Y_N, V_N, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_siho_cw(const R *Y_N, B *V_N, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
	for (size_t i = 0; i < (size_t)this->N * this->get_n_frames_per_wave(); i++)
		this->Y_N[i] = Y_N[i] ? (R)-1 : (R)1;
	return this->_decode_siho(this->Y_N.data(), CWD, V_K, frame_id);
}

template <typename B, typename R>
int Decoder_SIHO<B,R>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
	for (size_t i = 0; i < (size_t)this->N * this->get_n_frames_per_wave(); i++)
		this->Y_N[i] = Y_N[i] ? (R)-1 : (R)1;
	return this->_decode_siho_cw(this->Y_N.data(), CWD, V_N, frame_id);
}

template <typename B, typename R>
void Decoder_SIHO<B,R>
::set_n_frames_per_wave(const size_t n_frames_per_wave)
{
	const auto old_n_frames_per_wave = this->get_n_frames_per_wave();
	if (old_n_frames_per_wave != n_frames_per_wave)
	{
		Decoder_HIHO<B>::set_n_frames_per_wave(n_frames_per_wave);
		this->Y_N.resize(this->N * n_frames_per_wave);
	}
}

}
}

