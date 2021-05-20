#include "Module/Binaryop/Binaryop.hpp"

namespace aff3ct
{
namespace module
{

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Task& Binaryop<TI,TO,BOP>
::operator[](const bop::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Socket& Binaryop<TI,TO,BOP>
::operator[](const bop::sck::perform s)
{
	return Module::operator[]((size_t)bop::tsk::perform)[(size_t)s];
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Binaryop<TI,TO,BOP>
::Binaryop(const size_t n_elmts)
: Module(), n_elmts(n_elmts)
{
	const std::string name = "Binaryop";
	this->set_name(name + "<" + tools::bop_get_name<TI,TO,BOP>() + ">");
	this->set_short_name(name);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("perform");
	auto ps1_in = this->template create_socket_in <TI>(p, "in1", this->n_elmts);
	auto ps2_in = this->template create_socket_in <TI>(p, "in2", this->n_elmts);
	auto ps_out = this->template create_socket_out<TO>(p, "out", this->n_elmts);
	this->create_codelet(p, [ps1_in, ps2_in, ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &bop = static_cast<Binaryop&>(m);
		bop._perform(static_cast<const TI*>(t[ps1_in].get_dataptr()),
		             static_cast<const TI*>(t[ps2_in].get_dataptr()),
		             static_cast<      TO*>(t[ps_out].get_dataptr()),
		             frame_id);
		return status_t::SUCCESS;
	});
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Binaryop<TI,TO,BOP>* Binaryop<TI,TO,BOP>
::clone() const
{
	auto m = new Binaryop(*this);
	m->deep_copy(*this);
	return m;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
size_t Binaryop<TI,TO,BOP>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
template <class AI, class AO>
void Binaryop<TI,TO,BOP>
::perform(const std::vector<TI,AI>& in1,
          const std::vector<TI,AI>& in2,
                std::vector<TO,AO>& out,
          const int frame_id,
          const bool managed_memory)
{
	(*this)[bop::sck::perform::in1].bind(in1);
	(*this)[bop::sck::perform::in2].bind(in2);
	(*this)[bop::sck::perform::out].bind(out);
	(*this)[bop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::perform(const TI *in1, const TI *in2, TO *out, const int frame_id, const bool managed_memory)
{
	(*this)[bop::sck::perform::in1].bind(in1);
	(*this)[bop::sck::perform::in2].bind(in2);
	(*this)[bop::sck::perform::out].bind(out);
	(*this)[bop::tsk::perform].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Binaryop<TI,TO,BOP>
::_perform(const TI *in1, const TI *in2, TO *out, const size_t frame_id)
{
	for (size_t e = 0; e < this->n_elmts; e++)
		out[e] = BOP(in1[e], in2[e]);
}

}
}
