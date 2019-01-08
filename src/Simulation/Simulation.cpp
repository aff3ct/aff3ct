#include <sstream>
#include "Tools/Exception/exception.hpp"

#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation
::Simulation(const factory::Simulation::parameters& simu_params)
: params(simu_params), simu_error(false)
{
}

bool Simulation
::is_error() const
{
	return this->simu_error;
}

void Simulation
::build_communication_chain()
{
	_build_communication_chain();

	for (auto &m : modules)
		for (auto& mm : m.second)
			if (mm != nullptr)
				for (auto &t : mm->tasks)
				{
					t->set_autoalloc(true);

					if (params.statistics)
						t->set_stats(true);

					// enable the debug mode in the modules
					if (params.debug)
					{
						t->set_debug(true);
						t->set_debug_hex(params.debug_hex);
						if (params.debug_limit)
							t->set_debug_limit((uint32_t)params.debug_limit);
						if (params.debug_precision)
							t->set_debug_precision((uint8_t)params.debug_precision);
						if (params.debug_frame_max)
							t->set_debug_frame_max((uint32_t)params.debug_frame_max);
					}

					if (!t->is_stats() && !t->is_debug())
						t->set_fast(true);
				}
}

void Simulation
::add_module(const std::string& module_name, const int n_threads)
{
	if (this->modules.find(module_name) != this->modules.end())
	{
		std::stringstream message;
		message << "'module_name' has already been given in the modules list ('module_name' = " << module_name << ").";
		throw tools::invalid_argument(__FILE__, __LINE__, __func__, message.str());
	}

	this->modules[module_name] = std::vector<const module::Module*>(n_threads);
}
