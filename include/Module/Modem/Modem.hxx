#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Modem/Modem.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename R, typename Q>
Task& Modem<B,R,Q>
::operator[](const mdm::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::modulate s)
{
	return Module::operator[]((size_t)mdm::tsk::modulate)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::tmodulate s)
{
	return Module::operator[]((size_t)mdm::tsk::tmodulate)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::filter s)
{
	return Module::operator[]((size_t)mdm::tsk::filter)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::demodulate s)
{
	return Module::operator[]((size_t)mdm::tsk::demodulate)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::tdemodulate s)
{
	return Module::operator[]((size_t)mdm::tsk::tdemodulate)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::demodulate_wg s)
{
	return Module::operator[]((size_t)mdm::tsk::demodulate_wg)[(size_t)s];
}

template <typename B, typename R, typename Q>
Socket& Modem<B,R,Q>
::operator[](const mdm::sck::tdemodulate_wg s)
{
	return Module::operator[]((size_t)mdm::tsk::tdemodulate_wg)[(size_t)s];
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>
::Modem(const int N, const int N_mod, const int N_fil)
: Module(),
  N(N),
  N_mod(N_mod),
  N_fil(N_fil),
  enable_filter(false),
  enable_demodulator(true),
  last_channel_param(std::numeric_limits<float>::max())
{
	const std::string name = "Modem";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_mod <= 0)
	{
		std::stringstream message;
		message << "'N_mod' has to be greater than 0 ('N_mod' = " << N_mod  << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_fil <= 0)
	{
		std::stringstream message;
		message << "'N_fil' has to be greater than 0 ('N_fil' = " << N_fil << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init_processes();
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>
::Modem(const int N, const int N_mod)
: Module(),
  N(N),
  N_mod(N_mod),
  N_fil(N_mod),
  enable_filter(false),
  enable_demodulator(true),
  last_channel_param(std::numeric_limits<float>::max())
{
	const std::string name = "Modem";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	if (N_mod <= 0)
	{
		std::stringstream message;
		message << "'N_mod' has to be greater than 0 ('N_mod' = " << N_mod  << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init_processes();
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>
::Modem(const int N)
: Module(),
  N(N),
  N_mod(N),
  N_fil(N),
  enable_filter(false),
  enable_demodulator(true),
  last_channel_param(std::numeric_limits<float>::max())
{
	const std::string name = "Modem";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->init_processes();
}

template <typename B, typename R, typename Q>
Modem<B,R,Q>* Modem<B,R,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::init_processes()
{
	auto &p1 = this->create_task("modulate");
	auto p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N    );
	auto p1s_X_N2 = this->template create_socket_out<R>(p1, "X_N2", this->N_mod);
	this->create_codelet(p1, [p1s_X_N1, p1s_X_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._modulate(static_cast<B*>(t[p1s_X_N1].get_dataptr()),
		              static_cast<R*>(t[p1s_X_N2].get_dataptr()),
		              frame_id);

		return status_t::SUCCESS;
	});

	auto &p7 = this->create_task("tmodulate");
	auto p7s_X_N1 = this->template create_socket_in <Q>(p7, "X_N1", this->N    );
	auto p7s_X_N2 = this->template create_socket_out<R>(p7, "X_N2", this->N_mod);
	this->create_codelet(p7, [p7s_X_N1, p7s_X_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._tmodulate(static_cast<Q*>(t[p7s_X_N1].get_dataptr()),
		               static_cast<R*>(t[p7s_X_N2].get_dataptr()),
		               frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("filter");
	auto p2s_CP   = this->template create_socket_in <float>(p2, "CP",             1);
	auto p2s_Y_N1 = this->template create_socket_in <R    >(p2, "Y_N1", this->N_mod);
	auto p2s_Y_N2 = this->template create_socket_out<R    >(p2, "Y_N2", this->N_fil);
	this->create_codelet(p2, [p2s_CP, p2s_Y_N1, p2s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._filter(static_cast<float*>(t[p2s_CP  ].get_dataptr()),
		            static_cast<R    *>(t[p2s_Y_N1].get_dataptr()),
		            static_cast<R    *>(t[p2s_Y_N2].get_dataptr()),
		            frame_id);

		mdm.last_channel_param = *static_cast<float*>(t[p2s_CP].get_dataptr());

		return status_t::SUCCESS;
	});

	auto &p3 = this->create_task("demodulate");
	auto p3s_CP   = this->template create_socket_in <float>(p3, "CP",             1);
	auto p3s_Y_N1 = this->template create_socket_in <Q    >(p3, "Y_N1", this->N_fil);
	auto p3s_Y_N2 = this->template create_socket_out<Q    >(p3, "Y_N2", this->N    );
	this->create_codelet(p3, [p3s_CP, p3s_Y_N1, p3s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._demodulate(static_cast<float*>(t[p3s_CP  ].get_dataptr()),
		                static_cast<Q    *>(t[p3s_Y_N1].get_dataptr()),
		                static_cast<Q    *>(t[p3s_Y_N2].get_dataptr()),
		                frame_id);

		mdm.last_channel_param = *static_cast<float*>(t[p3s_CP].get_dataptr());

		return status_t::SUCCESS;
	});

	auto &p4 = this->create_task("tdemodulate");
	auto p4s_CP   = this->template create_socket_in <float>(p4, "CP",             1);
	auto p4s_Y_N1 = this->template create_socket_in <Q    >(p4, "Y_N1", this->N_fil);
	auto p4s_Y_N2 = this->template create_socket_in <Q    >(p4, "Y_N2", this->N    );
	auto p4s_Y_N3 = this->template create_socket_out<Q    >(p4, "Y_N3", this->N    );
	this->create_codelet(p4, [p4s_CP, p4s_Y_N1, p4s_Y_N2, p4s_Y_N3](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._tdemodulate(static_cast<float*>(t[p4s_CP  ].get_dataptr()),
		                 static_cast<Q    *>(t[p4s_Y_N1].get_dataptr()),
		                 static_cast<Q    *>(t[p4s_Y_N2].get_dataptr()),
		                 static_cast<Q    *>(t[p4s_Y_N3].get_dataptr()),
		                 frame_id);

		mdm.last_channel_param = *static_cast<float*>(t[p4s_CP].get_dataptr());

		return status_t::SUCCESS;
	});

	auto &p5 = this->create_task("demodulate_wg");
	auto p5s_CP   = this->template create_socket_in <float>(p5, "CP",             1);
	auto p5s_H_N  = this->template create_socket_in <R    >(p5, "H_N",  this->N_fil);
	auto p5s_Y_N1 = this->template create_socket_in <Q    >(p5, "Y_N1", this->N_fil);
	auto p5s_Y_N2 = this->template create_socket_out<Q    >(p5, "Y_N2", this->N    );
	this->create_codelet(p5, [p5s_CP, p5s_H_N, p5s_Y_N1, p5s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._demodulate_wg(static_cast<float*>(t[p5s_CP  ].get_dataptr()),
		                   static_cast<R    *>(t[p5s_H_N ].get_dataptr()),
		                   static_cast<Q    *>(t[p5s_Y_N1].get_dataptr()),
		                   static_cast<Q    *>(t[p5s_Y_N2].get_dataptr()),
		                   frame_id);

		mdm.last_channel_param = *static_cast<float*>(t[p5s_CP].get_dataptr());

		return status_t::SUCCESS;
	});

	auto &p6 = this->create_task("tdemodulate_wg");
	auto p6s_CP   = this->template create_socket_in <float>(p6, "CP",             1);
	auto p6s_H_N  = this->template create_socket_in <R    >(p6, "H_N",  this->N_fil);
	auto p6s_Y_N1 = this->template create_socket_in <Q    >(p6, "Y_N1", this->N_fil);
	auto p6s_Y_N2 = this->template create_socket_in <Q    >(p6, "Y_N2", this->N    );
	auto p6s_Y_N3 = this->template create_socket_out<Q    >(p6, "Y_N3", this->N    );
	this->create_codelet(p6, [p6s_CP, p6s_H_N, p6s_Y_N1, p6s_Y_N2, p6s_Y_N3](Module &m, Task &t, const size_t frame_id)
		-> int
	{
		auto &mdm = static_cast<Modem<B,R,Q>&>(m);

		mdm._tdemodulate_wg(static_cast<float*>(t[p6s_CP  ].get_dataptr()),
		                    static_cast<R    *>(t[p6s_H_N ].get_dataptr()),
		                    static_cast<Q    *>(t[p6s_Y_N1].get_dataptr()),
		                    static_cast<Q    *>(t[p6s_Y_N2].get_dataptr()),
		                    static_cast<Q    *>(t[p6s_Y_N3].get_dataptr()),
		                    frame_id);

		mdm.last_channel_param = *static_cast<float*>(t[p6s_CP].get_dataptr());

		return status_t::SUCCESS;
	});
}

template <typename B, typename R, typename Q>
int Modem<B,R,Q>
::get_N() const
{
	return this->N;
}

template <typename B, typename R, typename Q>
int Modem<B,R,Q>
::get_N_mod() const
{
	return this->N_mod;
}

template <typename B, typename R, typename Q>
int Modem<B,R,Q>
::get_N_fil() const
{
	return this->N_fil;
}

template <typename B, typename R, typename Q>
bool Modem<B,R,Q>
::is_filter() const
{
	return this->enable_filter;
}

template <typename B, typename R, typename Q>
bool Modem<B,R,Q>
::is_demodulator() const
{
	return this->enable_demodulator;
}

template <typename B, typename R, typename Q>
template <class AB, class AR>
void Modem<B,R,Q>
::modulate(const std::vector<B,AB>& X_N1, std::vector<R,AR>& X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::modulate::X_N1].bind(X_N1);
	(*this)[mdm::sck::modulate::X_N2].bind(X_N2);
	(*this)[mdm::tsk::modulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::modulate(const B *X_N1, R *X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::modulate::X_N1].bind(X_N1);
	(*this)[mdm::sck::modulate::X_N2].bind(X_N2);
	(*this)[mdm::tsk::modulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::tmodulate(const std::vector<Q,AQ>& X_N1, std::vector<R,AR>& X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::tmodulate::X_N1].bind(X_N1);
	(*this)[mdm::sck::tmodulate::X_N2].bind(X_N2);
	(*this)[mdm::tsk::tmodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tmodulate(const Q *X_N1, R *X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::tmodulate::X_N1].bind(X_N1);
	(*this)[mdm::sck::tmodulate::X_N2].bind(X_N2);
	(*this)[mdm::tsk::tmodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::filter(const std::vector<float,A>& CP, const std::vector<R,A>& Y_N1, std::vector<R,A>& Y_N2, const int frame_id,
         const bool managed_memory)
{
	(*this)[mdm::sck::filter::CP  ].bind(CP  );
	(*this)[mdm::sck::filter::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::filter::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::filter].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::filter(const float *CP, const R *Y_N1, R *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::filter::CP  ].bind(CP  );
	(*this)[mdm::sck::filter::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::filter::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::filter].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::demodulate(const std::vector<float,A>& CP, const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2,
             const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::demodulate::CP  ].bind(CP  );
	(*this)[mdm::sck::demodulate::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::demodulate::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::demodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::demodulate::CP  ].bind(CP  );
	(*this)[mdm::sck::demodulate::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::demodulate::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::demodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::demodulate_wg(const std::vector<float,AR>& CP, const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1,
                std::vector<Q,AQ>& Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::demodulate_wg::CP  ].bind(CP  );
	(*this)[mdm::sck::demodulate_wg::H_N ].bind(H_N );
	(*this)[mdm::sck::demodulate_wg::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::demodulate_wg::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::demodulate_wg].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::demodulate_wg::CP  ].bind(CP  );
	(*this)[mdm::sck::demodulate_wg::H_N ].bind(H_N );
	(*this)[mdm::sck::demodulate_wg::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::demodulate_wg::Y_N2].bind(Y_N2);
	(*this)[mdm::tsk::demodulate_wg].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::tdemodulate(const std::vector<float,A>& CP, const std::vector<Q,A>& Y_N1, const std::vector<Q,A>& Y_N2,
              std::vector<Q,A>& Y_N3, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::tdemodulate::CP  ].bind(CP  );
	(*this)[mdm::sck::tdemodulate::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::tdemodulate::Y_N2].bind(Y_N2);
	(*this)[mdm::sck::tdemodulate::Y_N3].bind(Y_N3);
	(*this)[mdm::tsk::tdemodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::tdemodulate::CP  ].bind(CP  );
	(*this)[mdm::sck::tdemodulate::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::tdemodulate::Y_N2].bind(Y_N2);
	(*this)[mdm::sck::tdemodulate::Y_N3].bind(Y_N3);
	(*this)[mdm::tsk::tdemodulate].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::tdemodulate_wg(const std::vector<float,AR>& CP, const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1,
                 const std::vector<Q,AQ>& Y_N2, std::vector<Q,AQ>& Y_N3, const int frame_id, const bool managed_memory)
{
	(*this)[mdm::sck::tdemodulate_wg::CP  ].bind(CP  );
	(*this)[mdm::sck::tdemodulate_wg::H_N ].bind(H_N );
	(*this)[mdm::sck::tdemodulate_wg::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::tdemodulate_wg::Y_N2].bind(Y_N2);
	(*this)[mdm::sck::tdemodulate_wg::Y_N3].bind(Y_N3);
	(*this)[mdm::tsk::tdemodulate_wg].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id,
                 const bool managed_memory)
{
	(*this)[mdm::sck::tdemodulate_wg::CP  ].bind(CP  );
	(*this)[mdm::sck::tdemodulate_wg::H_N ].bind(H_N );
	(*this)[mdm::sck::tdemodulate_wg::Y_N1].bind(Y_N1);
	(*this)[mdm::sck::tdemodulate_wg::Y_N2].bind(Y_N2);
	(*this)[mdm::sck::tdemodulate_wg::Y_N3].bind(Y_N3);
	(*this)[mdm::tsk::tdemodulate_wg].exec(frame_id, managed_memory);
}

template <typename B, typename R, typename Q>
int Modem<B,R,Q>
::get_buffer_size_after_modulation(const int N, const int n_b_per_s, const int tl, const int s_factor, const bool complex)
{
	return ((int)(std::ceil((float)N / (float)n_b_per_s)) + tl) * s_factor * (complex ? 2 : 1);
}

template <typename B, typename R, typename Q>
int Modem<B,R,Q>
::get_buffer_size_after_filtering(const int N, const int n_b_per_s, const int tl, const int max_wa_id, const bool complex)
{
	return ((int)(std::ceil((float)N / (float)n_b_per_s)) + tl) * max_wa_id * (complex ? 2 : 1);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_modulate(const B *X_N1, R *X_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tmodulate(const Q *X_N1, R *X_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_filter(const float *CP, const R *Y_N1, R *Y_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_demodulate(const float *CP, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_demodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, Q *Y_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tdemodulate(const float *CP, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tdemodulate_wg(const float *CP, const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::set_filter(const bool filter)
{
	this->enable_filter = filter;
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::set_demodulator(const bool demodulator)
{
	this->enable_demodulator = demodulator;
}

}
}
