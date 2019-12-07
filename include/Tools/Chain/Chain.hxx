#include "Module/Subchain/Subchain.hpp"
#include "Tools/Chain/Chain.hpp"

namespace aff3ct
{
namespace tools
{
size_t Chain
::get_n_threads() const
{
	return this->n_threads;
}

template <class C>
std::vector<C*> Chain
::get_modules(const bool subchain_modules) const
{
	std::vector<C*> ret;
	for (auto &mm : this->modules)
		for (auto &m : mm)
		{
			if (subchain_modules)
			{
				auto c = dynamic_cast<module::Subchain*>(m.get());
				if (c != nullptr)
				{
					auto subret = c->get_chain().get_modules<C>(subchain_modules);
					ret.insert(ret.end(), subret.begin(), subret.end());
				}
			}

			auto c = dynamic_cast<C*>(m.get());
			if (c != nullptr)
				ret.push_back(c);
		}

	return ret;
}

const std::vector<std::vector<module::Task*>>& Chain
::get_tasks_sequences() const
{
	return this->tasks_sequences;
}

const std::vector<module::Task*>& Chain
::get_tasks_sequence(const int tid) const
{
	return this->tasks_sequences[tid];
}

}
}
