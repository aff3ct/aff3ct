#ifndef SIMULATION_HXX_
#define SIMULATION_HXX_

#include <type_traits>
#include "Simulation.hpp"

namespace aff3ct
{
namespace simulation
{

template <typename T>
typename std::enable_if<std::is_base_of<module::Module, T>::value, void>::type Simulation
::set_module(const std::string& module_name, const int tid, const std::shared_ptr<T>& mod)
{
	this->modules[module_name][tid] = static_cast<module::Module*>(mod.get());
}

template <typename T>
typename std::enable_if<std::is_base_of<module::Module, T>::value, void>::type Simulation
::set_module(const std::string& module_name, const int tid, const std::unique_ptr<T>& mod)
{
	this->modules[module_name][tid] = static_cast<module::Module*>(mod.get());
}

}
}

#endif /* SIMULATION_HXX_ */
