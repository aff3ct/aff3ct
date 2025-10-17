#include <streampu.hpp>

#include "Simulation/Simulation.hpp"

using namespace aff3ct;
using namespace aff3ct::simulation;

Simulation ::Simulation()
  : simu_error(false)
{
    spu::tools::Signal_handler::init();
    spu::tools::Buffer_allocator::set_task_autoalloc(true);
}

bool
Simulation ::is_error() const
{
    return this->simu_error;
}
