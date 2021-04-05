#include <chrono>
#include <sstream>

#include "Module/Incrementer/Incrementer.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

template <typename T>
Incrementer<T>
::Incrementer(const size_t n_elmts, const size_t ns)
: Module(), n_elmts(n_elmts), ns(ns)
{
	const std::string name = "Incrementer";
	this->set_name(name);
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("increment");
	auto ps_in  = this->template create_socket_in <T>(p, "in",  this->n_elmts);
	auto ps_out = this->template create_socket_out<T>(p, "out", this->n_elmts);
	this->create_codelet(p, [ps_in, ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &inc = static_cast<Incrementer&>(m);
		inc._increment(static_cast<const T*>(t[ps_in ].get_dataptr()),
		               static_cast<      T*>(t[ps_out].get_dataptr()),
		               frame_id);
		return status_t::SUCCESS;
	});
}

template <typename T>
Incrementer<T>* Incrementer<T>
::clone() const
{
	auto m = new Incrementer(*this);
	m->deep_copy(*this);
	return m;
}

template <typename T>
size_t Incrementer<T>
::get_ns() const
{
	return this->ns;
}

template <typename T>
size_t Incrementer<T>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename T>
void Incrementer<T>
::set_ns(const size_t ns)
{
	this->ns = ns;
}

template <typename T>
void Incrementer<T>
::increment(const T *in, T *out, const int frame_id, const bool managed_memory)
{
	(*this)[inc::sck::increment::in ].bind(in );
	(*this)[inc::sck::increment::out].bind(out);
	(*this)[inc::tsk::increment].exec(frame_id, managed_memory);
}

template <typename T>
void Incrementer<T>
::_increment(const T *in, T *out, const size_t frame_id)
{
	std::chrono::time_point<std::chrono::steady_clock> t_start;
	if (this->ns)
		t_start = std::chrono::steady_clock::now();

	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = in[e] +1;

	if (this->ns)
	{
		std::chrono::nanoseconds duration = std::chrono::steady_clock::now() - t_start;
		while ((size_t)duration.count() < this->ns) // active waiting
			duration = std::chrono::steady_clock::now() - t_start;
	}
}

// ==================================================================================== explicit template instantiation
template class aff3ct::module::Incrementer<int8_t>;
template class aff3ct::module::Incrementer<int16_t>;
template class aff3ct::module::Incrementer<int32_t>;
template class aff3ct::module::Incrementer<int64_t>;
template class aff3ct::module::Incrementer<float>;
template class aff3ct::module::Incrementer<double>;
// ==================================================================================== explicit template instantiation

