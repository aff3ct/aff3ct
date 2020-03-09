#include "Tools/Chain/Pipeline.hpp"

namespace aff3ct
{
namespace tools
{

template <class C>
std::vector<C*> Pipeline
::get_modules(const bool subchain_modules) const
{
	std::vector<C*> ret;
	for (auto &stage : this->stages)
	{
		auto modules = stage->get_modules<C>(subchain_modules);
		ret.insert(ret.end(), modules.begin(), modules.end());
	}
	return ret;
}

}
}
