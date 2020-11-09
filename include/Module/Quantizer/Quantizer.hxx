#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Quantizer/Quantizer.hpp"

namespace aff3ct
{
namespace module
{

template <typename R, typename Q>
Task& Quantizer<R,Q>
::operator[](const qnt::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename R, typename Q>
Socket& Quantizer<R,Q>
::operator[](const qnt::sck::process s)
{
	return Module::operator[]((size_t)qnt::tsk::process)[(size_t)s];
}

template <typename R, typename Q>
Quantizer<R,Q>
::Quantizer(const int N)
: Module(), N(N)
{
	const std::string name = "Quantizer";
	this->set_name(name);
	this->set_short_name(name);

	if (N <= 0)
	{
		std::stringstream message;
		message << "'N' has to be greater than 0 ('N' = " << N << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("process");
	auto ps_Y_N1 = this->template create_socket_in <R>(p, "Y_N1", this->N);
	auto ps_Y_N2 = this->template create_socket_out<Q>(p, "Y_N2", this->N);
	this->create_codelet(p, [ps_Y_N1, ps_Y_N2](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &qnt = static_cast<Quantizer<R,Q>&>(m);

		qnt._process(static_cast<R*>(t[ps_Y_N1].get_dataptr()),
		             static_cast<Q*>(t[ps_Y_N2].get_dataptr()),
		             frame_id);

		return status_t::SUCCESS;
	});
}

template <typename R, typename Q>
Quantizer<R,Q>* Quantizer<R,Q>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename R, typename Q>
int Quantizer<R,Q>
::get_N() const
{
	return N;
}

template <typename R, typename Q>
template <class AR, class AQ>
void Quantizer<R,Q>
::process(const std::vector<R,AR>& Y_N1, std::vector<Q,AQ>& Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[qnt::sck::process::Y_N1].bind(Y_N1);
	(*this)[qnt::sck::process::Y_N2].bind(Y_N2);
	(*this)[qnt::tsk::process].exec(frame_id, managed_memory);
}

template <typename R, typename Q>
void Quantizer<R,Q>
::process(const R *Y_N1, Q *Y_N2, const int frame_id, const bool managed_memory)
{
	(*this)[qnt::sck::process::Y_N1].bind(Y_N1);
	(*this)[qnt::sck::process::Y_N2].bind(Y_N2);
	(*this)[qnt::tsk::process].exec(frame_id, managed_memory);
}

template <typename R, typename Q>
void Quantizer<R,Q>
::_process(const R *Y_N1, Q *Y_N2, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
