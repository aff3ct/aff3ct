#include "Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation
::Simulation(const factory::Simulation::parameters& simu_params)
: params(simu_params)
{
}

Simulation
::~Simulation()
{
}

void Simulation
::build_communication_chain()
{
	_build_communication_chain();

	for (auto &m : modules)
		for (auto mm : m.second)
			if (mm != nullptr)
				for (auto &t : mm->tasks)
				{
					t.second->set_autoexec (true);
					t.second->set_autoalloc(true);
					t.second->set_stats    (true);

					// enable the debug mode in the modules
					if (params.debug)
					{
						t.second->set_debug(true);
						if (params.debug_limit)
							t.second->set_debug_limit((uint32_t)params.debug_limit);
						if (params.debug_precision)
							t.second->set_debug_precision((uint8_t)params.debug_precision);
					}
				}
}
