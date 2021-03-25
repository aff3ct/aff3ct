#include "Module/Reducer/Reducer.hpp"

namespace aff3ct
{
namespace module
{

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Task& Reducer<TI,TO,BOP>
::operator[](const red::tsk t)
{
	return Module::operator[]((size_t)t);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Socket& Reducer<TI,TO,BOP>
::operator[](const red::sck::reduce s)
{
	return Module::operator[]((size_t)red::tsk::reduce)[(size_t)s];
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Reducer<TI,TO,BOP>
::Reducer(const size_t n_elmts)
: Module(), n_elmts(n_elmts)
{
	const std::string name = "Reducer";
	this->set_name(name + "<" + tools::bop_get_name<TI,TO,BOP>() + ">");
	this->set_short_name(name);
	this->set_single_wave(true);

	if (n_elmts == 0)
	{
		std::stringstream message;
		message << "'n_elmts' has to be greater than 0 ('n_elmts' = " << n_elmts << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	auto &p = this->create_task("reduce");
	auto ps_in  = this->template create_socket_in <TI>(p, "in",  this->n_elmts);
	auto ps_out = this->template create_socket_out<TO>(p, "out", 1);
	this->create_codelet(p, [ps_in, ps_out](Module &m, Task &t, const size_t frame_id) -> int
	{
		auto &red = static_cast<Reducer&>(m);
		red._reduce(static_cast<const TI*>(t[ps_in ].get_dataptr()),
		            static_cast<      TO*>(t[ps_out].get_dataptr()),
		            frame_id);
		return status_t::SUCCESS;
	});
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
Reducer<TI,TO,BOP>* Reducer<TI,TO,BOP>
::clone() const
{
	auto m = new Reducer(*this);
	m->deep_copy(*this);
	return m;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
size_t Reducer<TI,TO,BOP>
::get_n_elmts() const
{
	return this->n_elmts;
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
template <class AI, class AO>
void Reducer<TI,TO,BOP>
::reduce(const std::vector<TI,AI>& in, std::vector<TO,AO>& out, const int frame_id, const bool managed_memory)
{
	(*this)[red::sck::reduce::in ].bind(in );
	(*this)[red::sck::reduce::out].bind(out);
	(*this)[red::tsk::reduce].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Reducer<TI,TO,BOP>
::reduce(const TI *in, TO *out, const int frame_id, const bool managed_memory)
{
	(*this)[red::sck::reduce::in ].bind(in );
	(*this)[red::sck::reduce::out].bind(out);
	(*this)[red::tsk::reduce].exec(frame_id, managed_memory);
}

template <typename TI, typename TO, tools::proto_bop<TI,TO> BOP>
void Reducer<TI,TO,BOP>
::_reduce(const TI *in, TO *out, const size_t frame_id)
{
	TO reduced_val = (TO)(in[0]);
	for (size_t f = 0; f < this->get_n_frames(); f++)
	{
		const size_t start = (f == 0) ? 1 : 0;
		for (size_t e = start; e < this->n_elmts; e++)
			reduced_val = BOP((TI)reduced_val, in[f * this->n_elmts + e]);
	}
	out[0] = reduced_val;
}

}
}
