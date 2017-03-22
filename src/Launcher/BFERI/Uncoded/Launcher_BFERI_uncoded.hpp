#ifndef LAUNCHER_BFERI_UNCODED_HPP_
#define LAUNCHER_BFERI_UNCODED_HPP_

#include "../Launcher_BFERI.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFERI_uncoded : public Launcher_BFERI<B,R,Q>
{
public:
	Launcher_BFERI_uncoded(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFERI_uncoded() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual simulation::Simulation* build_simu();
};
}
}

#endif /* LAUNCHER_BFERI_UNCODED_HPP_ */
