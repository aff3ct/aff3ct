#include "Simulation/Simulation.hpp"
#include "Tools/signal_handler.h"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation
::Simulation()
: simu_error(false)
{
	tools::setup_signal_handler();
}

bool Simulation
::is_error() const
{
	return this->simu_error;
}
