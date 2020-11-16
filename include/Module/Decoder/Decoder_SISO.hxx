#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Decoder/Decoder_SISO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R>
Task& Decoder_SISO<B,R>
::operator[](const dec::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_hiho s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_hiho_cw s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho_cw)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_siho s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siho)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_siho_cw s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siho_cw)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_siso s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siso)[(size_t)s];
}

template <typename B, typename R>
Socket& Decoder_SISO<B,R>
::operator[](const dec::sck::decode_siso_alt s)
{
	return Module::operator[]((size_t)dec::tsk::decode_siso_alt)[(size_t)s];
}

template <typename B, typename R>
Decoder_SISO<B,R>
::Decoder_SISO(const int K, const int N)
: Decoder_SIHO<B,R>(K, N)
{
	const std::string name = "Decoder_SISO";
	this->set_name(name);

	auto &p1 = this->create_task("decode_siso", (int)dec::tsk::decode_siso);
	auto p1s_Y_N1 = this->template create_socket_in <R     >(p1, "Y_N1", this->N);
	auto p1s_CWD  = this->template create_socket_out<int8_t>(p1, "CWD",        1);
	auto p1s_Y_N2 = this->template create_socket_out<R     >(p1, "Y_N2", this->N);
	this->create_codelet(p1, [p1s_Y_N1, p1s_CWD, p1s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_SISO<B,R>&>(m);

		auto ret = dec._decode_siso(static_cast<R     *>(t[p1s_Y_N1].get_dataptr()),
		                            static_cast<int8_t*>(t[p1s_CWD ].get_dataptr()),
		                            static_cast<R     *>(t[p1s_Y_N2].get_dataptr()),
		                            frame_id);

		if (dec.is_auto_reset())
			dec._reset(frame_id);

		return ret;
	});

	auto &p2 = this->create_task("decode_siso_alt", (int)dec::tsk::decode_siso_alt);
	auto p2s_sys = this->template create_socket_in <R     >(p2, "sys", this->K + this->tail_length() / 2);
	auto p2s_par = this->template create_socket_in <R     >(p2, "par", this->K + this->tail_length() / 2);
	auto p2s_CWD = this->template create_socket_out<int8_t>(p2, "CWD",       1);
	auto p2s_ext = this->template create_socket_out<R     >(p2, "ext", this->K);
	this->create_codelet(p2, [p2s_sys, p2s_par, p2s_CWD, p2s_ext](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_SISO<B,R>&>(m);

		auto ret = dec._decode_siso_alt(static_cast<R     *>(t[p2s_sys].get_dataptr()),
		                                static_cast<R     *>(t[p2s_par].get_dataptr()),
		                                static_cast<int8_t*>(t[p2s_CWD].get_dataptr()),
		                                static_cast<R     *>(t[p2s_ext].get_dataptr()),
		                                frame_id);

		if (dec.is_auto_reset())
			dec._reset(frame_id);

		return ret;
	});
}

template <typename B, typename R>
Decoder_SISO<B,R>* Decoder_SISO<B,R>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
template <class A>
int Decoder_SISO<B,R>
::decode_siso(const std::vector<R,A> &Y_N1, std::vector<int8_t,A> &CWD, std::vector<R,A> &Y_N2,
              const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso::Y_N1].bind(Y_N1);
	(*this)[dec::sck::decode_siso::CWD ].bind(CWD );
	(*this)[dec::sck::decode_siso::Y_N2].bind(Y_N2);
	const auto &status = (*this)[dec::tsk::decode_siso].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class A>
int Decoder_SISO<B,R>
::decode_siso(const std::vector<R,A> &Y_N1, std::vector<R,A> &Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso::Y_N1].bind(Y_N1     );
	(*this)[dec::sck::decode_siso::CWD ].bind(this->CWD);
	(*this)[dec::sck::decode_siso::Y_N2].bind(Y_N2     );
	const auto &status = (*this)[dec::tsk::decode_siso].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso::Y_N1].bind(Y_N1);
	(*this)[dec::sck::decode_siso::CWD ].bind(CWD );
	(*this)[dec::sck::decode_siso::Y_N2].bind(Y_N2);
	const auto &status = (*this)[dec::tsk::decode_siso].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::decode_siso(const R *Y_N1, R *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso::Y_N1].bind(Y_N1            );
	(*this)[dec::sck::decode_siso::CWD ].bind(this->CWD.data());
	(*this)[dec::sck::decode_siso::Y_N2].bind(Y_N2            );
	const auto &status = (*this)[dec::tsk::decode_siso].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class A>
int Decoder_SISO<B,R>
::decode_siso_alt(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<int8_t,A> &CWD,
                  std::vector<R,A> &ext, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso_alt::sys].bind(sys);
	(*this)[dec::sck::decode_siso_alt::par].bind(par);
	(*this)[dec::sck::decode_siso_alt::CWD].bind(CWD);
	(*this)[dec::sck::decode_siso_alt::ext].bind(ext);
	const auto &status = (*this)[dec::tsk::decode_siso_alt].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
template <class A>
int Decoder_SISO<B,R>
::decode_siso_alt(const std::vector<R,A> &sys, const std::vector<R,A> &par, std::vector<R,A> &ext,
                  const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso_alt::sys].bind(sys      );
	(*this)[dec::sck::decode_siso_alt::par].bind(par      );
	(*this)[dec::sck::decode_siso_alt::CWD].bind(this->CWD);
	(*this)[dec::sck::decode_siso_alt::ext].bind(ext      );
	const auto &status = (*this)[dec::tsk::decode_siso_alt].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::decode_siso_alt(const R *sys, const R *par, int8_t *CWD, R *ext, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso_alt::sys].bind(sys);
	(*this)[dec::sck::decode_siso_alt::par].bind(par);
	(*this)[dec::sck::decode_siso_alt::CWD].bind(CWD);
	(*this)[dec::sck::decode_siso_alt::ext].bind(ext);
	const auto &status = (*this)[dec::tsk::decode_siso_alt].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::decode_siso_alt(const R *sys, const R *par, R *ext, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_siso_alt::sys].bind(sys             );
	(*this)[dec::sck::decode_siso_alt::par].bind(par             );
	(*this)[dec::sck::decode_siso_alt::CWD].bind(this->CWD.data());
	(*this)[dec::sck::decode_siso_alt::ext].bind(ext             );
	const auto &status = (*this)[dec::tsk::decode_siso_alt].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::tail_length() const
{
	return 0;
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::_decode_siso(const R *Y_N1, int8_t *CWD, R *Y_N2, const size_t frame_id)
{
	const auto status = this->_decode_siso(Y_N1, Y_N2, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::_decode_siso(const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::_decode_siso_alt(const R *sys, const R *par, int8_t *CWD, R *ext, const size_t frame_id)
{
	const auto status = this->_decode_siso_alt(sys, par, ext, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B, typename R>
int Decoder_SISO<B,R>
::_decode_siso_alt(const R *sys, const R *par, R *ext, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
