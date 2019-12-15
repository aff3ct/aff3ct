#include "Simulation/Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation
::Simulation()
: simu_error(false)
{

}

bool Simulation
::is_error() const
{
	return this->simu_error;
}
