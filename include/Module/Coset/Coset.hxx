#include <string>
#include <sstream>

#include "Tools/Exception/exception.hpp"
#include "Module/Coset/Coset.hpp"

namespace aff3ct
{
namespace module
{

template <typename B, typename D>
Task& Coset<B,D>
::operator[](const cst::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename B, typename D>
Socket& Coset<B,D>
::operator[](const cst::sck::apply s)
{
	return Module::operator[]((size_t)cst::tsk::apply)[(size_t)s];
}

template <typename B, typename D>
Coset<B,D>
::Coset(const int size)
: Module(), size(size)
{
	const std::string name = "Coset";
	this->set_name(name);
	this->set_short_name(name);

	if (size <= 0)
	{
		std::stringstream message;
		message << "'size' has to be greater than 0. ('size' = " << size << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("apply");
	auto ps_ref = this->template create_socket_in <B>(p, "ref", this->size);
	auto ps_in  = this->template create_socket_in <D>(p, "in",  this->size);
	auto ps_out = this->template create_socket_out<D>(p, "out", this->size);
	this->create_codelet(p, [ps_ref, ps_in, ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &cst = static_cast<Coset<B,D>&>(m);

		cst._apply(static_cast<B*>(t[ps_ref].get_dataptr()),
		           static_cast<D*>(t[ps_in ].get_dataptr()),
		           static_cast<D*>(t[ps_out].get_dataptr()),
		           frame_id);

		return status_t::SUCCESS;
	});
}

template <typename B, typename D>
Coset<B,D>* Coset<B,D>
::clone() const
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

template <typename B, typename D>
int Coset<B,D>
::get_size() const
{
	return this->size;
}

template <typename B, typename D>
template <class AB, class AD>
void Coset<B,D>
::apply(const std::vector<B,AB>& ref, const std::vector<D,AD> &in, std::vector<D,AD> &out, const int frame_id,
        const bool managed_memory)
{
	(*this)[cst::sck::apply::ref].bind(ref);
	(*this)[cst::sck::apply::in ].bind(in );
	(*this)[cst::sck::apply::out].bind(out);
	(*this)[cst::tsk::apply].exec(frame_id, managed_memory);
}

template <typename B, typename D>
void Coset<B,D>
::apply(const B *ref, const D *in, D *out, const int frame_id, const bool managed_memory)
{
	(*this)[cst::sck::apply::ref].bind(ref);
	(*this)[cst::sck::apply::in ].bind(in );
	(*this)[cst::sck::apply::out].bind(out);
	(*this)[cst::tsk::apply].exec(frame_id, managed_memory);
}

template <typename B, typename D>
void Coset<B,D>
::_apply(const B *ref, const D *in, D *out, const size_t frame_id)
{
	throw tools::unimplemented_error(__FILE__, __LINE__, __func__);
}

}
}
