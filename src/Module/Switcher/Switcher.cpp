#include "Module/Switcher/Switcher.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Switcher* Switcher
::clone() const
{
	auto m = new Switcher(*this);
	m->deep_copy(*this);
	return m;
}

void Switcher
::reset()
{
	this->path = this->get_n_data_sockets() -1;
}
