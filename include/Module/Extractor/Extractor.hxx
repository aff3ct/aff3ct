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
::Extractor(const int K, const int N, const int tail_length, const int n_frames)
: Module(n_frames),
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
	this->create_codelet(p1, [p1s_Y_N, p1s_Y_K](Module &m, Task &t) -> int
	{
		static_cast<Extractor<B,Q>&>(m).get_sys_llr(static_cast<Q*>(t[p1s_Y_N].get_dataptr()),
		                                            static_cast<Q*>(t[p1s_Y_K].get_dataptr()));

		return status_t::OK;
	});

	auto &p2 = this->create_task("get_sys_bit");
	auto p2s_Y_N = this->template create_socket_in <Q>(p2, "Y_N", this->N);
	auto p2s_V_K = this->template create_socket_out<B>(p2, "V_K", this->K);
	this->create_codelet(p2, [p2s_Y_N, p2s_V_K](Module &m, Task &t) -> int
	{
		static_cast<Extractor<B,Q>&>(m).get_sys_bit(static_cast<Q*>(t[p2s_Y_N].get_dataptr()),
		                                            static_cast<B*>(t[p2s_V_K].get_dataptr()));

		return status_t::OK;
	});

	const auto tb_2 = this->tail_length / 2;
	auto &p3 = this->create_task("get_sys_and_par_llr");
	auto p3s_Y_N = this->template create_socket_in <Q>(p3, "Y_N", this->N                 );
	auto p3s_sys = this->template create_socket_out<Q>(p3, "sys", this->K           + tb_2);
	auto p3s_par = this->template create_socket_out<Q>(p3, "par", this->N - this->K - tb_2);
	this->create_codelet(p3, [&p3s_Y_N, &p3s_sys, &p3s_par](Module &m, Task &t) -> int
	{
		static_cast<Extractor<B,Q>&>(m).get_sys_and_par_llr(static_cast<Q*>(t[p3s_Y_N].get_dataptr()),
		                                                    static_cast<Q*>(t[p3s_sys].get_dataptr()),
		                                                    static_cast<Q*>(t[p3s_par].get_dataptr()));

		return status_t::OK;
	});

	auto &p4 = this->create_task("add_sys_and_ext_llr");
	auto p4s_ext = this->template create_socket_in    <Q>(p4, "ext", this->K);
	auto p4s_Y_N = this->template create_socket_in_out<Q>(p4, "Y_N", this->N);
	this->create_codelet(p4, [p4s_ext, p4s_Y_N](Module &m, Task &t) -> int
	{
		static_cast<Extractor<B,Q>&>(m).add_sys_and_ext_llr(static_cast<Q*>(t[p4s_ext].get_dataptr()),
		                                                    static_cast<Q*>(t[p4s_Y_N].get_dataptr()));

		return status_t::OK;
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
::get_sys_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &Y_K, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
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

	this->get_sys_llr(Y_N.data(), Y_K.data(), frame_id);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_get_sys_llr(Y_N + f * this->N,
		                   Y_K + f * this->K,
		                   f);
}

template <typename B, typename Q>
template <class AQ, class AB>
void Extractor<B,Q>
::get_sys_bit(const std::vector<Q,AQ> &Y_N, std::vector<B,AB> &V_K, const int frame_id)
{
	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
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

	this->get_sys_bit(Y_N.data(), V_K.data(), frame_id);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_get_sys_bit(Y_N + f * this->N,
		                   V_K + f * this->K,
		                   f);
}

template <typename B, typename Q>
template <class A>
void Extractor<B,Q>
::get_sys_and_par_llr(const std::vector<Q,A> &Y_N, std::vector<Q,A> &sys, std::vector<Q,A> &par, const int frame_id)
{
	const auto tb_2 = this->tail_length / 2;

	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->K + tb_2) * this->n_frames != (int)sys.size())
	{
		std::stringstream message;
		message << "'sys.size()' has to be equal to ('K' + 'tb_2') * 'n_frames' ('sys.size()' = " << sys.size()
		        << ", 'K' = " << this->K << ", 'tb_2' = " << tb_2 << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if ((this->N - this->K - tb_2) * this->n_frames != (int)par.size())
	{
		std::stringstream message;
		message << "'par.size()' has to be equal to ('N' - 'K' - 'tb_2') * 'n_frames' ('par.size()' = "
		        << par.size() << ", 'N' = " << this->N << ", 'K' = " << this->K << ", 'tb_2' = " << tb_2
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

	this->get_sys_and_par_llr(Y_N.data(), sys.data(), par.data(), frame_id);
}

template <typename B, typename Q>
void Extractor<B,Q>
::get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id)
{
	const auto tb_2 = this->tail_length / 2;

	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_get_sys_and_par_llr(Y_N + f *  this->N,
		                           sys + f * (this->K           + tb_2),
		                           par + f * (this->N - this->K - tb_2),
		                           f);
}

template <typename B, typename Q>
template <class A>
void Extractor<B,Q>
::add_sys_and_ext_llr(const std::vector<Q,A> &ext, std::vector<Q,A> &Y_N, const int frame_id)
{
	if (this->K * this->n_frames != (int)ext.size())
	{
		std::stringstream message;
		message << "'ext.size()' has to be equal to 'K' * 'n_frames' ('ext.size()' = " << ext.size()
		        << ", 'K' = " << this->K << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N.size())
	{
		std::stringstream message;
		message << "'Y_N.size()' has to be equal to 'N' * 'n_frames' ('Y_N.size()' = " << Y_N.size()
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

	this->add_sys_and_ext_llr(ext.data(), Y_N.data(), frame_id);
}

template <typename B, typename Q>
void Extractor<B,Q>
::add_sys_and_ext_llr(const Q *ext, Q *Y_N, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_add_sys_and_ext_llr(ext + f * this->K,
		                           Y_N + f * this->N,
		                           f);
}

template <typename B, typename Q>
void Extractor<B,Q>
::_get_sys_llr(const Q *Y_N, Q *Y_K, const int frame_id)
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
::_get_sys_bit(const Q *Y_N, B *V_K, const int frame_id)
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
::_get_sys_and_par_llr(const Q *Y_N, Q *sys, Q *par, const int frame_id)
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
::_add_sys_and_ext_llr(const Q *ext, Q *Y_N, const int frame_id)
{
	const auto &info_bits_pos = this->get_info_bits_pos();
	if (info_bits_pos.size() != (size_t)this->K)
	{
		std::stringstream message;
		message << "'info_bits_pos.size()' has to be equal to 'K' ('info_bits_pos.size()' = " << info_bits_pos.size()
		        << ", 'K' = " << this->K << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	for (auto i = 0; i < K; i++)
		Y_N[info_bits_pos[i]] += ext[i];
}

}
}