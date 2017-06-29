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

	tools::Header::params_list sim, cde, src, mod, demod, chn, qua, enc, dec, mon, ter;

public:
	Launcher_BFER(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();

	virtual void print_header();
};
}
}

#endif /* LAUNCHER_BFER_HPP_ */
