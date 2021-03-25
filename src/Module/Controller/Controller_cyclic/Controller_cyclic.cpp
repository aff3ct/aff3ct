#include <string>

#include "Module/Controller/Controller_cyclic/Controller_cyclic.hpp"

using namespace aff3ct;
using namespace aff3ct::module;

Controller_cyclic
::Controller_cyclic(const size_t n_paths, const size_t init_path)
: Controller(init_path % n_paths), n_paths(n_paths)
{
	const std::string name = "Controller_cyclic";
	this->set_name(name);
}

Controller_cyclic* Controller_cyclic
::clone() const
{
	auto m = new Controller_cyclic(*this);
	m->deep_copy(*this);
	return m;
}

size_t Controller_cyclic
::get_n_paths() const
{
	return this->n_paths;
}

void Controller_cyclic
::set_n_paths(const size_t n_paths)
{
	this->n_paths = n_paths;
	this->path = this->path % this->n_paths;
}

void Controller_cyclic
::set_path(const size_t path)
{
	this->path = path % this->n_paths;
}

void Controller_cyclic
::reset()
{
	this->path = this->init_path % this->n_paths;
}

void Controller_cyclic
::_control(int8_t *out, const size_t frame_id)
{
	out[0] = this->path = this->path % this->n_paths;
	this->path++;
}
