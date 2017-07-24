#ifndef LAUNCHER_BFER_STD_HPP_
#define LAUNCHER_BFER_STD_HPP_

#include "Factory/Simulation/BFER/BFER_std.hpp"

#include "Tools/Codec/Codec.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class BFER_std : public Launcher
{
protected:
	tools::Codec<B,Q> *codec = nullptr;
	factory::BFER_std::parameters *params;

public:
	BFER_std(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~BFER_std();

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

#endif /* LAUNCHER_BFER_STD_HPP_ */
