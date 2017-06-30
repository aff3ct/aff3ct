#ifndef LAUNCHER_EXIT_HPP_
#define LAUNCHER_EXIT_HPP_

#include "Tools/Codec/Codec_SISO.hpp"
#include "Tools/Factory/Simulation/EXIT/Factory_simulation_EXIT.hpp"

#include "Simulation/EXIT/Simulation_EXIT.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_EXIT : public Launcher<B,R,Q>
{
protected:
	tools::Codec_SISO<B,R> *codec = nullptr;

	tools::Factory_simulation_EXIT::chain_parameters_EXIT<B,R,Q> * m_chain_params = nullptr;

public:
	Launcher_EXIT(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_EXIT();

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
