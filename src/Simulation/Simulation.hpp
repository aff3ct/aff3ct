#ifndef SIMULATION_HPP_
#define SIMULATION_HPP_

namespace aff3ct
{
namespace simulation
{
class Simulation
{
protected:
	bool simu_error;

public:
	Simulation();

	virtual ~Simulation() = default;

	virtual void launch() = 0;

	bool is_error() const;
};
}
}

#endif /* SIMULATION_HPP_ */
