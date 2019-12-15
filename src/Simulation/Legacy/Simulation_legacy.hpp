#ifndef SIMULATION_LEGACY_HPP_
#define SIMULATION_LEGACY_HPP_

#include <type_traits>
#include <vector>
#include <memory>
#include <string>
#include <map>

#include "Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"
#include "Simulation/Simulation.hpp"
#include "Module/Module.hpp"

namespace aff3ct
{
namespace simulation
{
class Simulation_legacy : public Simulation
{
protected:
	// parameters
	const factory::Simulation &params;

	// map of Modules
	std::map<std::string, std::vector<const module::Module*>> modules;

public:
	/*!
	 *  \brief Constructor.
	 */
	explicit Simulation_legacy(const factory::Simulation& simu_params);

	/*!
	 *  \brief Destructor.
	 */
	virtual ~Simulation_legacy() = default;

	/*!
	 *  \brief This method has to be overloaded, this is the start point of the Simulation.
	 */
	virtual void launch() = 0;

protected:
	void build_communication_chain();
	virtual void _build_communication_chain() = 0;

	void add_module(const std::string& module_name, const int n_threads);

	// cast module T in module::Module
	template <typename T>
	typename std::enable_if<std::is_base_of<module::Module, T>::value, void>::type
	set_module(const std::string& module_name, const int tid, const T& mod);
};
}
}

#include "Simulation/Legacy/Simulation_legacy.hxx"

#endif /* SIMULATION_LEGACY_HPP_ */
