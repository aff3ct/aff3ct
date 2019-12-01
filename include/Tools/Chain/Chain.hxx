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
