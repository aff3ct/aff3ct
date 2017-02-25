#ifndef LAUNCHER_BFER_REPETITION_HPP_
#define LAUNCHER_BFER_REPETITION_HPP_

#include "../Launcher_BFER.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B, typename R, typename Q>
class Launcher_BFER_repetition : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_repetition(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_repetition() {};

protected:
	virtual void build_args();
	virtual void store_args();
	virtual simulation::Simulation* build_simu();

	virtual std::vector<std::pair<std::string,std::string>> header_encoder();
};
}
}

#endif /* LAUNCHER_BFER_REPETITION_HPP_ */
