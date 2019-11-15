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
::Modem(const int N, const int N_mod, const int N_fil, const int n_frames)
: Module(n_frames), N(N), N_mod(N_mod), N_fil(N_fil), noise(nullptr), enable_filter(false), enable_demodulator(true)
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
::Modem(const int N, const int N_mod, const int n_frames)
: Module(n_frames), N(N), N_mod(N_mod), N_fil(N_mod), noise(nullptr), enable_filter(false), enable_demodulator(true)
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
::Modem(const int N, const int n_frames)
: Module(n_frames), N(N), N_mod(N), N_fil(N), noise(nullptr), enable_filter(false), enable_demodulator(true)
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
void Modem<B,R,Q>
::init_processes()
{
	auto &p1 = this->create_task("modulate");
	auto p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N    );
	auto p1s_X_N2 = this->template create_socket_out<R>(p1, "X_N2", this->N_mod);
	this->create_codelet(p1, [this, p1s_X_N1, p1s_X_N2](Task &t) -> int
	{
		this->modulate(static_cast<B*>(t[p1s_X_N1].get_dataptr()),
		               static_cast<R*>(t[p1s_X_N2].get_dataptr()));

		return 0;
	});

	auto &p7 = this->create_task("tmodulate");
	auto p7s_X_N1 = this->template create_socket_in <Q>(p7, "X_N1", this->N    );
	auto p7s_X_N2 = this->template create_socket_out<R>(p7, "X_N2", this->N_mod);
	this->create_codelet(p7, [this, p7s_X_N1, p7s_X_N2](Task &t) -> int
	{
		this->tmodulate(static_cast<Q*>(t[p7s_X_N1].get_dataptr()),
		                static_cast<R*>(t[p7s_X_N2].get_dataptr()));

		return 0;
	});

	auto &p2 = this->create_task("filter");
	auto p2s_Y_N1 = this->template create_socket_in <R>(p2, "Y_N1", this->N_mod);
	auto p2s_Y_N2 = this->template create_socket_out<R>(p2, "Y_N2", this->N_fil);
	this->create_codelet(p2, [this, p2s_Y_N1, p2s_Y_N2](Task &t) -> int
	{
		this->filter(static_cast<R*>(t[p2s_Y_N1].get_dataptr()),
		             static_cast<R*>(t[p2s_Y_N2].get_dataptr()));

		return 0;
	});

	auto &p3 = this->create_task("demodulate");
	auto p3s_Y_N1 = this->template create_socket_in <Q>(p3, "Y_N1", this->N_fil);
	auto p3s_Y_N2 = this->template create_socket_out<Q>(p3, "Y_N2", this->N    );
	this->create_codelet(p3, [this, p3s_Y_N1, p3s_Y_N2](Task &t) -> int
	{
		this->demodulate(static_cast<Q*>(t[p3s_Y_N1].get_dataptr()),
		                 static_cast<Q*>(t[p3s_Y_N2].get_dataptr()));

		return 0;
	});

	auto &p4 = this->create_task("tdemodulate");
	auto p4s_Y_N1 = this->template create_socket_in <Q>(p4, "Y_N1", this->N_fil);
	auto p4s_Y_N2 = this->template create_socket_in <Q>(p4, "Y_N2", this->N    );
	auto p4s_Y_N3 = this->template create_socket_out<Q>(p4, "Y_N3", this->N    );
	this->create_codelet(p4, [this, p4s_Y_N1, p4s_Y_N2, p4s_Y_N3](Task &t) -> int
	{
		this->tdemodulate(static_cast<Q*>(t[p4s_Y_N1].get_dataptr()),
		                  static_cast<Q*>(t[p4s_Y_N2].get_dataptr()),
		                  static_cast<Q*>(t[p4s_Y_N3].get_dataptr()));

		return 0;
	});

	auto &p5 = this->create_task("demodulate_wg");
	auto p5s_H_N  = this->template create_socket_in <R>(p5, "H_N",  this->N_fil);
	auto p5s_Y_N1 = this->template create_socket_in <Q>(p5, "Y_N1", this->N_fil);
	auto p5s_Y_N2 = this->template create_socket_out<Q>(p5, "Y_N2", this->N    );
	this->create_codelet(p5, [this, p5s_H_N, p5s_Y_N1, p5s_Y_N2](Task &t) -> int
	{
		this->demodulate_wg(static_cast<R*>(t[p5s_H_N ].get_dataptr()),
		                    static_cast<Q*>(t[p5s_Y_N1].get_dataptr()),
		                    static_cast<Q*>(t[p5s_Y_N2].get_dataptr()));

		return 0;
	});

	auto &p6 = this->create_task("tdemodulate_wg");
	auto p6s_H_N  = this->template create_socket_in <R>(p6, "H_N",  this->N_fil);
	auto p6s_Y_N1 = this->template create_socket_in <Q>(p6, "Y_N1", this->N_fil);
	auto p6s_Y_N2 = this->template create_socket_in <Q>(p6, "Y_N2", this->N    );
	auto p6s_Y_N3 = this->template create_socket_out<Q>(p6, "Y_N3", this->N    );
	this->create_codelet(p6, [this, p6s_H_N, p6s_Y_N1, p6s_Y_N2, p6s_Y_N3](Task &t) -> int
	{
		this->tdemodulate_wg(static_cast<R*>(t[p6s_H_N ].get_dataptr()),
		                     static_cast<Q*>(t[p6s_Y_N1].get_dataptr()),
		                     static_cast<Q*>(t[p6s_Y_N2].get_dataptr()),
		                     static_cast<Q*>(t[p6s_Y_N3].get_dataptr()));

		return 0;
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
void Modem<B,R,Q>
::set_noise(const tools::Noise<>& noise)
{
	this->noise = &noise;
	if (this->noise->is_set())
		this->noise_changed();
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::noise_changed()
{
}

template <typename B, typename R, typename Q>
const tools::Noise<>* Modem<B,R,Q>
::get_noise() const
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}

	return this->noise;
}

template <typename B, typename R, typename Q>
template <class AB, class AR>
void Modem<B,R,Q>
::modulate(const std::vector<B,AB>& X_N1, std::vector<R,AR>& X_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)X_N1.size())
	{
		std::stringstream message;
		message << "'X_N1.size()' has to be equal to 'N' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_mod * this->n_frames != (int)X_N2.size())
	{
		std::stringstream message;
		message << "'X_N2.size()' has to be equal to 'N_mod' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
		        << ", 'N_mod' = " << this->N_mod << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->modulate(X_N1.data(), X_N2.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_modulate(X_N1 + f * this->N,
		                X_N2 + f * this->N_mod,
		                f);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::tmodulate(const std::vector<Q,AQ>& X_N1, std::vector<R,AR>& X_N2, const int frame_id)
{
	if (this->N * this->n_frames != (int)X_N1.size())
	{
		std::stringstream message;
		message << "'X_N1.size()' has to be equal to 'N' * 'n_frames' ('X_N1.size()' = " << X_N1.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_mod * this->n_frames != (int)X_N2.size())
	{
		std::stringstream message;
		message << "'X_N2.size()' has to be equal to 'N_mod' * 'n_frames' ('X_N2.size()' = " << X_N2.size()
		        << ", 'N_mod' = " << this->N_mod << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->tmodulate(X_N1.data(), X_N2.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tmodulate(const Q *X_N1, R *X_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_tmodulate(X_N1 + f * this->N,
		                 X_N2 + f * this->N_mod,
		                 f);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::filter(const std::vector<R,A>& Y_N1, std::vector<R,A>& Y_N2, const int frame_id)
{
	if (this->N_mod * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N_mod' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N_mod' = " << this->N_mod << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_fil * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->filter(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_filter(Y_N1 + f * this->N_mod,
		              Y_N2 + f * this->N_fil,
		              f);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::demodulate(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2, const int frame_id)
{
	if (this->N_fil * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->demodulate(Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_demodulate(Y_N1 + f * this->N_fil,
		                  Y_N2 + f * this->N,
		                  f);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::demodulate_wg(const std::vector<R,AR>& H_N, const std::vector<Q,AQ>& Y_N1, std::vector<Q,AQ>& Y_N2, const int frame_id)
{
	if (this->N_fil * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_fil * this->n_frames != (int)H_N.size())
	{
		std::stringstream message;
		message << "'H_N.size()' has to be equal to 'N_fil' * 'n_frames' ('H_N.size()' = " << H_N.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->demodulate_wg(H_N.data(), Y_N1.data(), Y_N2.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_demodulate_wg(H_N  + f * this->N_fil,
		                     Y_N1 + f * this->N_fil,
		                     Y_N2 + f * this->N,
		                     f);
}

template <typename B, typename R, typename Q>
template <class A>
void Modem<B,R,Q>
::tdemodulate(const std::vector<Q,A>& Y_N1, const std::vector<Q,A>& Y_N2, std::vector<Q,A>& Y_N3, const int frame_id)
{
	if (this->N_fil * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N3.size())
	{
		std::stringstream message;
		message << "'Y_N3.size()' has to be equal to 'N' * 'n_frames' ('Y_N3.size()' = " << Y_N3.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ".";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->tdemodulate(Y_N1.data(), Y_N2.data(), Y_N3.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_tdemodulate(Y_N1 + f * this->N_fil,
		                   Y_N2 + f * this->N,
		                   Y_N3 + f * this->N,
		                   f);
}

template <typename B, typename R, typename Q>
template <class AQ, class AR>
void Modem<B,R,Q>
::tdemodulate_wg(const std::vector<R,AR>& H_N,  const std::vector<Q,AQ>& Y_N1,
                 const std::vector<Q,AQ>& Y_N2,       std::vector<Q,AQ>& Y_N3,
                 const int frame_id)
{
	if (this->N_fil * this->n_frames != (int)Y_N1.size())
	{
		std::stringstream message;
		message << "'Y_N1.size()' has to be equal to 'N_fil' * 'n_frames' ('Y_N1.size()' = " << Y_N1.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N_fil * this->n_frames != (int)H_N.size())
	{
		std::stringstream message;
		message << "'H_N.size()' has to be equal to 'N_fil' * 'n_frames' ('H_N.size()' = " << H_N.size()
		        << ", 'N_fil' = " << this->N_fil << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N2.size())
	{
		std::stringstream message;
		message << "'Y_N2.size()' has to be equal to 'N' * 'n_frames' ('Y_N2.size()' = " << Y_N2.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	if (this->N * this->n_frames != (int)Y_N3.size())
	{
		std::stringstream message;
		message << "'Y_N3.size()' has to be equal to 'N' * 'n_frames' ('Y_N3.size()' = " << Y_N3.size()
		        << ", 'N' = " << this->N << ", 'n_frames' = " << this->n_frames << ").";
		throw tools::length_error(__FILE__, __LINE__, __func__, message.str());
	}

	this->tdemodulate_wg(H_N.data(), Y_N1.data(), Y_N2.data(), Y_N3.data(), frame_id);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	const auto f_start = (frame_id < 0) ? 0 : frame_id % this->n_frames;
	const auto f_stop  = (frame_id < 0) ? this->n_frames : f_start +1;

	for (auto f = f_start; f < f_stop; f++)
		this->_tdemodulate_wg(H_N  + f * this->N_fil,
		                      Y_N1 + f * this->N_fil,
		                      Y_N2 + f * this->N,
		                      Y_N3 + f * this->N,
		                      f);
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
::_modulate(const B *X_N1, R *X_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tmodulate(const Q *X_N1, R *X_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_filter(const R *Y_N1, R *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_demodulate(const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_demodulate_wg(const R *H_N, const Q *Y_N1, Q *Y_N2, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tdemodulate(const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::_tdemodulate_wg(const R *H_N, const Q *Y_N1, const Q *Y_N2, Q *Y_N3, const int frame_id)
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

template <typename B, typename R, typename Q>
void Modem<B,R,Q>
::check_noise()
{
	if (this->noise == nullptr)
	{
		std::stringstream message;
		message << "'noise' should not be nullptr.";
		throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
	}
}

}
}
