#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Puncturer/Puncturer.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename Q>
Task& Puncturer<B,Q>
::operator[](const pct::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename Q>
Socket& Puncturer<B,Q>
::operator[](const pct::sck::puncture s)
{
	return Module::operator[]((size_t)pct::tsk::puncture)[(size_t)s];
}

template <typename B, typename Q>
Socket& Puncturer<B,Q>
::operator[](const pct::sck::depuncture s)
{
	return Module::operator[]((size_t)pct::tsk::depuncture)[(size_t)s];
}

template <typename B, typename Q>
Puncturer<B,Q>
::Puncturer(const int K, const int N, const int N_cw)
: Module(), K(K), N(N), N_cw(N_cw)
{
	const std::string name = "Puncturer";
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

	auto &p1 = this->create_task("puncture");
	auto p1s_X_N1 = this->template create_socket_in <B>(p1, "X_N1", this->N_cw);
	auto p1s_X_N2 = this->template create_socket_out<B>(p1, "X_N2", this->N   );
	this->create_codelet(p1, [p1s_X_N1, p1s_X_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &pct = static_cast<Puncturer<B,Q>&>(m);

		pct._puncture(static_cast<B*>(t[p1s_X_N1].get_dataptr()),
		              static_cast<B*>(t[p1s_X_N2].get_dataptr()),
		              frame_id);

		return status_t::SUCCESS;
	});

	auto &p2 = this->create_task("depuncture");
	auto p2s_Y_N1 = this->template create_socket_in <Q>(p2, "Y_N1", this->N   );
	auto p2s_Y_N2 = this->template create_socket_out<Q>(p2, "Y_N2", this->N_cw);
	this->create_codelet(p2, [p2s_Y_N1, p2s_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &pct = static_cast<Puncturer<B,Q>&>(m);

		pct._depuncture(static_cast<Q*>(t[p2s_Y_N1].get_dataptr()),
		                static_cast<Q*>(t[p2s_Y_N2].get_dataptr()),
		                frame_id);

		return status_t::SUCCESS;
	});
}

template <typename B, typename Q>
Puncturer<B,Q>* Puncturer<B,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
int Puncturer<B,Q>
::get_K() const
{
	return K;
}

template <typename B, typename Q>
int Puncturer<B,Q>
::get_N() const
{
	return N;
}

template <typename B, typename Q>
int Puncturer<B,Q>
::get_N_cw() const
{
	return N_cw;
}

template <typename B, typename Q>
template <class A>
void Puncturer<B,Q>
::puncture(const std::vector<B,A>& X_N1, std::vector<B,A>& X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[pct::sck::puncture::X_N1].bind(X_N1);
	(*this)[pct::sck::puncture::X_N2].bind(X_N2);
	(*this)[pct::tsk::puncture].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Puncturer<B,Q>
::puncture(const B *X_N1, B *X_N2, const int frame_id, const bool managed_memory)
{
	(*this)[pct::sck::puncture::X_N1].bind(X_N1);
	(*this)[pct::sck::puncture::X_N2].bind(X_N2);
	(*this)[pct::tsk::puncture].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
template <class A>
void Puncturer<B,Q>
::depuncture(const std::vector<Q,A>& Y_N1, std::vector<Q,A>& Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[pct::sck::depuncture::Y_N1].bind(Y_N1);
	(*this)[pct::sck::depuncture::Y_N2].bind(Y_N2);
	(*this)[pct::tsk::depuncture].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Puncturer<B,Q>
::depuncture(const Q *Y_N1, Q *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[pct::sck::depuncture::Y_N1].bind(Y_N1);
	(*this)[pct::sck::depuncture::Y_N2].bind(Y_N2);
	(*this)[pct::tsk::depuncture].exec(frame_id, managed_memory);
}

template <typename B, typename Q>
void Puncturer<B,Q>
::_puncture(const B *X_N1, B *X_N2, const size_t frame_id) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename Q>
void Puncturer<B,Q>
::_depuncture(const Q *Y_N1, Q *Y_N2, const size_t frame_id) const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
