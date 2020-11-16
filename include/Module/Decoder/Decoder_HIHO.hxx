#include <string>
#include <limits>
#include <sstream>
#include <algorithm>

#include "Tools/Exception/exception.hpp"
#include "Module/Module.hpp"
#include "Module/Decoder/Decoder_HIHO.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Decoder_HIHO<B>
::operator[](const dec::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Decoder_HIHO<B>
::operator[](const dec::sck::decode_hiho s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho)[(size_t)s];
}

template <typename B>
Socket& Decoder_HIHO<B>
::operator[](const dec::sck::decode_hiho_cw s)
{
	return Module::operator[]((size_t)dec::tsk::decode_hiho_cw)[(size_t)s];
}

template <typename B>
Decoder_HIHO<B>
::Decoder_HIHO(const int K, const int N)
: Decoder(K, N)
{
	const std::string name = "Decoder_HIHO";
	this->set_name(name);

	auto &p1 = this->create_task("decode_hiho", (int)dec::tsk::decode_hiho);
	auto p1s_Y_N = this->template create_socket_in <B     >(p1, "Y_N", this->N);
	auto p1s_CWD = this->template create_socket_out<int8_t>(p1, "CWD",       1);
	auto p1s_V_K = this->template create_socket_out<B     >(p1, "V_K", this->K);
	this->create_codelet(p1, [p1s_Y_N, p1s_CWD, p1s_V_K](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_HIHO<B>&>(m);

		auto ret = dec._decode_hiho(static_cast<B     *>(t[p1s_Y_N].get_dataptr()),
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

	auto &p2 = this->create_task("decode_hiho_cw", (int)dec::tsk::decode_hiho_cw);
	auto p2s_Y_N = this->template create_socket_in <B     >(p2, "Y_N", this->N);
	auto p2s_CWD = this->template create_socket_out<int8_t>(p2, "CWD",       1);
	auto p2s_V_N = this->template create_socket_out<B     >(p2, "V_N", this->N);
	this->create_codelet(p2, [p2s_Y_N, p2s_CWD, p2s_V_N](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &dec = static_cast<Decoder_HIHO<B>&>(m);

		auto ret = dec._decode_hiho_cw(static_cast<B     *>(t[p2s_Y_N].get_dataptr()),
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
}

template <typename B>
Decoder_HIHO<B>* Decoder_HIHO<B>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
template <class A>
int Decoder_HIHO<B>
::decode_hiho(const std::vector<B,A>& Y_N, std::vector<int8_t,A>& CWD, std::vector<B,A>& V_K,
              const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_hiho::CWD].bind(CWD);
	(*this)[dec::sck::decode_hiho::V_K].bind(V_K);
	const auto &status = (*this)[dec::tsk::decode_hiho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
template <class A>
int Decoder_HIHO<B>
::decode_hiho(const std::vector<B,A>& Y_N, std::vector<B,A>& V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho::Y_N].bind(Y_N      );
	(*this)[dec::sck::decode_hiho::CWD].bind(this->CWD);
	(*this)[dec::sck::decode_hiho::V_K].bind(V_K      );
	const auto &status = (*this)[dec::tsk::decode_hiho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
int Decoder_HIHO<B>
::decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_hiho::CWD].bind(CWD);
	(*this)[dec::sck::decode_hiho::V_K].bind(V_K);
	const auto &status = (*this)[dec::tsk::decode_hiho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
int Decoder_HIHO<B>
::decode_hiho(const B *Y_N, B *V_K, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho::Y_N].bind(Y_N             );
	(*this)[dec::sck::decode_hiho::CWD].bind(this->CWD.data());
	(*this)[dec::sck::decode_hiho::V_K].bind(V_K             );
	const auto &status = (*this)[dec::tsk::decode_hiho].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
template <class A>
int Decoder_HIHO<B>
::decode_hiho_cw(const std::vector<B,A>& Y_N, std::vector<int8_t,A>& CWD, std::vector<B,A>& V_N,
                 const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho_cw::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_hiho_cw::CWD].bind(CWD);
	(*this)[dec::sck::decode_hiho_cw::V_N].bind(V_N);
	const auto &status = (*this)[dec::tsk::decode_hiho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
template <class A>
int Decoder_HIHO<B>
::decode_hiho_cw(const std::vector<B,A>& Y_N, std::vector<B,A>& V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho_cw::Y_N].bind(Y_N      );
	(*this)[dec::sck::decode_hiho_cw::CWD].bind(this->CWD);
	(*this)[dec::sck::decode_hiho_cw::V_N].bind(V_N      );
	const auto &status = (*this)[dec::tsk::decode_hiho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
int Decoder_HIHO<B>
::decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho_cw::Y_N].bind(Y_N);
	(*this)[dec::sck::decode_hiho_cw::CWD].bind(CWD);
	(*this)[dec::sck::decode_hiho_cw::V_N].bind(V_N);
	const auto &status = (*this)[dec::tsk::decode_hiho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
int Decoder_HIHO<B>
::decode_hiho_cw(const B *Y_N, B *V_N, const int frame_id, const bool managed_memory)
{
	(*this)[dec::sck::decode_hiho_cw::Y_N].bind(Y_N             );
	(*this)[dec::sck::decode_hiho_cw::CWD].bind(this->CWD.data());
	(*this)[dec::sck::decode_hiho_cw::V_N].bind(V_N             );
	const auto &status = (*this)[dec::tsk::decode_hiho_cw].exec(frame_id, managed_memory);

	return this->compute_status(status, frame_id);
}

template <typename B>
int Decoder_HIHO<B>
::_decode_hiho(const B *Y_N, int8_t *CWD, B *V_K, const size_t frame_id)
{
	const auto status = this->_decode_hiho(Y_N, V_K, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B>
int Decoder_HIHO<B>
::_decode_hiho(const B *Y_N, B *V_K, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
int Decoder_HIHO<B>
::_decode_hiho_cw(const B *Y_N, int8_t *CWD, B *V_N, const size_t frame_id)
{
	const auto status = this->_decode_hiho_cw(Y_N, V_N, frame_id);
	std::fill(CWD, CWD + this->get_n_frames_per_wave(), 0);
	return status;
}

template <typename B>
int Decoder_HIHO<B>
::_decode_hiho_cw(const B *Y_N, B *V_N, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
