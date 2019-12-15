#include "Simulation/Legacy/Simulation_legacy.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename T>
typename std::enable_if<std::is_base_of<module::Module, T>::value, void>::type Simulation_legacy
::set_module(const std::string& module_name, const int tid, const T& mod)
{
	this->modules[module_name][tid] = static_cast<const module::Module*>(&mod);
}

}
}
