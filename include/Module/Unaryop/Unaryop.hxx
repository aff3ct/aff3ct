#include "Module/Unaryop/Unaryop.hpp"

namespace aff3ct
{
namespace module
{

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
Task& Unaryop<TI,TO,UOP>
::operator[](const uop::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
Socket& Unaryop<TI,TO,UOP>
::operator[](const uop::sck::perform s)
{
	return Module::operator[]((size_t)uop::tsk::perform)[(size_t)s];
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
Unaryop<TI,TO,UOP>
::Unaryop(const size_t n_elmts)
: Module(), n_elmts(n_elmts)
{
	const std::string name = "Unaryop";
	this->set_name(name + "<" + tools::uop_get_name<TI,TO,UOP>() + ">");
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("perform");
	auto ps_in  = this->template create_socket_in <TI>(p, "in",  this->n_elmts);
	auto ps_out = this->template create_socket_out<TO>(p, "out", this->n_elmts);
	this->create_codelet(p, [ps_in, ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &uop = static_cast<Unaryop&>(m);
		uop._perform(static_cast<const TI*>(t[ps_in ].get_dataptr()),
		             static_cast<      TO*>(t[ps_out].get_dataptr()),
		             frame_id);
		return status_t::SUCCESS;
	});
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
Unaryop<TI,TO,UOP>* Unaryop<TI,TO,UOP>
::clone() const
{
	auto m = new Unaryop(*this);
	m->deep_copy(*this);
	return m;
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
size_t Unaryop<TI,TO,UOP>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
template <class AI, class AO>
void Unaryop<TI,TO,UOP>
::perform(const std::vector<TI,AI>& in,
                std::vector<TO,AO>& out,
          const int frame_id,
          const bool managed_memory)
{
	(*this)[uop::sck::perform::in ].bind(in );
	(*this)[uop::sck::perform::out].bind(out);
	(*this)[uop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
void Unaryop<TI,TO,UOP>
::perform(const TI *in, TO *out, const int frame_id, const bool managed_memory)
{
	(*this)[uop::sck::perform::in ].bind(in );
	(*this)[uop::sck::perform::out].bind(out);
	(*this)[uop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_uop<TI,TO> UOP>
void Unaryop<TI,TO,UOP>
::_perform(const TI *in, TO *out, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = UOP(in[e]);
}

}
}
