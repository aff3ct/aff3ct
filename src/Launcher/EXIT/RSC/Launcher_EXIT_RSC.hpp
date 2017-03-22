#ifndef LAUNCHER_EXIT_RSC_HPP_
#define LAUNCHER_EXIT_RSC_HPP_

#include "../Launcher_EXIT.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R, typename QD = Q>
class Launcher_EXIT_RSC : public Launcher_EXIT<B,R,Q>
{
public:
	Launcher_EXIT_RSC(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_EXIT_RSC() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual simulation::Simulation* build_simu();

	virtual std::vector<std::pair<std::string,std::string>> header_encoder();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder();
};
}
}

#endif /* LAUNCHER_EXIT_RSC_HPP_ */
