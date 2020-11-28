#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Source/Source.hpp"

namespace aff3ct
{
namespace module
{

template <typename B>
Task& Source<B>
::operator[](const src::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B>
Socket& Source<B>
::operator[](const src::sck::generate s)
{
	return Module::operator[]((size_t)src::tsk::generate)[(size_t)s];
}

template <typename B>
Source<B>
::Source(const int K)
: Module(), K(K)
{
	const std::string name = "Source";
	this->set_name(name);
	this->set_short_name(name);

	if (K <= 0)
	{
		std::stringstream message;
		message << "'K' has to be greater than 0 ('K' = " << K << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("generate");
	auto ps_U_K = this->template create_socket_out<B>(p, "U_K", this->K);
	this->create_codelet(p, [ps_U_K](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &src = static_cast<Source<B>&>(m);

		src._generate(static_cast<B*>(t[ps_U_K].get_dataptr()),
		              frame_id);

		return status_t::SUCCESS;
	});
}

template <typename B>
Source<B>* Source<B>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
int Source<B>
::get_K() const
{
	return K;
}

template <typename B>
template <class A>
void Source<B>
::generate(std::vector<B,A>& U_K, const int frame_id, const bool managed_memory)
{
	(*this)[src::sck::generate::U_K].bind(U_K);
	(*this)[src::tsk::generate].exec(frame_id, managed_memory);
}

template <typename B>
void Source<B>
::generate(B *U_K, const int frame_id, const bool managed_memory)
{
	(*this)[src::sck::generate::U_K].bind(U_K);
	(*this)[src::tsk::generate].exec(frame_id, managed_memory);
}
template <typename B>
void Source<B>
::_generate(B *U_K, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B>
void Source<B>
::set_seed(const int seed)
{
	// do nothing in the general case, this method has to be overrided
}

template <typename B>
bool Source<B>
::is_done() const
{
	return false;
}

template <typename B>
void Source<B>
::reset()
{
	// do nothing in the general case, this method has to be overrided
}

}
}
