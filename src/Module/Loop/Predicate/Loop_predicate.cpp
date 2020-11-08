#include <string>
#include <typeindex>

#include "Module/Loop/Predicate/Loop_predicate.hpp"

namespace aff3ct
{
namespace module
{

template <typename O>
Loop_predicate<O>
::Loop_predicate(const tools::Predicate &predicate, const size_t n_elmts_out)
: Loop(1, typeid(int8_t), n_elmts_out, typeid(O)),
  predicate(predicate.clone()),
  use_hack(true),
  hack(1)
{
	const std::string name = "Loop_predicate";
	this->set_name(name);
	(*this)[lop::sck::stop::in1].bind(hack.data());
	(*this)[lop::sck::stop::in2].bind(hack.data());
}

template <typename O>
Loop_predicate<O>
::Loop_predicate(const tools::Predicate &predicate,
                 const size_t n_elmts_in,
                 const std::type_index datatype_in,
                 const size_t n_elmts_out)
: Loop(n_elmts_in, datatype_in, n_elmts_out, typeid(O)),
  predicate(predicate.clone()),
  use_hack(false)
{
	const std::string name = "Loop_predicate";
	this->set_name(name);
}

template <typename O>
Loop_predicate<O>* Loop_predicate<O>
::clone() const
{
	auto m = new Loop_predicate(*this);
	m->deep_copy(*this);
	return m;
}

template <typename O>
void Loop_predicate<O>
::deep_copy(const Loop_predicate<O> &m)
{
	Module::deep_copy(m);
	if (m.predicate != nullptr) this->predicate.reset(dynamic_cast<tools::Predicate*>(m.predicate->clone()));
	if (this->use_hack)
	{
		(*this)[lop::sck::stop::in1].bind(this->hack.data());
		(*this)[lop::sck::stop::in2].bind(this->hack.data());
	}
}

template <typename O>
tools::Predicate& Loop_predicate<O>
::get_predicate()
{
	return *this->predicate;
}

template <typename O>
void Loop_predicate<O>
::reset()
{
	Loop::reset();
	this->get_predicate().reset();
}

template <typename O>
bool Loop_predicate<O>
::_stop(const int8_t *in, const int frame_id)
{
	this->n_calls++;
	return (*this->predicate)();
}

}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Loop_predicate<B_8>;
template class aff3ct::module::Loop_predicate<B_16>;
template class aff3ct::module::Loop_predicate<B_32>;
template class aff3ct::module::Loop_predicate<B_64>;
template class aff3ct::module::Loop_predicate<R_32>;
template class aff3ct::module::Loop_predicate<R_64>;
// ==================================================================================== explicit template instantiation

