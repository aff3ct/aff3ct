#ifndef LAUNCHER_BCH_HPP_
#define LAUNCHER_BCH_HPP_

#include "Tools/Factory/BCH/Factory_decoder_BCH.hpp"
#include "Tools/Factory/BCH/Factory_encoder_BCH.hpp"

namespace aff3ct
{
namespace launcher
{
template <class cLauncher, typename B = int, typename R = float, typename Q = R>
class Launcher_BCH : public cLauncher
{
protected :
	typename tools::Factory_encoder_BCH<B  >::encoder_parameters     *m_enc = nullptr;
	typename tools::Factory_decoder_BCH<B,Q>::decoder_parameters_BCH *m_dec = nullptr;

public:
	Launcher_BCH(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_BCH();

protected:
	virtual void build_args();
	virtual void store_args();
	virtual void group_args();
	virtual void print_header();

	virtual void build_codec();

};
}
}

#include "Launcher_BCH.hxx"

#endif /* LAUNCHER_BCH_HPP_ */
