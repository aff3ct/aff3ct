#ifndef LAUNCHER_EXIT_HPP_
#define LAUNCHER_EXIT_HPP_

#include "Tools/Codec/Codec_SISO.hpp"

#include "Factory/Simulation/EXIT/Simulation_EXIT.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float>
class EXIT : public Launcher
{
protected:
	tools::Codec_SISO<B,R> *codec = nullptr;
	factory::Simulation_EXIT::parameters *params;

public:
	EXIT(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~EXIT();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual void build_codec() = 0;

	virtual simulation::Simulation* build_simu();

	virtual void print_header();
};
}
}

#endif /* LAUNCHER_EXIT_HPP_ */
