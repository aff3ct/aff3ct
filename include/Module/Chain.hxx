#include "Module/Chain.hpp"

namespace aff3ct
{
namespace module
{
size_t Chain
::get_n_threads() const
{
	return this->n_threads;
}

template <class C>
std::vector<C*> Chain
::get_modules() const
{
	std::vector<C*> ret;
	for (auto &mm : this->modules)
		for (auto &m : mm)
		{
			auto c = dynamic_cast<C*>(m.get());
			if (c != nullptr)
				ret.push_back(c);
		}

	return ret;
}

}
}
