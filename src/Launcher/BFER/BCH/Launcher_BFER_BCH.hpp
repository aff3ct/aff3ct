#ifndef LAUNCHER_BFER_BCH_HPP_
#define LAUNCHER_BFER_BCH_HPP_

#include "Tools/Factory/BCH/Factory_decoder_BCH.hpp"
#include "Tools/Factory/BCH/Factory_encoder_BCH.hpp"
#include "../Launcher_BFER.hpp"

namespace aff3ct
{
namespace launcher
{
template <typename B = int, typename R = float, typename Q = R>
class Launcher_BFER_BCH : public Launcher_BFER<B,R,Q>
{
public:
	Launcher_BFER_BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BFER_BCH();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual simulation::Simulation* build_simu();

	typename tools::Factory_encoder_BCH<B  >::encoder_parameters     *m_enc = nullptr;
	typename tools::Factory_decoder_BCH<B,Q>::decoder_parameters_BCH *m_dec = nullptr;
};
}
}

#endif /* LAUNCHER_BFER_BCH_HPP_ */
