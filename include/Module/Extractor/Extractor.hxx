#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Extractor/Extractor.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Task& Extractor<B,Q>
::operator[](const ext::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename Q>
Socket& Extractor<B,Q>
::operator[](const ext::sck::get_sys_llr s)
{
	return Module::operator[]((size_t)ext::tsk::get_sys_llr)[(size_t)s];
}

template <typename B, typename Q>
Socket& Extractor<B,Q>
::operator[](const ext::sck::get_sys_bit s)
{
	return Module::operator[]((size_t)ext::tsk::get_sys_bit)[(size_t)s];
}

template <typename B, typename Q>
Socket& Extractor<B,Q>
::operator[](const ext::sck::get_sys_and_par_llr s)
{
	return Module::operator[]((size_t)ext::tsk::get_sys_and_par_llr)[(size_t)s];
}

template <typename B, typename Q>
Socket& Extractor<B,Q>
::operator[](const ext::sck::add_sys_and_ext_llr s)
{
	return Module::operator[]((size_t)ext::tsk::add_sys_and_ext_llr)[(size_t)s];
}

template <typename B, typename Q>
Extractor<B,Q>
::Extractor(const int K, const int N, const int tail_length)
: Module(),
  K(K), N(N), tail_length(tail_length)
{
	const std::string name = "Extractor";
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

	if (tail_length < 0)
	{
		std::stringstream message;
		message << "'tail_length' has to be greater or equal to 0 ('tail_length' = " << tail_length << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p1 = this->create_task("get_sys_llr");
	auto p1s_Y_N = this->template create_socket_in <Q>(p1, "Y_N", this->N);
	auto p1s_Y_K = this->template create_socket_out<Q>(p1, "Y_K", this->K);
	this->create_codelet(p1, [p1s_Y_N, p1s_Y_K](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ext = static_cast<Extractor<B,Q>&>(m);

		ext._get_sys_llr(static_cast<Q*>(t[p1s_Y_N].get_dataptr()),
		                 static_cast<Q*>(t[p1s_Y_K].get_dataptr()),
		                 frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("get_sys_bit");
	auto p2s_Y_N = this->template create_socket_in <Q>(p2, "Y_N", this->N);
	auto p2s_V_K = this->template create_socket_out<B>(p2, "V_K", this->K);
	this->create_codelet(p2, [p2s_Y_N, p2s_V_K](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ext = static_cast<Extractor<B,Q>&>(m);

		ext._get_sys_bit(static_cast<Q*>(t[p2s_Y_N].get_dataptr()),
		                 static_cast<B*>(t[p2s_V_K].get_dataptr()),
		                 frame_id);

		return status_t::SUCCESS;
	});

	const auto tb_2 = this->tail_length / 2;
	auto &p3 = this->create_task("get_sys_and_par_llr");
	auto p3s_Y_N = this->template create_socket_in <Q>(p3, "Y_N", this->N                 );
	auto p3s_sys = this->template create_socket_out<Q>(p3, "sys", this->K           + tb_2);
	auto p3s_par = this->template create_socket_out<Q>(p3, "par", this->N - this->K - tb_2);
	this->create_codelet(p3, [&p3s_Y_N, &p3s_sys, &p3s_par](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ext = static_cast<Extractor<B,Q>&>(m);

		ext._get_sys_and_par_llr(static_cast<Q*>(t[p3s_Y_N].get_dataptr()),
		                         static_cast<Q*>(t[p3s_sys].get_dataptr()),
		                         static_cast<Q*>(t[p3s_par].get_dataptr()),
		                         frame_id);

		return status_t::SUCCESS;
	});

	auto &p4 = this->create_task("add_sys_and_ext_llr");
	auto p4s_ext  = this->template create_socket_in <Q>(p4, "ext",  this->K);
	auto p4s_Y_N1 = this->template create_socket_in <Q>(p4, "Y_N1", this->N);
	auto p4s_Y_N2 = this->template create_socket_out<Q>(p4, "Y_N2", this->N);
	this->create_codelet(p4, [p4s_ext, p4s_Y_N1, p4s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &ext = static_cast<Extractor<B,Q>&>(m);

		ext._add_sys_and_ext_llr(static_cast<Q*>(t[p4s_ext ].get_dataptr()),
		                         static_cast<Q*>(t[p4s_Y_N1].get_dataptr()),
		                         static_cast<Q*>(t[p4s_Y_N2].get_dataptr()),
		                         frame_id);

		return status_t::SUCCESS;
	});
}

template <typename B, typename Q>
Extractor<B,Q>* Extractor<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
template <class A>
void Extractor<B,Q>
::get_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::get_sys_llr::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_llr::Y_K].bind(Y_K);
	(*this)[ext::tsk::get_sys_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::get_sys_llr::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_llr::Y_K].bind(Y_K);
	(*this)[ext::tsk::get_sys_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
template <class AQ, class AB>
void Extractor<B,Q>
::get_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::get_sys_bit::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_bit::V_K].bind(V_K);
	(*this)[ext::tsk::get_sys_bit].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_bit(const Q *Y_N, B *V_K, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::get_sys_bit::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_bit::V_K].bind(V_K);
	(*this)[ext::tsk::get_sys_bit].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
template <class A>
void Extractor<B,Q>
::get_sys_and_par_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par, const int frame_id,
                      const bool managed_memory)
{
	(*this)[ext::sck::get_sys_and_par_llr::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_and_par_llr::sys].bind(sys);
	(*this)[ext::sck::get_sys_and_par_llr::par].bind(par);
	(*this)[ext::tsk::get_sys_and_par_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::get_sys_and_par_llr::Y_N].bind(Y_N);
	(*this)[ext::sck::get_sys_and_par_llr::sys].bind(sys);
	(*this)[ext::sck::get_sys_and_par_llr::par].bind(par);
	(*this)[ext::tsk::get_sys_and_par_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
template <class A>
void Extractor<B,Q>
::add_sys_and_ext_llr(const std::vector<Q,A> &ext, const std::vector<Q,A> &Y_N1, std::vector<Q,A> &Y_N2,
                      const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::add_sys_and_ext_llr::ext ].bind(ext);
	(*this)[ext::sck::add_sys_and_ext_llr::Y_N1].bind(Y_N1);
	(*this)[ext::sck::add_sys_and_ext_llr::Y_N2].bind(Y_N2);
	(*this)[ext::tsk::add_sys_and_ext_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Extractor<B,Q>
::add_sys_and_ext_llr(const Q *ext, const Q *Y_N1, Q *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[ext::sck::add_sys_and_ext_llr::ext ].bind(ext);
	(*this)[ext::sck::add_sys_and_ext_llr::Y_N1].bind(Y_N1);
	(*this)[ext::sck::add_sys_and_ext_llr::Y_N2].bind(Y_N2);
	(*this)[ext::tsk::add_sys_and_ext_llr].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Extractor<B,Q>
::_get_sys_llr(const Q *Y_N, Q *Y_K, const size_t frame_id)
{
	const auto &info_bits_pos = this->get_info_bits_pos();
	if (info_bits_pos.size() != (size_t)this->K)
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto i = 0; i < this->K; i++)
		Y_K[i] = Y_N[info_bits_pos[i]];
}

template <typename B, typename Q>
void Extractor<B,Q>
::_get_sys_bit(const Q *Y_N, B *V_K, const size_t frame_id)
{
	const auto &info_bits_pos = this->get_info_bits_pos();
	if (info_bits_pos.size() != (size_t)this->K)
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto i = 0; i < this->K; i++)
		V_K[i] = Y_N[info_bits_pos[i]] >= 0 ? (B)0 : (B)1;
}

template <typename B, typename Q>
void Extractor<B,Q>
::_get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const size_t frame_id)
{
	const auto &info_bits_pos = this->get_info_bits_pos();
	if (info_bits_pos.size() != (size_t)this->K)
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	const auto K = this->K;
	const auto N = this->N;

	for (auto i = 0; i < K; i++)
		sys[i] = Y_N[info_bits_pos[i]];

	auto sys_idx = 0;
	for (auto i = 0; i < N; i++)
		if (std::find(info_bits_pos.begin(), info_bits_pos.end(), i) != info_bits_pos.end())
		{
			par[sys_idx] = Y_N[i];
			sys_idx++;
		}
}

template <typename B, typename Q>
void Extractor<B,Q>
::_add_sys_and_ext_llr(const Q *ext, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	const auto &info_bits_pos = this->get_info_bits_pos();
	if (info_bits_pos.size() != (size_t)this->K)
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	std::copy(Y_N1, Y_N1 + this->N, Y_N2);
	for (auto i = 0; i < K; i++)
		Y_N2[info_bits_pos[i]] += ext[i];
}

}
}