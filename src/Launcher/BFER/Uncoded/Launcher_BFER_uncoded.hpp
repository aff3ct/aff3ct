#ifndef LAUNCHER_BFER_UNCODED_HPP_
#define LAUNCHER_BFER_UNCODED_HPP_

#include "../Launcher_BFER.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B, typename R, typename Q>
class Launcher_BFER_uncoded : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_uncoded(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_uncoded() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual simulation::Simulation* build_simu();
};
}
}

#endif /* LAUNCHER_BFER_UNCODED_HPP_ */
