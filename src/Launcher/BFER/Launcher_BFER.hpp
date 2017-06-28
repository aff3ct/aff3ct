#ifndef LAUNCHER_BFER_HPP_
#define LAUNCHER_BFER_HPP_

#include "Tools/Codec/Codec.hpp"
#include "Tools/Factory/Simulation/Factory_simulation_BFER_std.hpp"

#include "../Launcher.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFER : public Launcher<B,R,Q>
{
protected:
	tools::Codec<B,Q> *codec;

	tools::Factory_simulation_BFER_std::chain_parameters_BFER_std<B,R,Q> * m_chain_params = nullptr;

public:
	Launcher_BFER(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual std::vector<std::pair<std::string,std::string>> header_simulation();
	virtual std::vector<std::pair<std::string,std::string>> header_code      ();
	virtual std::vector<std::pair<std::string,std::string>> header_encoder   ();
	virtual std::vector<std::pair<std::string,std::string>> header_decoder   ();
	virtual std::vector<std::pair<std::string,std::string>> header_monitor   ();
};
}
}

#endif /* LAUNCHER_BFER_HPP_ */
