/*!
 * \file
 * \brief The Simulation is an abstract and generic container.
 *
 * \section LICENSE
 * This file is under MIT license (https://opensource.org/licenses/MIT).
 */
#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

#include <memory>
#include "Module/Module.hpp"
#include "Tools/Display/Terminal/Terminal.hpp"
#include "Factory/Simulation/Simulation.hpp"

namespace aff3ct
{
namespace simulation
{
/*!
 * \class Simulation
 *
 * \brief The Simulation is an abstract and generic container.
 */
class Simulation
{
protected:
	// parameters
	const factory::Simulation::parameters &params;

	// map of Modules
	std::map<std::string, std::vector<std::shared_ptr<module::Module>>> modules;

	bool simu_error;

public:
	/*!
	 *  \brief Constructor.
	 */
	explicit Simulation(const factory::Simulation::parameters& simu_params);

	/*!
	 *  \brief Destructor.
	 */
	virtual ~Simulation();

	bool is_error() const;

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
	typename std::enable_if<std::is_base_of<module::Module, T>::value, void>::type set_module(const std::string& module_name, const int tid, std::shared_ptr<T> mod);
};
}
}

#include "Simulation.hxx"

#endif /* SIMULATION_HPP_ */
