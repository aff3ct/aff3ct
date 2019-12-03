#include <string>

#include "Module/Router/Predicate/Router_predicate.hpp"

namespace aff3ct
{
namespace module
{

template <typename O>
Router_predicate<O>
::Router_predicate(const tools::Predicate &predicate, const size_t n_elmts_out, const int n_frames)
: Router(8, n_elmts_out * sizeof(O), 2, n_frames),
  predicate(predicate.clone()),
  hack(1)
{
	const std::string name = "Router_predicate";
	this->set_name(name);
	(*this)[rtr::tsk::route].sockets[0]->bind(hack.data());
}

template <typename O>
Router_predicate<O>* Router_predicate<O>
::clone() const
{
	auto m = new Router_predicate(*this);
	m->deep_copy(*this);
	(*m)[rtr::tsk::route].sockets[0]->bind(m->hack.data());
	return m;
}

template <typename O>
void Router_predicate<O>
::deep_copy(const Router_predicate<O> &m)
{
	Module::deep_copy(m);
	if (m.predicate != nullptr) this->predicate.reset(dynamic_cast<tools::Predicate*>(m.predicate->clone()));
}

template <typename O>
tools::Predicate& Router_predicate<O>
::get_predicate()
{
	return *this->predicate;
}

template <typename O>
void Router_predicate<O>
::reset()
{
	this->get_predicate().reset();
}

template <typename O>
size_t Router_predicate<O>
::_route(const int8_t *in, const int frame_id)
{
	return (*this->predicate)() ? 1 : 0;
}

template <typename O>
size_t Router_predicate<O>
::select_route_inter(const size_t a, const size_t b)
{
	return a;
}

}
}

// ==================================================================================== explicit template instantiation
#include "Tools/types.h"
template class aff3ct::module::Router_predicate<B_8>;
template class aff3ct::module::Router_predicate<B_16>;
template class aff3ct::module::Router_predicate<B_32>;
template class aff3ct::module::Router_predicate<B_64>;
template class aff3ct::module::Router_predicate<R_32>;
template class aff3ct::module::Router_predicate<R_64>;
// ==================================================================================== explicit template instantiation

